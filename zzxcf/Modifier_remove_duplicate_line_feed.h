#pragma once
#include "IModifier_WhiteSpace.h"
#include "StringsManager.h"
#include <algorithm>
class Modifier_remove_duplicate_line_feed : public IModifier_WhiteSpace
{
protected:
	void changeToken(TokenSequence::iterator&it)
	{
		TokenSequence::value_type temp;
		//erase all line feed.
		std::copy_if(it->begin(), it->end(), std::back_inserter(temp), [](TokenSequence::value_type::value_type c) { return StringsManager::GetString_LineFeed()[0] != c; });
		if (it->length() > temp.length())
		{
			temp.push_back(StringsManager::GetString_LineFeed()[0]);
		}
		*it = temp;
	}
};