#pragma once
#include "IModifier.h"
#include "StringsManager.h"
#include <cassert>
//http://en.cppreference.com/w/cpp/string/byte/isblank
//space is 0x20
//horizontal tab is 0x09
//blank is space or horizontal tab
//line feed is \n(na)
//vertical tab is \v(vb)
//form feed is \f(fc)
//carriage return is \r(rd)
//whitespaces are \n(0x0a) \v(0x0b) \f(0x0c) \r(0x0d)
//http://en.cppreference.com/w/cpp/string/byte/isspace
class Modifier_replace_c_comment_with_space : public IModifier
{
private:
	bool isEndsWith(const TokenSequence::value_type&s1, const TokenSequence::value_type&s2)const
	{
		if (s2.length() > s1.length())
		{
			return false;
		}
		return s1.find_last_of(s2) + s2.length() == s1.length();
	}
protected:
	bool isCStyleComment(const TokenSequence::value_type&token)const
	{
		if (0 == token.find(StringsManager::GetString_CStyleCommentBegin()))
		{
			if (this->isEndsWith(token, StringsManager::GetString_CStyleCommentEnd()))
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
			if (this->isCStyleComment(*it))
			{
				*it = StringsManager::GetString_Space();
			}
			it++;
		}
		return tokenSequence;
	}
};