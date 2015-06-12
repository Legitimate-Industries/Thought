#ifndef THOUGHT_COMPOUNDNAMENODE_H
#define THOUGHT_COMPOUNDNAMENODE_H

#include "Node.h"
#include <vector>

namespace Thought {
	class CompoundNameNode;
};

class Thought::CompoundNameNode : public Node {
	NodePtr left_name;
	NodePtr right_name;
public:
	CompoundNameNode(NodePtr l, NodePtr r) : left_name(l), right_name(r) {}
	void print(std::ostream& o) const {
		o << "[";
		left_name->print(o);
		o << ".";
		right_name->print(o);
		o << "]";
	}
};

#endif // THOUGHT_COMPOUNDNAMENODE_H