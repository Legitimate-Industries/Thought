#include "Block.h"
using namespace Thought;

#define PACK_OP_ABC(x, a, b, c) (x & 0xFF << 24) + (a & 0xFF << 16) + (b & 0xFF << 8) + (c & 0xFF)

std::uint32_t Block::operator[](int index) {
	return bytecode[index];
}

void Block::push_inst_abc(OpCode op, int a, int b, int c) {
	bytecode.add(PACK_OP_ABC(op, a, b, c));
}

void Block::push_inst_ad(OpCode op, int a, int d) {
	push_inst_abc(op, a, d & 0xFF00, d & 0xFF);
}