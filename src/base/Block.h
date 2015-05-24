#ifndef THOUGHT_BLOCK_H
#define THOUGHT_BLOCK_H

#include "Array.h"
#include <lib/Value.h>
// #include <lib/ValueRef.h>
#include <cstdint>
#include <string>
#include <map>

// A chunk of code
namespace Thought {
	class Block;
	// All instructions that are possible, followed by a little documentation
	// Codes go from 0 to 255
	enum OpCode : std::uint8_t {
		OP_NOP = 0x0,
		OP_PUSHL = 0x1, // Pushs a block local to the stack a-register number b-constant number 
	};
};

class Thought::Block {
	friend class VM;

	Array<std::uint32_t> bytecode;
	Array<Value*> constants;
public:
	void push_inst_abc(OpCode, int a = 0x0, int b = 0x0, int c = 0x0);
	void push_inst_ad(OpCode, int a = 0x0, int d = 0x0);

	int add_constant(Value*);
	Value* get_constant(int);
	int constants_count() { return constants.size(); }

	std::uint32_t operator[](int index);

	~Block();
};

#endif // THOUGHT_BLOCK_H