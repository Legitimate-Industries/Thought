#ifndef THOUGHT_VALUE_H
#define THOUGHT_VALUE_H

#include <cstdint>
#include <base/Array.h>
#include <string>

#include "Table.h"

namespace Thought {
	struct Value;
	struct ValueHandle;
	struct VM;
};

class Thought::Value {
	std::size_t refs;
	// Make values only constructable by a VM Object
	friend class VM;
	Value(VM* vm) : refs(0), home(vm) {}

	VM* home;

	void destroy(bool = false);
public:
	enum Type {
		DESTROYED, // NEVER SET VALUE TO THIS TYPE MANUALLY

		DOUBLE,
		BOOL,
		STRING,
		TABLE,
		BLOCK, // Code-side function
		NATIVE, // C++-side function
		ARRAY
	} type;

	union {
		double v_double;
		bool v_bool;
		char* v_string; // Always heap-allocated
		Table* v_table;
	};

	// Should suffice for now (make it an array for multiple prototypes)
	Value* prototype;

	// Lovely prototype ref-counting proper interface to set prototypes
	void set_prototype(Value* v) {
		if(v != nullptr)
			v->retain();
		if(prototype != nullptr)
			prototype->release();
		prototype = v;
	}

	// Used for GC
	int mark;

	Value* next;

	// Gets the number of references to this object
	std::size_t count() { return refs; }

	void retain() {
		refs++;
	}

	void release() {
		refs--;
		if(refs <= 0)
			destroy(true);
	}

	Value(const Value&) = delete;
	Value& operator=(const Value&) = delete;
	// Make values uncopyable until we separate the actual objects (double, string, table)
	// from Value, so that 2 values can safely refer to the same object
	// Actually, still keep values uncopyable. It's nice.
};

// This is the class users manipulate, pointing to a Value in memory
class Thought::ValueHandle {
	Value* val;
	// Right now, it is exclusively created by the VM. Maybe later on, I'll free it up.
	friend class VM;
	ValueHandle(Value* v) : val(v) { if(!isNull()) val->retain(); }
public:
	ValueHandle() : val(nullptr) {}
	ValueHandle(const ValueHandle& o) { val = o.val; if(!isNull()) val->retain(); }
	ValueHandle& operator=(const ValueHandle& o) { 
		if(!isNull()) val->release();
		val = o.val; 
		if(!isNull()) val->retain(); 
		return *this;
	}
	~ValueHandle() { if(!isNull()) val->release(); }

	bool isNull() const { return val == nullptr; }

	// Accessing interface
	double as_double() const { 
		if(isNull() || val->type != Value::DOUBLE)
			return 0.0;
		return val->v_double;
	}

	bool as_bool() const { 
		if(isNull() || val->type != Value::BOOL)
			return false;
		return val->v_bool; 
	}

	std::string as_string() const { 
		if(isNull() || val->type != Value::STRING)
			return "";
		return val->v_string;
	}

	char* as_string_array() const { 
		if(isNull() || val->type != Value::STRING)
			return nullptr;
		return val->v_string;
	}

	Table* as_table() const { 
		if(isNull() || val->type != Value::TABLE)
			return nullptr;
		return val->v_table;
	}

	Table* force_table() const { 
		if(isNull()) return nullptr;
		if(val->type == Value::TABLE)
			return val->v_table;
		else {
			if(val->prototype != nullptr)
				return ValueHandle(val->prototype).force_table();
			return nullptr;
		}
	}

	void set_prototype(Value* value) {
		if(val->prototype != nullptr)
			val->prototype->release();
		if(value != nullptr)
			value->retain();
		val->prototype = value;
	}
};

#endif // THOUGHT_VALUE_H