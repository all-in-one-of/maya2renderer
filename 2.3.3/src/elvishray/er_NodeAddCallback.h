#ifndef _ER_NODE_ADD_CALLBACK_H_
#define _ER_NODE_ADD_CALLBACK_H_

#include <common/prerequest_maya.h>

namespace elvishray
{
	class NodeAddCallback
	{
	public:
		NodeAddCallback();
		virtual ~NodeAddCallback();

		static MCallbackId	sId;

		static void registCallback();
		static void removeCallback();

	protected:
		static void NodeAddCallbackFunc( MObject& node, void* clientData );

	private:
		NodeAddCallback(const NodeAddCallback&);
		NodeAddCallback& operator=(const NodeAddCallback&);
	};

}//namespace elvishray

#endif//_ER_NODE_ADD_CALLBACK_H_