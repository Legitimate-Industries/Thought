#ifndef THOUGHT_VALUE_H
#define THOUGHT_VALUE_H

#include <cstdint>
#include <base/Array.h>
#include <string>

#include "Table.h"

namespace Thought {
	struct Value;
	struct ValueHandle;
};

class Thought::Value {
	std::size_t refs;
	// Make values only constructable by a VM Object
	friend class VM;
	Value() : refs(0) {}

	friend class ValueBaseRef; // Allows for destruction of a value
	void destroy() {
		if(type != DESTROYED) {
			switch(type) {
				case STRING:
					delete[] v_string;
					break;
				case TABLE:
					delete v_table;
					break;
			};
			// This is THE STUPIDEST trick in the book, and will not 
			// work if immediately overwritten by accident, but will work for now.
			type = DESTROYED; 
			delete this;
		}
	}
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
			destroy();
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

	bool isNull() { return val == nullptr; }

	// Accessing interface
	double as_double() { 
		if(val == nullptr || val->type != Value::DOUBLE)
			return 0.0;
		return val->v_double;
	}

	bool as_bool() { 
		if(val == nullptr || val->type != Value::BOOL)
			return false;
		return val->v_bool; 
	}

	std::string as_string() { 
		if(val == nullptr || val->type != Value::STRING)
			return "";
		return val->v_string;
	}

	char* as_string_array() { 
		if(val == nullptr || val->type != Value::STRING)
			return nullptr;
		return val->v_string;
	}

	Table* as_table() { 
		if(val == nullptr || val->type != Value::TABLE)
			return nullptr;
		return val->v_table;
	}

	Table* force_table() { 
		if(val == nullptr) return nullptr;
		if(val->type == Value::TABLE)
			return val->v_table;
		else {
			if(val->prototype != nullptr)
				return ValueHandle(val->prototype).force_table();
			return nullptr;
		}
	}
};

#endif // THOUGHT_VALUE_H