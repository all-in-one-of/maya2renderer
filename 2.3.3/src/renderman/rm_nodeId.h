#ifndef _RM_NODE_ID_
#define _RM_NODE_ID_
 
namespace renderman
{
	enum TypeID
	{
		TI_START		= 0xFFFFF460,

		TI_GLOBAL_NODE	= 0xFFFFF460,

		TI_NUM,
	};
	//
	extern const char *TN_GLOBAL_NODE;
	//
}//namespace renderman
#endif//_RM_NODE_ID_