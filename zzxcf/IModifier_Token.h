#pragma once
#include "IModifier.h"
#include <algorithm>
class IModifier_Token : public IModifier
{
protected:
	virtual bool isCandidateToken(const TokenSequence::value_type&)const = 0;
	virtual void changeToken(TokenSequence::value_type&)const = 0;
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)const
	{
		std::for_each(tokenSequence.begin(), tokenSequence.end(), [this](auto &token) { if (this->isCandidateToken(token)) { this->changeToken(token); } });
		return tokenSequence;
	}
};