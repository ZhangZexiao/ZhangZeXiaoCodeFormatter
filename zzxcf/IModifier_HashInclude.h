#pragma once
#include "IModifier_Token.h"
#include "StringsManager.h"
#include <cassert>
//https://docs.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp
class IModifier_HashInclude : public IModifier_Token
{
private:
	bool isCandidateToken(const TokenSequence::value_type&token)const
	{
		return 0 == token.find(StringsManager::GetString_HashInclude());
	}
};