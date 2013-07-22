#ifndef _DAG_MGR_H_
#define _DAG_MGR_H_

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
#include "liqtypes.h"

namespace liquidmaya
{
	class DagMgr
	{
	public:
		DagMgr();
		~DagMgr();

		//get the upstream of 'node' in maya shader graph
		//'result' does not contain 'node'
		void BreadthFirstTraversal(const MString &startNode, MStringArray& result);

	protected:
		bool isVisited(const std::string &node);
		void visit(const std::string &node);
		void pushInputPlugs(const MString &node);
		void begin(const MString &node);
		void end(MStringArray& result);

		std::deque<std::string> m_UpStreamNodes;
		std::queue<std::string> m_todo;
		std::vector<std::string> m_result;
	};
}//namespace liquidmaya

#endif//_DAG_MGR_H_