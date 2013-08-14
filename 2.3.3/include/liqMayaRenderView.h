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
** Liquid server communicating with the liqmaya display driver
** ______________________________________________________________________
*/


#ifndef liqMayaRenderView_H
#define liqMayaRenderView_H

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>

#include "liqBucket.h"


class liqMayaRenderCmd : public MPxCommand
{

public:
				liqMayaRenderCmd();
	virtual		~liqMayaRenderCmd();

	MStatus		doIt( const MArgList& );
	MStatus		redoIt();
	MStatus		undoIt();
	bool		isUndoable() const;
	static MSyntax newSyntax();
	static		void* creator();
	static MString getName(){ return "liquidRenderView";}

	static std::deque<std::string> m_lastBucketFiles;

private:
	MStatus renderBucket(const bucket* b, const imageInfo &info);
	MStatus getBucket(const int socket,const unsigned int numChannels,bucket* b,bool &theEnd);
	MStatus writeBuckets(const char* file, const std::vector<bucket*> &buckets,const imageInfo &info) const;
	MStatus readBuckets(const char* file,std::vector<bucket*> &buckets, imageInfo &info) const;

	int createSocket(const char *hostname,const unsigned int port);

	MString m_camera;
	MString m_bucketFile;
	bool m_bRenderFromFile;
	bool m_bLocalhost;
	bool m_bGetRenderRegion;
	bool m_bDoRegionRender;
	unsigned int m_port;
	double m_quantize[4];
	unsigned int m_timeout;


private:
	liqMayaRenderCmd(const liqMayaRenderCmd&);
	liqMayaRenderCmd& operator=(const liqMayaRenderCmd&);
};



#endif// liqMayaRenderView_H

