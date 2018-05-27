#pragma once
#include "IModifier.h"
class IModifier_Token : public IModifier
{
protected:
	virtual bool isCandidateToken(TokenSequence::iterator&it)const = 0;
	virtual void changeToken(TokenSequence::iterator&it) = 0;
public:
	TokenSequence&Modify(TokenSequence&tokenSequence)
	{
		TokenSequence::iterator it = tokenSequence.begin();
		while (tokenSequence.end() != it)
		{
			if (this->isCandidateToken(it))
			{
				this->changeToken(it);
			}
			it++;
		}
		return tokenSequence;
	}
};