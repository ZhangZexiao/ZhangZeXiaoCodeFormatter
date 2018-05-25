#pragma once
#include "IModifier.h"
#include <algorithm>
class Modifier_convert_include_header_path_separator_from_slash_to_backslash : public IModifier
{
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			//https://docs.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp
			if (0 == it->find("#include"))
			{
				assert(TokenSequence::value_type::npos == it->find('\n'));
				std::replace(it->begin(), it->end(), '/', '\\');
			}
			it++;
		}
		return tokenSequence;
	}
};