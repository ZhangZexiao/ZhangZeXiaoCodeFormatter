#pragma once
#include "IModifier_Token.h"
#include <algorithm>
#include <cassert>
#include <cctype>
class IModifier_WhiteSpace : public IModifier_Token
{
private:
	bool areWhiteSpaces(const TokenSequence::value_type&token)const
	{
		assert(token.length() > 0);
		return std::all_of(token.begin(), token.end(), [](TokenSequence::value_type::value_type c) { return std::isspace(c); });
	}
	bool isCandidateToken(TokenSequence::iterator&it)const
	{
		return this->areWhiteSpaces(*it);
	}
};