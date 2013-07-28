#ifndef liqRibHT_MGR_H
#define liqRibHT_MGR_H

#include <common/prerequest_std.h>
#include <liqtypes.h>
#include <liqRibNode.h>
#include <liqRefactoring.h>
#include <liqRibHT.h>

class LIQUID_EXPORT liqRibHTMgr
{
public:
		~liqRibHTMgr();
		static liqRibHTMgr* getInstancePtr();

		void createHTable();
		void destroyHTable();
		boost::shared_ptr< liqRibHT >& getHTable();

protected:
		liqRibHTMgr();

		static liqRibHTMgr* m_instance;

		boost::shared_ptr< liqRibHT > htable;
};

#endif//liqRibHT_MGR_H