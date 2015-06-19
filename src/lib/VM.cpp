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

ValueHandle VM::createValue(Value::Type type) {
	Value* newVal = new Value(this);
	newVal->mark = 0;
	newVal->type = type;
	newVal->prototype = nullptr;

	newVal->next = first;

	first = newVal;
	return ValueHandle(newVal);
}

ValueHandle VM::createDouble(double val) {
	auto newval = createValue(Value::DOUBLE);
	newval.val->v_double = val;

	return newval;
}

ValueHandle VM::createBool(bool val) {
	auto newval = createValue(Value::BOOL);
	newval.val->v_bool = val;

	return newval;
}

ValueHandle VM::createString(const char* str) {
	return createString(str, std::strlen(str));
}

ValueHandle VM::createString(const char* str, int size) {
	auto newval = createValue(Value::STRING);

	char* vstr = new char[size + 1];
	std::memcpy(vstr, str, size);
	vstr[size] = '\0';

	newval.val->v_string = vstr;

	return newval;
}

ValueHandle VM::createTable(Value* proto) {
	auto newval = createValue(Value::TABLE);

	Table* table = new Table(newval.val);
	newval.val->v_table = table;

	return newval;
}

void VM::markAll() {
	// Mark everything reachable

	// Marking the stack
	for(int i = 0; i < stack.size(); i++) {
		mark(stack[i].val);
	}

	// Marking callframe constants
	for(CallFrame frame : frames) {
		for(auto constant : frame.code->constants) {
			mark(constant.val);
		}
		mark(frame.self.val);
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

void VM::isolate(Value* val) {
	// Used for when a value is destroyed by ref counting
	// Removes a value from the GC linked list
	Value** list = &first;
	while(*list) {
		if(*list == val) {
			*list = (*list)->next;
			return;
		} 
		list = &(*list)->next;
	}
	// The Value never was on the list if it reaches here.
}

void VM::callBlock(Ref<Block> block, unsigned int args, ValueHandle receiver) {
	frames.push(CallFrame(0, stack.size() - args, block, receiver));

	// Question: Expose run() separately?
	run();
}

void VM::popFrame() {
	frames.pop();
}

void VM::push(ValueHandle val, int index) {
	stack.insert(val, index);
}

ValueHandle VM::peek(int idx) {
	return stack[idx];
}

ValueHandle VM::pop(int idx) {
	ValueHandle val = stack[idx];
	stack.remove(idx);
	return val;
}

void VM::clear() {
	while(!stack.isEmpty()) {
		pop();
	}
}

void VM::dump() {
	std::cout << "-- THINK (THOUGHT VM) DUMP --" << std::endl;
	// Dump callframe info
	// Dump value stack
	for(int i = 0; i < stack.size(); i++) {
		std::cout << "Location " << i << ": ";
		#define VCASE(x) case Value::x: std::cout << #x << std::endl; break;
		switch(stack[i].val->type) {
			VCASE(DOUBLE)
			VCASE(BOOL)
			VCASE(STRING)
		}
		#undef VCASE
	}

	std::cout << "-- END THINK DUMP --" << std::endl;
}

void VM::run() {
	while(!isDone()) {
		CallFrame& frame = top();
		if(frame.ip >= frame.code->size())
			// Overflow, method failed to end in RETURN or END
			throw "MethodOverflowError: method failed to end";
		std::uint32_t raw_inst = (*frame.code)[frame.ip];
		std::uint8_t inst = DECODE_INST(raw_inst);
		std::uint8_t a = DECODE_A(raw_inst);
		std::uint8_t b = DECODE_B(raw_inst);
		std::uint8_t c = DECODE_C(raw_inst);
		std::uint16_t d = DECODE_D(raw_inst);
		switch(inst) {
			case OP_PUSHC: {
				ValueHandle cons = frame.code->get_constant(d);
				store(frame, a, cons);
				break;
			}
			case OP_EXIT:
			case OP_RETURN: {
				popFrame();
				break;
			}
		};
		frame.ip++;
	}
}