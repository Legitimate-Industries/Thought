#include "Compiler.h"
using namespace Thought;

#include "nodes/AssignmentNode.h"
#include "nodes/NumberNode.h"
#include "nodes/NameNode.h"
#include <stdexcept>

void Compiler::compileAssignment(NodePtr nd) {
	AssignmentNode* node = dynamic_cast<AssignmentNode*>(nd.get());

	// A some point make -> syntax special ("global" syntax)
	NameNode* name = nullptr;
	if(name = dynamic_cast<NameNode*>(node->getName().get())) {
		int loc = reserveRegister(name->getValue());
		NumberNode* num = nullptr;
		if(num = dynamic_cast<NumberNode*>(node->getValue().get())) {
			file->push_inst_ad(OP_PUSHF, loc, static_cast<int>(num->getValue()));
			return;
		} else {
			// It's not a number...
		}
	} else {
		// It's a compound name, which we will try to support. Later.
	}

	throw std::runtime_error("Still in progress...");
}

int Compiler::reserveRegister(std::string n) {
	return regs.addString(n);
}