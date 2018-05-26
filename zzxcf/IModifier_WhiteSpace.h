#pragma once
#include "IModifier.h"
#include "StringsManager.h"
#include <algorithm>
#include <cassert>
#include <cctype>
class IModifier_WhiteSpace : public IModifier
{
protected:
	bool areWhiteSpaces(const TokenSequence::value_type token)const
	{
		assert(token.length() > 0);
		return std::all_of(token.begin(), token.end(), [](TokenSequence::value_type::value_type c) { return std::isspace(c); });
	}
	virtual void changeToken(TokenSequence::iterator it) = 0;
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			if (this->areWhiteSpaces(*it))
			{
				this->changeToken(it);
			}
			it++;
		}
		return tokenSequence;
	}
};