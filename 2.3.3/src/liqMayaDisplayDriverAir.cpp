/*
**
** The contents of this file are subject to the Mozilla Public License Version
** 1.1 (the "License"); you may not use this file except in compliance with
** the License. You may obtain a copy of the License at
** http://www.mozilla.org/MPL/
**
** Software distributed under the License is distributed on an "AS IS" basis,
** WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
** for the specific language governing rights and limitations under the
** License.
**
** The Original Code is the Liquid Rendering Toolkit.
**
** The Initial Developer of the Original Code is Colin Doncaster. Portions
** created by Colin Doncaster are Copyright (C) 2002. All Rights Reserved.
**
** Contributor(s): Philippe Leprince.
**
**
** The RenderMan (R) Interface Procedures and Protocol are:
** Copyright 1988, 1989, Pixar
** All Rights Reserved
**
**
** RenderMan (R) is a registered trademark of Pixar
*/

/* ______________________________________________________________________
**
** Liquid display driver for the maya render view
** ______________________________________________________________________
*/


#include "liqMayaDisplayDriverAir.h"

#include <errno.h>

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using namespace std;
#define HERE  cout<<"at "<<__LINE__<<" in "<<__FUNCTION__<<endl;
#define INFO(EXPR,ENDL) cout<<#EXPR<<" "<<EXPR<<" ";if(ENDL)cout<<endl;
static int timeout = 30;
static int recoverFlag=0;
static int socketId = -1;

int sendSockData(int s,char * data,int n);


#ifndef _WIN32
#define closesocket close
#endif

#ifdef __cplusplus
extern "C"
{
  
PtDspyError DspyFindStringInParamList(const char *string, char **result, int n, const UserParameter *p)
{
	int i;

	for (i = 0; i < n; i++, p++)
	{
		if ( p->ptype == 's' && p->name[0] == string[0] && strcmp(p->name, string) == 0) // PkString
		{
			*result = *(char**)p->data;
			return(PkDspyErrorNone);
		}
	}

return(PkDspyErrorNoResource);
}

PtDspyError DspyFindMatrixInParamList(const char *string, float *result, int n, const UserParameter *p)
{
	int i;

	for (i = 0; i < n; i++, p++)
	{
		if (p->ptype == 'f' && p->size == 16 && p->name[0] == string[0] && strcmp(p->name, string) == 0) // PkFloat
		{
			memcpy(result, (float *)p->data, 16 * sizeof(float));
			return(PkDspyErrorNone);
		}
	}
	return(PkDspyErrorNoResource);
}

PtDspyError DspyFindFloatInParamList(const char *string, float *result, int n, const UserParameter *p)
{
	int i;

	for (i = 0; i < n; i++, p++)
	{
		if ((p->ptype == 'f' || p->ptype == 'i') && p->name[0] == string[0] && strcmp(p->name, string) == 0)
		{
			if (p->ptype == 'f')
			{
				*result = *(float *)p->data;
				return(PkDspyErrorNone);
			}
			else
			{
				*result = (float)(*(int *)p->data);
				return(PkDspyErrorNone);
			}
		}
	}
	return(PkDspyErrorNoResource);
}

PtDspyError DspyFindFloatsInParamList(const char *string, int *resultCount, float *result, int n, const UserParameter *p)
{
	int i, j, *ip;

	for (i = 0; i < n; i++, p++)
	{
		if ((p->ptype == 'f' || p->ptype == 'i') && p->name[0] == string[0] && strcmp(p->name, string) == 0)
		{
			if (p->size < *resultCount)
				*resultCount = p->size;
			if (p->ptype == 'f')
			{
				memcpy(result, (float *)p->data, *resultCount * sizeof(float));
				return PkDspyErrorNone;
			}
			else
			{
				for (j = 0, ip = (int *)p->data; j < *resultCount; j++)
					*result++ = (float)*ip++;
				return(PkDspyErrorNone);
			}
		}
	}

	return(PkDspyErrorNoResource);
}

PtDspyError DspyFindIntInParamList(const char *str, int *result, int n, const UserParameter *p)
{
	int i;
  
  printf("DspyFindIntInParamList name = %s n = %d\n", str, n );
	for (i = 0; i < n; i++, p++)
	{
		printf("DspyFindIntInParamList p->name = %s p->ptype = %c p->nelements = %d\n", p->name, p->ptype, p->nelements );
    
    if ((p->ptype == 'i' || p->ptype == 'f') && p->name[0] == str[0] && strcmp(p->name, str) == 0)
		{
      if (p->ptype == 'i')
			{
				*result = *(int *)p->data;
				return PkDspyErrorNone;
			}
			else
			{
				*result = (int)(*(float *)p->data);
				return(PkDspyErrorNone);
			}
		}
	}

	return(PkDspyErrorNoResource);
}

PtDspyError DspyFindIntsInParamList(const char *string, int *resultCount, int *result, int n, const UserParameter *p)
{
	int i, j;
	float *fp;

	for (i = 0; i < n; i++, p++)
	{
		if ((p->ptype == 'i' || p->ptype == 'f') && p->name[0] == string[0] && strcmp(p->name, string) == 0)
		{
			if (p->size < *resultCount)
				*resultCount = p->size;
			if (p->ptype == 'i')
			{
				memcpy(result, (int *)p->data, *resultCount * sizeof(int));
				return PkDspyErrorNone;
			}
			else
			{
				for (j = 0, fp = (float *)p->data; j < *resultCount; j++)
					*result++ = (int)*fp++;
				return(PkDspyErrorNone);
			}
		}
	}
	return PkDspyErrorNoResource;
}

PtDspyError DspyReorderFormatting(int formatCount, PtDspyDevFormat *format, int outFormatCount, const PtDspyDevFormat *outFormat)
{
	PtDspyError ret = PkDspyErrorNone;
	int i, j;

	if (formatCount < outFormatCount)
		outFormatCount = formatCount;

	for (i = 0; i < outFormatCount; i++)
	{
		for (j = i; j < formatCount; j++)
		{
			if (format[j].name[0] == outFormat[i].name[0] && strcmp(format[j].name, outFormat[i].name) == 0)
			{
				if (i != j)
				{
					PtDspyDevFormat tmpFormat;
					tmpFormat = format[i];
					format[i] = format[j];
					format[j] = tmpFormat;
				}
				if (outFormat[i].type)
					format[i].type = outFormat[i].type;
				break;
			}
		}
		if (j >= formatCount)
			ret = PkDspyErrorBadParams;
	}

	return(ret);
}

void DspyMemReverseCopy(unsigned char *t, const unsigned char *s, int n)
{
	int i;

	s += n;
	for (i = 0; i < n; i++)
		*t++ = *--s;
}

} // extern "C"
#endif

