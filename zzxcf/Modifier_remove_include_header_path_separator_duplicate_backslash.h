#pragma once
#include "IModifier_HashInclude.h"
#include "StringsManager.h"
class Modifier_remove_include_header_path_separator_duplicate_backslash : public IModifier_HashInclude
{
private:
	void changeToken(TokenSequence::iterator&it)
	{
		while (TokenSequence::value_type::npos != it->find(StringsManager::GetString_TwoBackSlashes()))
		{
			it->replace(it->find(StringsManager::GetString_TwoBackSlashes()), StringsManager::GetString_TwoBackSlashes().length(), StringsManager::GetString_BackSlash());
		}
	}
};