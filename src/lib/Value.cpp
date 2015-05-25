#include "Value.h"
#include "VM.h"
using namespace Thought;

void Value::destroy(bool iso) {
	if(type != DESTROYED) {
		if(iso) home->isolate(this);

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