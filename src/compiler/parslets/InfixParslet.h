#ifndef THOUGHTC_INFIXPARSELET_H
#define THOUGHTC_INFIXPARSELET_H

#include "../nodes/Node.h"

namespace Thought {
class Parser;

class InfixParslet {
public:
	virtual ~InfixParslet() {}
	virtual NodePtr parse(Parser*, NodePtr, Token) = 0;
	virtual int getPrecedence() = 0;
};
};

#endif // AMAC_INFIXPARSELET_H