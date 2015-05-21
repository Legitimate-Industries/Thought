#ifndef THOUGHTC_NODE_H
#define THOUGHTC_NODE_H

#include <iostream>
#include <memory>
#include <string>

enum class NodeType
{
};

#define NTOS(n) case NodeType::n: return #n;
static std::string nt_to_string(NodeType n) {
	switch(n) {
	};
}
#undef NTOS

class Node
{
public:
	virtual ~Node() {}
	virtual void print(std::ostream&) const = 0;
	virtual NodeType getType() = 0;
};

using NodePtr = std::shared_ptr<Node>;

#endif // THOUGHTC_NODE_H