PtDspyError
DspyImageOpen(PtDspyImageHandle *pvImage,
              const char *drivername,
              const char *filename,
              int width,
              int height,
              int paramCount,
              const UserParameter *parameters,
              int formatCount,
              PtDspyDevFormat *format,
              PtFlagStuff *flagstuff) 
{
	int i,rc;
  int status = 0;
  static int origin[2];
  static int originalSize[2];
	
	static int port = 6667;

	if (0 == width)
		width = 640;
	if (0 == height)
		height = 480;

	//format can be rgb, rgba, rgbaz or rgbz
	char* rgba[5] = {"r","g","b","a","z"};
  
  printf( "[d_liqmaya] DspyImageOpen\n" );
  printf( "[d_liqmaya] drivername = %s\n", drivername );
  printf( "[d_liqmaya] filename = %s\n", filename );
  printf( "[d_liqmaya] width = %d height = %d\n", width, height );
  printf( "[d_liqmaya] paramCount = %d\n", paramCount );

#ifdef _WIN32
	WSADATA wsaData;
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) 
	{
		WSACleanup();
		return PkDspyErrorNoResource;
	}
#endif

	PtDspyDevFormat *outformat = new PtDspyDevFormat[formatCount];
	PtDspyDevFormat *f_ptr = outformat;
	for( i=0 ; i<formatCount ; i++,++f_ptr )
  {
		f_ptr->type = PkDspyFloat32;
		f_ptr->name = rgba[i];
	}
  
  DspyReorderFormatting(formatCount,format,formatCount,outformat);
  
	char hostname[32] = "localhost", *h;
  
  printf( "[d_liqmaya] DspyFindStringInParamList host\n" );
	if(PkDspyErrorNone==DspyFindStringInParamList("host",&h,paramCount,parameters))
		strcpy(hostname,h);
	
  printf( "[d_liqmaya] hostname = %s\n", hostname );
  
	printf( "[d_liqmaya] DspyFindIntsInParamList origin\n" );
  if(PkDspyErrorNone!=DspyFindIntsInParamList("origin",&rc,origin,paramCount,parameters)) 
  {
		origin[0]= 0;
		origin[1]= 0;
	}
  printf( "[d_liqmaya] origin x = %d y = %d\n", origin[0], origin[1] );
	
  printf( "[d_liqmaya] DspyFindIntsInParamList OriginalSize\n" );
