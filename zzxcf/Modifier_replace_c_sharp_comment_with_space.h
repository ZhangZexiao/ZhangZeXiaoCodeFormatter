#pragma once
#include "Modifier_replace_cpp_comment_with_space.h"
#include "StringsManager.h"
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

//The are following  types of comments in C#
// for single line comments
/* for multi line comments */
/// XML tags displayed in a code comment
class Modifier_replace_c_sharp_comment_with_space : public Modifier_replace_cpp_comment_with_space
{
	//do nothing currently
};