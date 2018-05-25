#pragma once
#include "IModifier.h"
#include "StringsManager.h"
#include <cassert>
class Modifier_remove_include_header_path_separator_duplicate_backslash : public IModifier
{
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			//https://docs.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp
			if (0 == it->find(StringsManager::GetString_HashInclude()))
			{
				assert(TokenSequence::value_type::npos == it->find(StringsManager::GetString_LineFeed()));
				while (TokenSequence::value_type::npos != it->find(StringsManager::GetString_TwoBackSlashes()))
				{
					it->replace(it->find(StringsManager::GetString_TwoBackSlashes()), StringsManager::GetString_TwoBackSlashes().length(), StringsManager::GetString_BackSlash());
				}
			}
			it++;
		}
		return tokenSequence;
	}
};