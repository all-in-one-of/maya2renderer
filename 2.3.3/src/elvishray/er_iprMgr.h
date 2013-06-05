#ifndef _ER_IPRMGR_H_
#define _ER_IPRMGR_H_

#include <common/prerequest_maya.h>

namespace elvishray
{	
	class Renderer;
	//
	class IprMgr
	{
	public:
		IprMgr();
		virtual ~IprMgr();

		void onAttributeChanged(MNodeMessage::AttributeMessage msg, 
			MPlug & plug, MPlug & otherPlug, void* userData);
	protected:
		void updateCamera(const MPlug &plug);
		void updateShader(const MPlug &plug);

	private:
		IprMgr(const IprMgr&);
		IprMgr& operator=(const IprMgr&);

	};
}

#endif//_ER_IPRMGR_H_