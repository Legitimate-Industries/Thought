#ifndef THOUGHTC_INFIXPARSELET_H
#define THOUGHTC_INFIXPARSELET_H

#include "../nodes/Node.h"

namespace Thought {
class Parser;

class InfixParslet {
public:
	virtual ~InfixParslet() {}
	virtual Node* parse(Parser*, Node*, Token) = 0;
	virtual int getPrecedence() = 0;
};
};

#endif // AMAC_INFIXPARSELET_H