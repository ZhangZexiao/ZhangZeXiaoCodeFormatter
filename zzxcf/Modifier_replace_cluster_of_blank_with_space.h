#pragma once
#include "IModifier_WhiteSpace.h"
#include "StringsManager.h"
#include <algorithm>
#include <cctype>
//A group of the same or similar elements gathered or 
//occurring closely together;
//https://www.thefreedictionary.com/cluster
class Modifier_replace_cluster_of_blank_with_space : public IModifier_WhiteSpace
{
protected:
	void changeToken(TokenSequence::iterator&it)
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
};