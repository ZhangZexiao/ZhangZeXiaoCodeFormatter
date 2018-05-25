#pragma once
#include "IModifier.h"
#include "StringsManager.h"
#include <cassert>
#include <algorithm>
class Modifier_convert_include_header_path_separator_from_backslash_to_slash : public IModifier
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
				std::replace(it->begin(), it->end(), StringsManager::GetString_BackSlash()[0], StringsManager::GetString_Slash()[0]);
			}
			it++;
		}
		return tokenSequence;
	}
};