//  if(PkDspyErrorNone!=DspyFindIntsInParamList("OriginalSize",&rc,originalSize,paramCount,parameters)) 
  {
		originalSize[0]= width;
		originalSize[1]= height;
	}
  
  printf( "[d_liqmaya] OriginalSize width = %d height = %d\n", originalSize[0], originalSize[1] );

	printf( "[d_liqmaya] DspyFindIntInParamList mayaDisplayPort\n" );
  if(PkDspyErrorNone!=DspyFindIntInParamList("mayaDisplayPort",&port,paramCount,parameters)) 
		port = 6667;
	
  printf( "[d_liqmaya] mayaDisplayPort = %d\n", port );
  
	printf( "[d_liqmaya] DspyFindIntInParamList looking for timeout (%d)\n", timeout );
  if(PkDspyErrorNone!=DspyFindIntInParamList("timeout",&timeout,paramCount,parameters)) 
    timeout = 30;
	
	printf( "[d_liqmaya] timeout = %d\n", timeout );
  
  printf( "[d_liqmaya] origin x = %d y = %d\n", origin[0], origin[1] );
  printf( "[d_liqmaya] OriginalSize width = %d height = %d\n", originalSize[0], originalSize[1] );
  
  imageInfo *imgSpecs = new imageInfo;
	imgSpecs->channels  = formatCount;
	imgSpecs->width     = width;
	imgSpecs->height    = height;
	imgSpecs->xo = origin[0];
	imgSpecs->yo = origin[1];
	imgSpecs->wo = originalSize[0];
	imgSpecs->ho = originalSize[1];
  
  *pvImage = imgSpecs;
 
	socketId = openSocket(hostname, port);
  printf("[d_liqmaya] openSocket = %d host = %s port = %d\n", socketId, hostname, port);
	if(socketId == -1)
	{
		#ifdef _WIN32
			WSACleanup();
		#endif
		delete imgSpecs;
		return PkDspyErrorNoResource;
	}

	if(!waitSocket(socketId,timeout,false))	
	{
		#ifdef _WIN32
			WSACleanup();
		#endif
		cerr<<"[d_liqmaya] Error: timeout"<<endl;
		delete imgSpecs;
		return PkDspyErrorNoResource;
	}
	status = sendSockData(socketId,(char*)imgSpecs,sizeof(imageInfo));

	if(status == false){
		#ifdef _WIN32
			WSACleanup();
		#endif
		perror("[d_liqmaya] Error: write(socketId,wh,2*sizeof(int))");
		delete imgSpecs;
		return PkDspyErrorNoResource;
	}
	return PkDspyErrorNone;
}


PtDspyError
DspyImageQuery(PtDspyImageHandle pvImage,
               PtDspyQueryType querytype,
               int datalen,
               void *data) {
	if ((datalen == 0) || (NULL == data))
		return PkDspyErrorBadParams;


	switch (querytype) 
  {
    case PkOverwriteQuery: 
    {
			PtDspyOverwriteInfo overwriteInfo;

			if (datalen > (int)sizeof(overwriteInfo))
				datalen = sizeof(overwriteInfo);
			overwriteInfo.overwrite = 1;
			overwriteInfo.interactive = 0;
			memcpy(data, &overwriteInfo, datalen);
			break;
		}

    case PkSizeQuery: 
    {
			PtDspySizeInfo sizeInfo;

			if (datalen > (int)sizeof(sizeInfo))
				datalen = sizeof(sizeInfo);

			memcpy(data, &sizeInfo, datalen);
			break;
		}



	default :
		return PkDspyErrorUnsupported;
	}

	return PkDspyErrorNone;
}



PtDspyError DspyImageData(PtDspyImageHandle pvImage,
                          int xmin,
                          int xmax_plusone,
                          int ymin,
                          int ymax_plusone,
                          int entrysize,
                          const unsigned char *data) {

	// if(recoverFlag && (ymin<image->recoverLine))return PkDspyErrorNone;

	imageInfo *spec = (imageInfo*)pvImage;
	PtDspyError status = sendData(socketId,xmin,xmax_plusone,ymin,ymax_plusone,entrysize,spec->channels,(BUCKETDATATYPE*)data);
	if(status != PkDspyErrorNone)
		DspyImageClose(pvImage);
	return status;

}


