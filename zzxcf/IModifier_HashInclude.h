#pragma once
#include "IModifier_Token.h"
#include "StringsManager.h"
#include <cassert>
//https://docs.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp
class IModifier_HashInclude : public IModifier_Token
{
private:
	bool isCandidateToken(TokenSequence::iterator&it)const
	{
		assert(TokenSequence::value_type::npos == it->find(StringsManager::GetString_LineFeed()));
		return 0 == it->find(StringsManager::GetString_HashInclude());
	}
};