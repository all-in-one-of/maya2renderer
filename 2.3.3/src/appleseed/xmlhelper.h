#pragma once
#include "../common/prerequest_std.h"
#include "prerequest_xml.h"

#define X(str) XStr(str).unicodeForm()

namespace appleseed
{
	class XStr
	{
	public :
		XStr(const char* const toTranscode);
		~XStr();
		const XMLCh* unicodeForm() const;

	private :
		XMLCh*   fUnicodeForm;
	};

	//
	XERCES_CPP_NAMESPACE::DOMDocument* XmlInitialize();
	void XmlTerminate(XERCES_CPP_NAMESPACE::DOMDocument* doc);
	void XmlOutput(XERCES_CPP_NAMESPACE::DOMDocument* pmyDOMDocument, const std::string &filePath); 

}//namespace appleseed