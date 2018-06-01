#pragma once
#include "IModifier_Token.h"
#include "StringsManager.h"
#include "..\zzxcf_tokenizer\zzxcf_tokenizer.h"
#include "Modifier_replace_cluster_of_blank_with_space.h"
#include <algorithm>
//A group of the same or similar elements gathered or 
//occurring closely together;
//https://www.thefreedictionary.com/cluster
class Modifier_replace_cluster_of_blank_with_space_in_directive : public IModifier_Token
{
private:
	bool isCandidateToken(const TokenSequence::value_type&token)const
	{
		return 0 == token.find(StringsManager::GetString_Hash());
	}
	class AppendStringFunctor
	{
	private:
		TokenSequence::value_type *token;
	public:
		AppendStringFunctor(TokenSequence::value_type *token)
		{
			this->token = token;
		}
		void operator()(const TokenSequence::value_type &piece)
		{
			this->token->append(piece);
		}
	};
	void changeToken(TokenSequence::value_type&token)const
	{
		TokenSequence tokenSequence;
		while (TokenSequence::value_type::npos != token.find(StringsManager::GetString_LineContinuation()))
		{
			token.replace(token.find(StringsManager::GetString_LineContinuation()), StringsManager::GetString_LineContinuation().length(), TokenSequence::value_type());
		}
		tokenize(token.substr(StringsManager::GetString_Hash().length()), tokenSequence);
		Modifier_replace_cluster_of_blank_with_space m;
		m.Modify(tokenSequence);
		token = StringsManager::GetString_Hash();
		//error C3493 : 'token' cannot be implicitly captured because 
		//no default capture mode has been specified
		/*
		error C2663: 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::append':
		7 overloads have no legal conversion for 'this' pointer
		see reference to function template instantiation
		'auto Modifier_replace_cluster_of_blank_with_space_in_directive::changeToken::<lambda_f0217d3c4317e504c6c9a15b33ba6c5a>::operator ()<std::basic_string<char,std::char_traits<char>,std::allocator<char>>>(std::basic_string<char,std::char_traits<char>,std::allocator<char>> &) const' being compiled
		*/
		//std::for_each(tokenSequence.begin(), tokenSequence.end(), [token](const auto &piece) { token.append(piece); });
		std::for_each(tokenSequence.begin(), tokenSequence.end(), AppendStringFunctor(&token));
	}
};