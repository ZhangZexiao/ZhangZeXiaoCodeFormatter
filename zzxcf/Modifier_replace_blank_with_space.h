#pragma once
#include "IModifier.h"
#include "StringsManager.h"
#include <algorithm>
#include <cassert>
#include <cctype>
class Modifier_replace_blank_with_space : public IModifier
{
private:
	bool areWhiteSpaces(const TokenSequence::value_type token)const
	{
		assert(token.length() > 0);
		return std::all_of(token.begin(), token.end(), [](TokenSequence::value_type::value_type c) { return std::isspace(c); });
	}
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			if (this->areWhiteSpaces(*it))
			{
				TokenSequence::value_type temp;
				//erase all blank.
				std::copy_if(it->begin(), it->end(), std::back_inserter(temp), [](TokenSequence::value_type::value_type c) { return !std::isblank(c); });
				//if any blank been erased, then append a space.
				if (it->length() > temp.length())
				{
					temp.push_back(StringsManager::GetString_Space()[0]);
				}				
				*it = temp;
			}
			it++;
		}
		return tokenSequence;
	}
};