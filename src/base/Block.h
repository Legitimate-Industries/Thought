#ifndef THOUGHT_BLOCK_H
#define THOUGHT_BLOCK_H

#include "Array.h"
#include <cstdint>

// A chunk of code
namespace Thought {
	class Block;
	// All instructions that are possible, followed by a little documentation
	// Codes go from 0 to 255
	enum OpCode : std::uint8_t {
		OP_NOP = 0x0,
	};
};

class Thought::Block {
	Array<std::uint32_t> bytecode;
public:
	void push_instruction(OpCode, int = 0x0, int = 0x0, int = 0x0);
	void push_instruction(OpCode, int = 0x0, int = 0x0);
};

#endif // THOUGHT_BLOCK_H