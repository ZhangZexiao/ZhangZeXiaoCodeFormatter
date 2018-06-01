#pragma once
#include "IModifier_Token.h"
#include <algorithm>
#include <cctype>
class IModifier_WhiteSpace : public IModifier_Token
{
private:
	bool isCandidateToken(const TokenSequence::value_type&token)const
	{
		return std::all_of(token.begin(), token.end(), [](const auto &c) { return std::isspace(c); });
	}
};