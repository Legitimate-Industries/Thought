#ifndef THOUGHT_VALUE_H
#define THOUGHT_VALUE_H

#include <cstdint>
#include <base/Array.h>
#include <string>

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
	};

	// Should suffice for now (make it an array for multiple prototypes)
	Value* prototype;

	// Accessing interface
	double as_double() { return (type == DOUBLE ? v_double : 0.0); }
	bool as_bool() { return (type == BOOL ? v_bool : false); }
	std::string as_string() { return (type == STRING ? std::string(v_string) : ""); }
	char* as_string_array() { return (type == STRING ? v_string : nullptr); }

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
};

#endif // THOUGHT_VALUE_H