PtDspyError DspyImageClose(PtDspyImageHandle pvImage) {
	bucket::bucketInfo binfo;
#ifdef _WIN32
	memset(&binfo,0,sizeof(bucket::bucketInfo));
#else
	bzero(&binfo,sizeof(bucket::bucketInfo));
#endif
  sendSockData(socketId, (char*) &binfo,sizeof(bucket::bucketInfo));
	closesocket(socketId);
#ifdef _WIN32
	WSACleanup();
#endif

	return PkDspyErrorNone;
}


PtDspyError sendData(const int socket,
			 const int xmin,
			 const int xmax_plusone,
			 const int ymin,
			 const int ymax_plusone,
			 const int entrysize,
			 const int numChannels,
			 const BUCKETDATATYPE *data) {
	int status =0;
	int size = (xmax_plusone-xmin)*(ymax_plusone-ymin)*numChannels*sizeof(BUCKETDATATYPE);
	bucket::bucketInfo binfo;
	binfo.left      = xmin;
	binfo.right     = xmax_plusone;
	binfo.bottom    = ymin;
	binfo.top       = ymax_plusone;
	binfo.channels  = numChannels;

	if(!waitSocket(socket,timeout,false))
  {
		cerr<<"[d_liqmaya] Error: timeout (" << timeout << ") reached, data cannot be sent"<<endl;
		return PkDspyErrorUndefined;
	}

	status = sendSockData(socket, (char*)&binfo,sizeof(bucket::bucketInfo));
	if(status == false)
  {
		perror("[d_liqmaya] Error: write(socket,bucketInfo)");
		return PkDspyErrorNoResource;
	}
	
  if(!waitSocket(socket,timeout,false))
  {
		cerr<<"[d_liqmaya] Error: timeout (" << timeout << ") reached, data cannot be sent"<<endl;
		return PkDspyErrorUndefined;
	}
	status = sendSockData(socket, (char*)data,size);
	if(status == false)
  {
		perror("[d_liqmaya] Error: write(socket,data)");
		return PkDspyErrorNoResource;
	}
	return PkDspyErrorNone;

}


int openSocket(const char *host, const int port)
{
	struct hostent *hostPtr = NULL;
	struct sockaddr_in serverName;
	int clientSocket = socket(PF_INET, SOCK_STREAM,IPPROTO_TCP);
	if (-1 == clientSocket) 
  {
		perror("[d_liqmaya] Error: socket()");
		return -1;
	}

	hostPtr = gethostbyname(host);
	if (NULL == hostPtr) 
  {
		hostPtr = gethostbyaddr(host,strlen(host), AF_INET);
		if (NULL == hostPtr) 
    {
			perror("[d_liqmaya] Error: resolving server address");
			return -1;
		}
	}

	serverName.sin_family = AF_INET;
	serverName.sin_port = htons(port);
	memcpy(&serverName.sin_addr,hostPtr->h_addr,hostPtr->h_length);
	errno = 0;
  int status;
  int counter = 10;
  
  while ( counter-- )
  {  
    status = connect(clientSocket,(struct sockaddr*) &serverName,sizeof(serverName));
    if ( status != -1 ) 
      break; 
#ifndef _WIN32
		sleep(2);
#else
  	Sleep(1000);
#endif 
  } 

  if (-1 == status)
  {
    perror("[d_liqmaya] Error: connect()");
    return -1;
  }
  int val = 1;
  int valLen = sizeof(int);
  status = setsockopt(clientSocket,IPPROTO_TCP,TCP_NODELAY,(const char *) &val,sizeof(int));
  
  #ifdef SO_NOSIGPIPE
    val = 1;
    setsockopt(clientSocket,SOL_SOCKET,SO_NOSIGPIPE,(const char *) &val,sizeof(int));
  #endif

	return clientSocket;
}

int sendSockData(int s,char * data,int n){
	int i,j;

	j	= n;
	i	= send(s,data,j,0);
  // printf("[d_liqmaya] s = %d n = %d i = %d\n", s, n, i);
	if (i <= 0) {
		perror("[d_liqmaya] Connection broken");
		return false;
	}

	// If we could not send the entire data, send the rest
	while(i < j) 
  {
		data	+=	i;
		j		-=	i;
		i		= send(s,data,j,0);
		if (i <= 0) 
    {
			perror("[d_liqmaya] Connection broken (sending rest)");
			return false;
		}
	}
	return true;
}
