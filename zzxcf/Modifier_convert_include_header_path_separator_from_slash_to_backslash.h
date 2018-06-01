#pragma once
#include "IModifier_HashInclude.h"
#include "StringsManager.h"
#include <algorithm>
class Modifier_convert_include_header_path_separator_from_slash_to_backslash : public IModifier_HashInclude
{
private:
	void changeToken(TokenSequence::value_type&token)
	{
		std::replace(token.begin(), token.end(), StringsManager::GetString_Slash()[0], StringsManager::GetString_BackSlash()[0]);
	}
};