
#include "liqRibHTMgr.h"

liqRibHTMgr* liqRibHTMgr::m_instance = NULL;

liqRibHTMgr::liqRibHTMgr()
{

}
liqRibHTMgr::~liqRibHTMgr()
{

}
liqRibHTMgr* liqRibHTMgr::getInstancePtr()
{
	if( m_instance == NULL )
	{
		m_instance = new liqRibHTMgr();
	}
	return m_instance;
}
void liqRibHTMgr::createHTable()
{
	 htable = boost::shared_ptr< liqRibHT >( new liqRibHT() );
}
void liqRibHTMgr::destroyHTable()
{
	//todo ...
}
boost::shared_ptr< liqRibHT >& liqRibHTMgr::getHTable()
{
	return htable;
}