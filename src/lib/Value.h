#ifndef THOUGHT_VALUE_H
#define THOUGHT_VALUE_H

#include <cstdint>
#include <base/Array.h>
#include <string>

#include "Table.h"

namespace Thought {
	struct Value;
};

class Thought::Value {
	std::size_t refs;
	// Make values only constructable by a VM Object
	friend class VM;
	Value() : refs(1) {}

	friend class ValueBaseRef; // Allows for destruction of a value
	void destroy() {
		switch(type) {
			case STRING:
				delete[] v_string;
				break;
			case TABLE:
				delete v_table;
				break;
		};
		delete this;
	}
public:
	enum Type {
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

	// Accessing interface
	double as_double() { return (type == DOUBLE ? v_double : 0.0); }
	bool as_bool() { return (type == BOOL ? v_bool : false); }
	std::string as_string() { return (type == STRING ? std::string(v_string) : ""); }
	char* as_string_array() { return (type == STRING ? v_string : nullptr); }
	Table* as_table() { return (type == TABLE ? v_table : nullptr); }
	Table* force_table() { return (type == TABLE ? v_table : (prototype != nullptr ? prototype->force_table() : nullptr)); }

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
};

#endif // THOUGHT_VALUE_H