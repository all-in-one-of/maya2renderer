#ifndef _ER_NODE_ID_
#define _ER_NODE_ID_
 
namespace elvishray
{
	enum TypeID
	{
		TI_START		= 0xFFFFF360,

		TI_GLOBAL_NODE	= 0xFFFFF360,

		TI_NUM,
	};
	//
	extern const char *TN_GLOBAL_NODE;
	//
}//namespace elvishray
#endif//_ER_NODE_ID_