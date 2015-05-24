#include "Block.h"
using namespace Thought;

#define PACK_OP_ABC(x, a, b, c) (x & 0xFF << 24) + (a & 0xFF << 16) + (b & 0xFF << 8) + (c & 0xFF)

Block::~Block() {
	// Clear references to constants
	for(auto constant : constants) {
		constant->release();
	}
}

std::uint32_t Block::operator[](int index) {
	return bytecode[index];
}

void Block::push_inst_abc(OpCode op, int a, int b, int c) {
	bytecode.add(PACK_OP_ABC(op, a, b, c));
}

void Block::push_inst_ad(OpCode op, int a, int d) {
	push_inst_abc(op, a, d & 0xFF00, d & 0xFF);
}

int Block::add_constant(Value* val) {
	val->retain();
	constants.add(val);
	return constants.size() - 1;
}

Value* Block::get_constant(int loc) {
	return ((loc < 0 ? constants.size() + loc : loc) < constants.size() ? constants[loc] : nullptr);
}
