#include "Block.h"
using namespace Thought;

#define PACK_OP_ABC(x, a, b, c) (x & 0xFF << 24) + (a & 0xFF << 16) + (b & 0xFF << 8) + (c & 0xFF)
#define PACK_OP_AD(x, a, d) (x & 0xFF << 24) + (a & 0xFF << 16) + (d & 0xFFFF)


void Block::push_instruction(OpCode op, int a, int b, int c) {
	bytecode.add(PACK_OP_ABC(op, a, b, c));
}

void Block::push_instruction(OpCode op, int a, int d) {
	bytecode.add(PACK_OP_AD(op, a, d));
}