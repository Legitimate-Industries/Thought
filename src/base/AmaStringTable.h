#ifndef AMA_BASE_STRINGTABLE_H
#define AMA_BASE_STRINGTABLE_H

#include <vector>
#include <string>
#include "AmaString.h"

namespace Ama {
class StringTable {
	std::vector<std::string> mStrings;
public:
	StringID addString(std::string);
	std::string getString(StringID);

	StringID findID(std::string);
};
};

#endif // AMA_BASE_STRINGTABLE_H