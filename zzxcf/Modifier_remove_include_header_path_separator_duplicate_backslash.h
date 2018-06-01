#pragma once
#include "IModifier_HashInclude.h"
#include "StringsManager.h"
class Modifier_remove_include_header_path_separator_duplicate_backslash : public IModifier_HashInclude
{
private:
	void changeToken(TokenSequence::value_type&token)
	{
		while (TokenSequence::value_type::npos != token.find(StringsManager::GetString_TwoBackSlashes()))
		{
			token.replace(token.find(StringsManager::GetString_TwoBackSlashes()), StringsManager::GetString_TwoBackSlashes().length(), StringsManager::GetString_BackSlash());
		}
	}
};