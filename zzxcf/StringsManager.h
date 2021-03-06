#pragma once
#include <string>
class StringsManager
{
public:
	static const std::string&GetString_Space()
	{
		static const std::string space(" ");
		return space;
	}
	static const std::string&GetString_CppStyleComment()
	{
		static const std::string comment("//");
		return comment;
	}
	static const std::string&GetString_CStyleCommentBegin()
	{
		static const std::string comment("/*");
		return comment;
	}
	static const std::string&GetString_CStyleCommentEnd()
	{
		static const std::string comment("*/");
		return comment;
	}
	static const std::string&GetString_BackSlash()
	{
		static const std::string backSlash("\\");
		return backSlash;
	}
	static const std::string&GetString_TwoBackSlashes()
	{
		static const std::string twoBackSlashes("\\\\");
		return twoBackSlashes;
	}
	static const std::string&GetString_Slash()
	{
		static const std::string slash("/");
		return slash;
	}
	static const std::string&GetString_TwoSlashes()
	{
		static const std::string twoSlashes("//");
		return twoSlashes;
	}
	static const std::string&GetString_HashInclude()
	{
		static const std::string hashInclude("#include");
		return hashInclude;
	}
	static const std::string&GetString_Hash()
	{
		static const std::string hash("#");
		return hash;
	}
	/*
	Line Feed '\n' takes the cursor to newline but does not take it to the beginning. 
	Wheareas the Carriage Return '\r' does just the opposite. 
	The combination '\r\n' is used as EOF (End Of File).
	*/
	static const std::string&GetString_LineFeed()
	{
		static const std::string lineFeed("\n");
		return lineFeed;
	}
	static const std::string&GetString_LineContinuation()
	{
		static const std::string lineContinuation("\\\n");
		return lineContinuation;
	}
};
