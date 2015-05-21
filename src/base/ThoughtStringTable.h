#ifndef THOUGHT_BASE_STRINGTABLE_H
#define THOUGHT_BASE_STRINGTABLE_H

#include <vector>
#include <string>
#include "ThoughtString.h"

namespace Thought {
class StringTable {
	std::vector<std::string> mStrings;
public:
	StringID addString(std::string);
	std::string getString(StringID);

	StringID findID(std::string);
};
};

#endif // THOUGHT_BASE_STRINGTABLE_H