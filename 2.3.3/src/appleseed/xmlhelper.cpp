#include "xmlhelper.h"
#include <liqlog.h>

XERCES_CPP_NAMESPACE_USE

namespace appleseed
{
	//
	XStr::XStr(const char* const toTranscode)
	{
		// Call the private transcoding method
		fUnicodeForm = XMLString::transcode(toTranscode);
	}
	//
	XStr::~XStr()
	{
		XMLString::release(&fUnicodeForm);
	}
	//
	const XMLCh* XStr::unicodeForm() const
	{
		return fUnicodeForm;
	}
	//------------------------------------------------------
	XERCES_CPP_NAMESPACE::DOMDocument* XmlInitialize()
	{
		try
		{
			XMLPlatformUtils::Initialize();
		}
		catch(const XMLException& e)
		{
			liquidMessage2(messageError, "Error Message: %s", e.getMessage());
			return NULL;
		}

		//
		DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(X("Core"));
		if (impl != NULL)
		{
			try
			{
				XERCES_CPP_NAMESPACE::DOMDocument* doc = impl->createDocument(
					0,                    // root element namespace URI.
					X("project"),         // root element name for appleseed
					0);                   // document type object (DTD).
				return doc;
			}
			catch (const OutOfMemoryException&)
			{
				liquidMessage2(messageError, "XmlInitialize() OutOfMemoryException");
			}
			catch (const DOMException& e)
			{
				liquidMessage2(messageError, "DOMException code is: %d ", e.code);
			}
			catch(const XMLException& e)
			{
				liquidMessage2(messageError, "Error Message: %s", e.getMessage());
			}
			catch (...)
			{
				liquidMessage2(messageError, "An error occurred creating the document");
			}
		}else
		{
			liquidMessage2(messageError,"Requested implementation is not supported");
		}
		return NULL;

	}
	//
	void XmlTerminate(XERCES_CPP_NAMESPACE::DOMDocument* doc)
	{
		assert(doc!=NULL);

		doc->release();

		XMLPlatformUtils::Terminate();
	}
	//
	void XmlOutput(XERCES_CPP_NAMESPACE::DOMDocument* pmyDOMDocument, const std::string &filePath) 
	{ 
		//Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save. 
		DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(L"LS"); 

		// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document. 
		DOMLSSerializer *serializer = ((DOMImplementationLS*)implementation)->createLSSerializer(); 

		// Make the output more human readable by inserting line feeds. 
		if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true)) 
			serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true); 

		// The end-of-line sequence of characters to be used in the XML being written out.  
		serializer->setNewLine(XMLString::transcode("\r\n"));  

		// Convert the path into Xerces compatible XMLCh*. 
		XMLCh *tempFilePath = XMLString::transcode(filePath.c_str()); 

		// Specify the target for the XML output. 
		XMLFormatTarget *formatTarget = new LocalFileFormatTarget(tempFilePath); 

		// Create a new empty output destination object. 
		DOMLSOutput *output = ((DOMImplementationLS*)implementation)->createLSOutput(); 

		// Set the stream to our target. 
		output->setByteStream(formatTarget); 

		// Write the serialized output to the destination. 
		serializer->write(pmyDOMDocument, output); 

		// Cleanup. 
		serializer->release(); 
		XMLString::release(&tempFilePath); 
		delete formatTarget; 
		output->release(); 
	} 
}