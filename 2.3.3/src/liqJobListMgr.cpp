#include "liqJobListMgr.h"

liqJobListMgr* liqJobListMgr::m_instance = NULL;

liqJobListMgr::liqJobListMgr()
{

}

liqJobListMgr::~liqJobListMgr()
{

}

liqJobListMgr* liqJobListMgr::getInstancePtr()
{
	if( m_instance == NULL){
		m_instance = new liqJobListMgr();
	}
	return m_instance;
}
