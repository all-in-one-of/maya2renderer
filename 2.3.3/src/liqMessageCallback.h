#ifndef _LIQ_MESSAGE_CALLBACK_H_
#define _LIQ_MESSAGE_CALLBACK_H_

#include <common/prerequest_maya.h>

namespace liquid
{
	class liqMessageCallback
	{
	public:
		liqMessageCallback();
		virtual ~liqMessageCallback();

		static MCallbackId	sNodeAddCallbackId;

		static void registCallback();
		static void removeCallback();

	protected:
		static void NodeAddCallbackFunc( MObject& node, void* clientData );

	private:
		liqMessageCallback(const liqMessageCallback&);
		liqMessageCallback& operator=(const liqMessageCallback&);
	};

}//namespace liquid

#endif//_LIQ_MESSAGE_CALLBACK_H_