#pragma once
#include "IModifier_Token.h"
#include "StringsManager.h"
#include <algorithm>
//https://en.wikipedia.org/wiki/Code_bloat
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
class Modifier_remove_line_continuation : public IModifier_Token
{
private:
	bool isCandidateToken(TokenSequence::iterator&it)const
	{
		return TokenSequence::value_type::npos != it->find(StringsManager::GetString_LineContinuation());
	}
	void changeToken(TokenSequence::iterator&it)
	{
		TokenSequence::value_type::size_type index = it->find(StringsManager::GetString_LineContinuation());
		do
		{
			it->erase(index, StringsManager::GetString_LineContinuation().length());
			index = it->find(StringsManager::GetString_LineContinuation(), index);
		}
		while (TokenSequence::value_type::npos != index);
	}
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (true)
		{
			it = std::adjacent_find(it, tokenSequence.end(), [](const TokenSequence::value_type &t1, const TokenSequence::value_type &t2) { return t1 == StringsManager::GetString_BackSlash() && 0 == t2.find(StringsManager::GetString_LineFeed()); });
			if (tokenSequence.end() == it)
			{
				break;
			}
			it = tokenSequence.erase(it);
			it->erase(0, StringsManager::GetString_LineFeed().length());
			if (it->empty())
			{
				it = tokenSequence.erase(it);
			}
		}
		IModifier_Token::Modify(tokenSequence);
		return tokenSequence;
	}
};