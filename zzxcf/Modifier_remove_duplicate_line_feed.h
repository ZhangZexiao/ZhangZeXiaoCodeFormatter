#pragma once
#include "IModifier_WhiteSpace.h"
#include "StringsManager.h"
#include <algorithm>
#include <iostream>
//https://docs.microsoft.com/en-us/cpp/preprocessor/phases-of-translation
//Line splicing
//All lines ending in a backslash(\) and immediately followed by a newline character 
//are joined with the next line in the source file forming logical lines 
//from the physical lines.Unless it is empty, 
//a source file must end in a newline character that is not preceded by 
//a backslash.
//https://msdn.microsoft.com/en-us/library/6aw8xdf2.aspx
//The backslash character (\) is a line-continuation character when it is placed at the end of a line.
class Modifier_remove_duplicate_line_feed : public IModifier_WhiteSpace
{
private:
	void changeToken(TokenSequence::value_type&token)
	{
		TokenSequence::value_type temp;
		std::copy_if(token.begin(), token.end(), std::back_inserter(temp), [](const auto &c) { return StringsManager::GetString_LineFeed()[0] != c; });
		if (token.length() > temp.length())
		{
			//temp.push_back(StringsManager::GetString_LineFeed()[0]);
			//trick to avoid line continuation issue.
			temp = StringsManager::GetString_LineFeed() + temp;
		}
		token = temp;
	}
};