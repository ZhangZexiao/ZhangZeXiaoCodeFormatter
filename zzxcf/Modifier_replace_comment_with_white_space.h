#pragma once
#include "IModifier.h"
#include "StringsManager.h"
#include <cassert>
class Modifier_replace_comment_with_white_space : public IModifier
{
private:
	bool isComment(std::string token)
	{
		if (0 == token.find(StringsManager::GetString_CppStyleComment()))
		{
			return true;
		}
		if (0 == token.find(StringsManager::GetString_CStyleCommentBegin()))
		{
			if (token.find_last_of(StringsManager::GetString_CStyleCommentEnd()) + StringsManager::GetString_CStyleCommentEnd().length() == token.length())
			{
				return true;
			}
		}
		return false;
	}
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			if (this->isComment(*it))
			{
				*it = StringsManager::GetString_WhiteSpace();
			}
			it++;
		}
		return tokenSequence;
	}
};