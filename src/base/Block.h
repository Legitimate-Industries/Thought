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
		OP_SS = 0x1, // Say Something (ad)
	};
};

class Thought::Block {
	Array<std::uint32_t> bytecode;
public:
	void push_inst_abc(OpCode, int a = 0x0, int b = 0x0, int c = 0x0);
	void push_inst_ad(OpCode, int a = 0x0, int d = 0x0);

	std::uint32_t operator[](int index);
};

#endif // THOUGHT_BLOCK_H