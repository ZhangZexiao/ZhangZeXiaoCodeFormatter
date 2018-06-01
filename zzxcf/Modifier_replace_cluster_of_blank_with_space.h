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
private:
	void changeToken(TokenSequence::value_type&token)const
	{
		TokenSequence::value_type temp;
		std::copy_if(token.begin(), token.end(), std::back_inserter(temp), [](const auto &c) { return !std::isblank(c); });
		//if any blank been erased, then append a space.
		if (token.length() > temp.length())
		{
			temp.push_back(StringsManager::GetString_Space()[0]);
		}
		token = temp;
	}
};