#ifndef THOUGHT_NAMENODE_H
#define THOUGHT_NAMENODE_H

#include "Node.h"
#include <string>

namespace Thought {
	class NameNode;
};

class Thought::NameNode : public Node {
	std::string value;
public:
	NameNode(std::string v) : value(v) {}
	void print(std::ostream& o) const {
		o << value;
	}
};

#endif // THOUGHT_NAMENODE_H