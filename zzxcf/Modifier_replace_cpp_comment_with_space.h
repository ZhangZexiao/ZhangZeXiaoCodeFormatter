#pragma once
#include "Modifier_replace_c_comment_with_space.h"
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
class Modifier_replace_cpp_comment_with_space : public Modifier_replace_c_comment_with_space
{
protected:
	bool isCppStyleComment(const std::string&token)const
	{
		if (0 == token.find(StringsManager::GetString_CppStyleComment()))
		{
			return true;
		}
		return false;
	}
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			if (this->isCppStyleComment(*it) || this->isCStyleComment(*it))
			{
				*it = StringsManager::GetString_Space();
			}
			it++;
		}
		return tokenSequence;
	}
};