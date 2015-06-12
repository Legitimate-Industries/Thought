#ifndef THOUGHT_CALLNODE_H
#define THOUGHT_CALLNODE_H

#include "Node.h"
#include <vector>

namespace Thought {
	class CallNode;
};

class Thought::CallNode : public Node {
	NodePtr object;
	std::vector<NodePtr> args;
public:
	CallNode(NodePtr o, std::vector<NodePtr> a) : object(o), args(a) {}
	void print(std::ostream& o) const {
		o << "[";
		object->print(o);
		o << "(";
		for(int i = 0; i < args.size(); i++) {
			args[i]->print(o);
			if(i < args.size() - 1) {
				o << ", ";
			}
		}
		o << ")";
		o << "]";
	}
};

#endif // THOUGHT_CALLNODE_H