#include "VM.h"
#include <cstring>
using namespace Thought;

// Macros for extracting arguments from bytecode
#define DECODE_INST(x) (x >> 24) & 0xff
#define DECODE_A(x) (x >> 16) & 0xff
#define DECODE_B(x) (x >> 8) & 0xff
#define DECODE_C(x) (x & 0xff)
#define DECODE_D(x) (x & 0xffff)

VM::VM() : first(nullptr) {
}

VM::~VM() {
	// If pure GC, we simply would GC here, but we aren't
	// Actually, forget the user. They shouldn't keep our values after we die.
	// To forcefully collect all objects, we simply clear, then sweep.
	clear(); // Remove all references we have to any Value
	// sweep();
}

Value* VM::createValue(Value::Type type) {
	Value* newVal = new Value;
	newVal->mark = 0;
	newVal->type = type;
	newVal->prototype = nullptr;

	newVal->next = first;

	first = newVal;
	return newVal;
}

Value* VM::createDouble(double val) {
	auto newval = createValue(Value::DOUBLE);
	newval->v_double = val;

	return newval;
}

Value* VM::createBool(bool val) {
	auto newval = createValue(Value::BOOL);
	newval->v_bool = val;

	return newval;
}

Value* VM::createString(const char* str) {
	return createString(str, std::strlen(str));
}

Value* VM::createString(const char* str, int size) {
	auto newval = createValue(Value::STRING);

	char* vstr = new char[size + 1];
	std::memcpy(vstr, str, size);
	vstr[size] = '\0';

	newval->v_string = vstr;

	return newval;
}

void VM::markAll() {
	// Mark everything reachable

	// Marking the stack
	for(int i = 0; i < stack.size(); i++) {
		mark(stack[i]);
	}

	// Marking callframe constants
	for(CallFrame frame : frames) {
		for(auto constant : frame.code->constants) {
			mark(constant);
		}
	}
}

void VM::mark(Value* val) {
	// Mark all proper things for the value
	if(val->mark) return;

	val->mark = 1;
}

void VM::sweep() {
	Value** live = &first;
	while(*live) {
		if(!(*live)->mark) {
			Value* unreach = *live;

			*live = unreach->next;
			// Destroy the unreachable
			unreach->destroy();
		} else {
			(*live)->mark = 0;
			live = &(*live)->next;
		}
	}
}

void VM::callBlock(Ref<Block> block, int args) {
	frames.push(CallFrame {0, stack.size() - args, block});
}

void VM::popFrame() {
	frames.pop();
}

void VM::push(Value* val, int index) {
	val->retain(); // So we are a prim example of reference counting
	stack.insert(val, index);
}

Value* VM::peek(int idx) {
	return stack[idx];
}

Value* VM::pop(int idx) {
	Value* val = stack[idx];
	stack.remove(idx);
	val->release(); // We don;t want it any more, so we let it go
	return val;
}

void VM::clear() {
	while(!stack.isEmpty()) {
		pop();
	}
}

void VM::dump() {
	// Dump callframe info
	// Dump value stack
	for(int i = 0; i < stack.size(); i++) {
		std::cout << "Location " << i << ": ";
		#define VCASE(x) case Value::x: std::cout << #x << std::endl; break;
		switch(stack[i]->type) {
			VCASE(DOUBLE)
			VCASE(BOOL)
			VCASE(STRING)
		}
		#undef VCASE
	}
}