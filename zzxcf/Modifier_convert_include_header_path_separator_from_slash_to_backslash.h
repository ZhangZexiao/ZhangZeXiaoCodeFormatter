#pragma once
#include "IModifier_HashInclude.h"
#include "StringsManager.h"
#include <algorithm>
class Modifier_convert_include_header_path_separator_from_slash_to_backslash : public IModifier_HashInclude
{
private:
	void changeToken(TokenSequence::iterator&it)
	{
		std::replace(it->begin(), it->end(), StringsManager::GetString_Slash()[0], StringsManager::GetString_BackSlash()[0]);
	}
};