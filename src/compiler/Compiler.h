#ifndef THOUGHT_COMPILER_H
#define THOUGHT_COMPILER_H

namespace Thought {
	class Compiler;
};

#include <map>
#include <base/Block.h>
#include <base/Ref.h>
#include <base/ThoughtStringTable.h>
#include "nodes/Node.h"

// Takes a NodePtr and produces a Block
class Thought::Compiler {
	//std::map<std::string, int> regs;
	StringTable regs;
	Ref<Block> file;
public:
	Compiler() : file(new Block) {} 
	void compileAssignment(NodePtr);
	int reserveRegister(std::string);

	void end() { file->push_inst_ad(OP_RETURN); }

	Ref<Block> getBlock() { return file; }
};

#endif // THOUGHT_COMPILER_H