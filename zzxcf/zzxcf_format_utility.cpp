#include "zzxcf_format_utility.h"
#include "zzxcf_command_line_option.h"
#include "zzxcf_multithread_utility.h"
#include "macro.h"
#include "../zzxcf_tokenizer/zzxcf_tokenizer.h"
#include "Modifier_convert_include_header_path_separator_from_backslash_to_slash.h"
#include "Modifier_remove_include_header_path_separator_duplicate_slash.h"
#include "Modifier_convert_include_header_path_separator_from_slash_to_backslash.h"
#include "Modifier_remove_include_header_path_separator_duplicate_backslash.h"
#include "Modifier_replace_c_comment_with_space.h"
#include "Modifier_replace_cpp_comment_with_space.h"
namespace format_utility_namespace
{
	extern zzxcf_map comment_map=zzxcf_map();
	extern zzxcf_map identifier_map=zzxcf_map();
	extern zzxcf_map inverse_identifier_map=zzxcf_map();
	extern zzxcf_map directive_map=zzxcf_map();
	extern type_directive_list directive_list=type_directive_list();
	extern type_directive_list failed_regrouping_file_paths=type_directive_list();
	extern sqlite3*pFunctionsDetailsDatabase=nullptr;
	extern sqlite3_stmt*pFunctionsDetailsStatementCreateTable=nullptr;
	extern sqlite3_stmt*pFunctionsDetailsStatementInsertTable=nullptr;
	extern const std::string sqlFunctionsDetailsCreateTable
	{
		"CREATE TABLE T_FUNCTIONSDETAILS(F_INDEX INTEGER PRIMARY KEY,F_FILENAME,F_FUNCTIONNAME,F_FUNCTIONRETURNTYPE,F_FUNCTIONDECLARATION,F_FUNCTIONBODY,F_NUMBEROFLINES,F_OFFSET);"
	};
	extern const std::string sqlFunctionsDetailsInsertTable
	{
		"INSERT INTO T_FUNCTIONSDETAILS(F_FILENAME,F_FUNCTIONNAME,F_FUNCTIONRETURNTYPE,F_FUNCTIONDECLARATION,F_FUNCTIONBODY,F_NUMBEROFLINES,F_OFFSET) VALUES(?,?,?,?,?,?,?);"
	};
	bool is_char_utf8_bom_EF(const char c)
	{
		return 0xEF==c;
	}
	bool is_char_utf8_bom_BB(const char c)
	{
		return 0xBB==c;
	}
	bool is_char_utf8_bom_BF(const char c)
	{
		return 0xBF==c;
	}
	bool is_char_utf8_bom(const char c)
	{
		return is_char_utf8_bom_EF(c)||is_char_utf8_bom_BB(c)||is_char_utf8_bom_BF(c);
	}
	bool is_char_asterisk(const unsigned char c)
	{
		return'*'==c;
	}
	bool is_char_slash(const unsigned char c)
	{
		return'/'==c;
	}
	bool is_char_hash(const unsigned char c)
	{
		return'#'==c;
	}
	bool is_char_underscore(const unsigned char c)
	{
		return'_'==c;
	}
	bool is_char_alpha_numeric(const unsigned char c)
	{
		return std::isalnum(c)!=0;
	}
	bool is_char_asterisk_slash_hash(const unsigned char c)
	{
		return is_char_asterisk(c)||is_char_slash(c)||is_char_hash(c);
	}
	bool is_char_identifier(const unsigned char c)
	{
		return is_char_underscore(c)||is_char_alpha_numeric(c);
	}
	bool is_char_white_space(const unsigned char c)
	{
		return std::isspace(c)!=0;
	}
	bool is_char_digit(const unsigned char c)
	{
		return std::isdigit(c)!=0;
	}
	bool is_char_double_quotation_mark(const unsigned char c)
	{
		return'\"'==c;
	}
	bool is_char_single_quotation_mark(const unsigned char c)
	{
		return'\''==c;
	}
	bool is_token_identifier(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_char_identifier(token[0]);
	}
	bool is_token_digit(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_char_digit(token[0]);
	}
	bool is_token_white_space(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_char_white_space(token[0]);
	}
	bool is_token_left_round_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"("==token;
	}
	bool is_token_right_round_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return")"==token;
	}
	bool is_token_round_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_token_left_round_bracket(token)||is_token_right_round_bracket(token);
	}
	bool is_token_left_square_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"["==token;
	}
	bool is_token_right_square_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"]"==token;
	}
	bool is_token_square_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_token_left_square_bracket(token)||is_token_right_square_bracket(token);
	}
	bool is_token_left_curly_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"{"==token;
	}
	bool is_token_right_curly_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"}"==token;
	}
	bool is_token_curly_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_token_left_curly_bracket(token)||is_token_right_curly_bracket(token);
	}
	bool is_token_bracket(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_token_round_bracket(token)||is_token_square_bracket(token)||is_token_curly_bracket(token);
	}
	bool is_token_semicolon(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return";"==token;
	}
	bool is_token_comma(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return","==token;
	}
	bool is_token_quoted_string(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()<2,false);
		return is_char_double_quotation_mark(token[0])&&is_char_double_quotation_mark(token[token.length()-1]);
	}
	bool is_token_quoted_char(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()<2,false);
		return is_char_single_quotation_mark(token[0])&&is_char_single_quotation_mark(token[token.length()-1]);
	}
	bool is_token_stand_alone(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_token_bracket(token)||is_token_comma(token)||is_token_quoted_string(token)||is_token_quoted_char(token);
	}
	bool is_token_c_style_comment(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		static const std::string left_mark
		{
			"/*"
		};
		static const std::string right_mark
		{
			"*/"
		};
		ON_TRUE_RETURN(left_mark!=token.substr(0,left_mark.length()),false);
		ON_TRUE_RETURN(right_mark!=token.substr(token.length()-right_mark.length()),false);
		return true;
	}
	bool is_token_cpp_style_comment(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		static const std::string cpp_comment_mark
		{
			"//"
		};
		return cpp_comment_mark==token.substr(0,cpp_comment_mark.length());
	}
	bool is_token_comment(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_token_c_style_comment(token)||is_token_cpp_style_comment(token);
	}
	bool is_token_empty_comment(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		ON_TRUE_RETURN(is_token_comment(token)==false,false);
		if(is_token_cpp_style_comment(token))
		{
			size_t i=2;
			while(i<token.length())
			{
				if(!is_char_slash(token[i]))
				{
					break;
				}
				i++;
			}
			while(i<token.length())
			{
				if(!is_char_white_space(token[i]))
				{
					return false;
				}
				i++;
			}
			return true;
		}
		if(is_token_c_style_comment(token))
		{
			std::string comment_content=token.substr(2,token.length()-4);
			size_t i=0;
			while(i<comment_content.length())
			{
				if(!is_char_asterisk(comment_content[i])&& !is_char_white_space(comment_content[i]))
				{
					return false;
				}
				i++;
			}
			return true;
		}
		return false;
	}
	bool is_token_directive(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_char_hash(token[0]);
	}
	bool is_token_directive_define(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		ON_TRUE_RETURN(is_token_directive(token)==false,false);
		return token.find("#define")==0;
	}
	bool is_token_directive_include(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		ON_TRUE_RETURN(is_token_directive(token)==false,false);
		return token.find("#include")==0;
	}
	bool is_token_directive_line(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		ON_TRUE_RETURN(is_token_directive(token)==false,false);
		return token.find("#line")==0;
	}
	bool is_token_directive_pragma_region_endregion(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		ON_TRUE_RETURN(is_token_directive(token)==false,false);
		return token.find("#pragma")==0&&(token.find("region")!=std::string::npos||token.find("endregion")!=std::string::npos);
	}
	bool is_token_white_space_comment_directive(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return is_token_white_space(token)||is_token_comment(token)||is_token_directive(token);
	}
	bool is_token_asm(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"__asm"==token;
	}
	bool is_token_line_continuation(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"\\"==token;
	}
	bool is_token_single_colon(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return":"==token;
	}
	bool is_token_scope_resolution_operator(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"::"==token;
	}
	bool is_token_question_mark(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		return"?"==token;
	}
	bool is_token_separator(const zzxcf_token&token)
	{
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,false);
		ON_TRUE_RETURN(is_token_scope_resolution_operator(token),false);
		ON_TRUE_RETURN((is_token_single_colon(token)||is_token_line_continuation(token)||is_token_semicolon(token)||is_token_curly_bracket(token)||is_token_round_bracket(token)||is_token_directive(token)),true);
		return false;
	}
	bool is_phrase_separator(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_token_comment(*phrase.begin())||is_token_separator(*phrase.begin());
	}
	bool is_phrase_comment(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_token_comment(*phrase.begin());
	}
	bool is_phrase_template(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return*(phrase.begin())=="template";
	}
	bool is_phrase_directive_line(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_token_directive_line(*phrase.begin());
	}
	bool is_phrase_directive_include(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_token_directive_include(*phrase.begin());
	}
	bool is_phrase_contains_token_single_colon(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		if(is_token_left_square_bracket(*phrase.begin()))
		{
			return false;
		}
		zzxcf_phrase::const_iterator it=std::find_if(phrase.begin(),phrase.end(),is_token_question_mark);
		ON_TRUE_RETURN(it!=phrase.end(),false);
		it=std::find_if(phrase.begin(),phrase.end(),is_token_single_colon);
		ON_TRUE_RETURN(it!=phrase.end(),true);
		return false;
	}
	bool is_phrase_contains_number_of_token_commas(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		size_t comma_count=0;
		for(zzxcf_phrase::const_iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(is_token_comma(*it))
			{
				comma_count++;
			}
		}
		return comma_count>command_line_namespace::zzxcf_command_line_option_number_of_commas::number_of_commas;
	}
	bool is_phrase_separator_char(const zzxcf_phrase&phrase,unsigned char c)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		if(is_token_separator(*phrase.begin()))
		{
			if((*phrase.begin())[0]==c)
			{
				return true;
			}
		}
		return false;
	}
	bool is_phrase_directive(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_separator_char(phrase,'#');
	}
	bool is_phrase_left_curly_bracket(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_separator_char(phrase,'{');
	}
	bool is_phrase_right_curly_bracket(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_separator_char(phrase,'}');
	}
	bool is_phrase_left_round_bracket(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_separator_char(phrase,'(');
	}
	bool is_phrase_right_round_bracket(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_separator_char(phrase,')');
	}
	bool is_phrase_semicolon(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_separator_char(phrase,';');
	}
	bool is_phrase_line_continuation(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_separator_char(phrase,'\\');
	}
	bool is_phrase_block_separator(const zzxcf_phrase&phrase)
	{
		assert(phrase.size()>0);
		ON_TRUE_RETURN(phrase.size()==0,false);
		return is_phrase_directive(phrase)||is_phrase_left_curly_bracket(phrase)||is_phrase_right_curly_bracket(phrase);
	}
	bool is_phrase_label(const zzxcf_phrase&phrase)
	{
		if(phrase.size()==2)
		{
			zzxcf_phrase::const_iterator it=phrase.begin();
			if(is_token_identifier(*it)&&(*it)!="default"&&(*it)!="case"&&(*it)!="protected"&&(*it)!="private"&&(*it)!="public")
			{
				it++;
				if(it!=phrase.end()&&is_token_single_colon(*it))
				{
					return true;
				}
			}
		}
		return false;
	}
	bool is_phrase_single_colon(const zzxcf_phrase&phrase)
	{
		if(!is_phrase_label(phrase))
		{
			zzxcf_phrase::const_iterator it=phrase.begin();
			if(it!=phrase.end()&&((*it)=="protected"||(*it)=="private"||(*it)=="public"))
			{
				if(phrase.size()==2)
				{
					while(it!=phrase.end())
					{
						it++;
					}
					it--;
					if(is_token_single_colon(*it))
					{
						return true;
					}
				}
			}
			if(it!=phrase.end()&&((*it)=="default"||(*it)=="case"))
			{
				while(it!=phrase.end())
				{
					it++;
				}
				it--;
				if(is_token_single_colon(*it))
				{
					return true;
				}
			}
		}
		return false;
	}
	bool does_contain_quoted_string(const zzxcf_phrase&phrase)
	{
		for(zzxcf_phrase::const_iterator iter=phrase.begin();iter!=phrase.end();iter++)
		{
			if(is_token_quoted_string(*iter))
			{
				return true;
			}
		}
		return false;
	}
	bool does_contain_bracket(const zzxcf_phrase&phrase)
	{
		for(zzxcf_phrase::const_iterator iter=phrase.begin();iter!=phrase.end();iter++)
		{
			if(is_token_left_square_bracket(*iter)||is_token_right_square_bracket(*iter))
			{
				return true;
			}
		}
		return false;
	}
	std::string&trim_left_star_backslash(std::string&s)
	{
		s.erase(s.begin(),std::find_if(s.begin(),s.end(),std::not1(std::ptr_fun<unsigned char,bool>(is_char_asterisk_slash_hash))));
		return s;
	}
	std::string&trim_right_star_backslash(std::string&s)
	{
		s.erase(std::find_if(s.rbegin(),s.rend(),std::not1(std::ptr_fun<unsigned char,bool>(is_char_asterisk_slash_hash))).base(),s.end());
		return s;
	}
	std::string&trim_star_backslash(std::string&s)
	{
		return trim_left_star_backslash(trim_right_star_backslash(s));
	}
	std::string&transform_to_lower_string(std::string&s)
	{
		std::transform(s.begin(),s.end(),s.begin(),[](const char c){return(char)std::tolower(c);});
		return s;
	}
	std::string&trim_left(std::string&s)
	{
		s.erase(s.begin(),std::find_if(s.begin(),s.end(),std::not1(std::ptr_fun<int,int>(std::isspace))));
		return s;
	}
	std::string&trim_right(std::string&s)
	{
		s.erase(std::find_if(s.rbegin(),s.rend(),std::not1(std::ptr_fun<int,int>(std::isspace))).base(),s.end());
		return s;
	}
	std::string&trim(std::string&s)
	{
		return trim_left(trim_right(s));
	}
	std::vector<std::string> &split(const std::string&s,char delim,std::vector<std::string> &elems)
	{
		std::stringstream ss(s);
		std::string item;
		while(std::getline(ss,item,delim))
		{
			elems.push_back(item);
		}
		return elems;
	}
	std::vector<std::string>split(const std::string&s,char delim)
	{
		std::vector<std::string>elems;
		split(s,delim,elems);
		return elems;
	}
	void to_lower_string(std::string&s)
	{
		std::for_each(s.begin(),s.end(),[](char&c){c=std::tolower(c);});
	}
	bool is_cpp_file(const std::string&fileName)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char name[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(fileName.c_str(),drive,dir,name,ext);
		std::string fileExtension=ext;
		to_lower_string(fileExtension);
		return fileExtension==".cpp";
	}
	size_t count_specific_token(const zzxcf_phrase&phrase,const zzxcf_token&token)
	{
		assert(!phrase.empty());
		ON_TRUE_RETURN(phrase.empty(),0);
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,0);
		return std::count(phrase.begin(),phrase.end(),token);
	}
	size_t count_specific_token(const std::string&s,const zzxcf_token&token)
	{
		assert(s.length()>0);
		ON_TRUE_RETURN(s.length()==0,0);
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,0);
		zzxcf_phrase phrase;
		::tokenize(s,phrase);
		return std::count(phrase.begin(),phrase.end(),token);
	}
	bool does_string_contain_specific_token(const std::string&s,const zzxcf_token&token)
	{
		assert(s.length()>0);
		ON_TRUE_RETURN(s.length()==0,0);
		assert(token.length()>0);
		ON_TRUE_RETURN(token.length()==0,0);
		zzxcf_phrase phrase;
		::tokenize(s,phrase);
		return std::find(phrase.begin(),phrase.end(),token)!=phrase.end();
	}
	size_t split_string_with_substr(const std::string&source,const std::string&substr,std::string&before_substr,std::string&after_substr)
	{
		before_substr.clear();
		after_substr.clear();
		ON_TRUE_RETURN(source.length()==0||substr.length()==0,std::string::npos);
		size_t start_of_substr=source.find(substr);
		if(std::string::npos!=start_of_substr)
		{
			size_t length_of_before_substr=start_of_substr;
			if(length_of_before_substr>0)
			{
				before_substr=source.substr(0,length_of_before_substr);
			}
			size_t start_of_after_substr=length_of_before_substr+substr.length();
			if(start_of_after_substr<source.length())
			{
				after_substr=source.substr(start_of_after_substr);
			}
		}
		return start_of_substr;
	}
	const std::string&get_count_line_format()
	{
		static std::string countLineFormat;
		if(countLineFormat.length()==0)
		{
			countLineFormat="\n";
			countLineFormat.append(50,'/');
			countLineFormat+="%s%05d";
			countLineFormat.append(30,'/');
			countLineFormat+="\n";
		}
		return countLineFormat;
	}
	void write_section_to_file(const zzxcf_section&section,const std::string&fileName)
	{
		FILE*filePtr=0;
		if(!file_operation_namespace::open_file(fileName,"w",&filePtr))
		{
			return;
		}
		size_t tokenSequenceCount=1;
		char szCountLine[512];
		for(zzxcf_section::const_iterator it=section.begin();it!=section.end();it++,tokenSequenceCount++)
		{
			if(command_line_namespace::option_insert_separator_between_phrases_when_saving_interim_result.is_turned_on())
			{
				sprintf_s(szCountLine,get_count_line_format().c_str(),"TOKEN SEQUENCE#",tokenSequenceCount);
				fputs(szCountLine,filePtr);
			}
			size_t tokenCount=1;
			for(zzxcf_phrase::const_iterator iter=it->begin();iter!=it->end();iter++,tokenCount++)
			{
				if(command_line_namespace::option_insert_separator_between_phrases_when_saving_interim_result.is_turned_on())
				{
					sprintf_s(szCountLine,get_count_line_format().c_str(),"TOKEN#",tokenCount);
					fputs(szCountLine,filePtr);
				}
				fputs(iter->c_str(),filePtr);
			}
		}
		file_operation_namespace::close_file(filePtr);
	}
	void concat_tokens_0separator(const zzxcf_phrase&phrase,zzxcf_section&section)
	{
		if(phrase.size()==0)
		{
			return;
		}
		zzxcf_phrase ts;
		for(zzxcf_phrase::const_iterator it=phrase.begin();it!=phrase.end();it++)
		{
			const zzxcf_token&token=it->c_str();
			if(token.length()>0)
			{
				if(is_token_asm(token))
				{
					std::string asmToken=token;
					while((++it)!=phrase.end())
					{
						asmToken+=(*it);
						if(*it=="}")
						{
							break;
						}
					}
					if(ts.size()>0)
					{
						section.push_back(ts);
						ts.clear();
					}
					ts.push_back(asmToken);
					section.push_back(ts);
					ts.clear();
				}
				else if(is_token_separator(token))
				{
					if(ts.size()>0)
					{
						section.push_back(ts);
						ts.clear();
					}
					ts.push_back(token);
					section.push_back(ts);
					ts.clear();
				}
				else
				{
					ts.push_back(token);
				}
			}
		}
		if(ts.size()>0)
		{
			section.push_back(ts);
			ts.clear();
		}
	}
	void concat_tokens_2round_bracket(zzxcf_section&section)
	{
		if(section.size()==0)
		{
			return;
		}
		enum State
		{
			Other,ParenthesisLeft
		};
		State state=Other;
		int left_round_bracket_count=0;
		zzxcf_section tsl;
		zzxcf_phrase ts;
		for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
		{
			const zzxcf_phrase&phrase= *it;
			if(phrase.size()>0)
			{
				switch(state)
				{
				case Other:
					if(is_phrase_left_round_bracket(phrase)&& !left_round_bracket_count)
					{
						left_round_bracket_count++;
						for(zzxcf_phrase::const_iterator iter=phrase.begin();iter!=phrase.end();iter++)
						{
							ts.push_back(*iter);
						}
						state=ParenthesisLeft;
					}
					else
					{
						tsl.push_back(phrase);
					}
					break;
				case ParenthesisLeft:
					if(is_phrase_left_round_bracket(phrase))
					{
						left_round_bracket_count++;
						for(zzxcf_phrase::const_iterator iter=phrase.begin();iter!=phrase.end();iter++)
						{
							ts.push_back(*iter);
						}
					}
					else if(is_phrase_right_round_bracket(phrase))
					{
						left_round_bracket_count--;
						for(zzxcf_phrase::const_iterator iter=phrase.begin();iter!=phrase.end();iter++)
						{
							ts.push_back(*iter);
						}
						if(left_round_bracket_count<0)
						{
							fprintf(stderr,"Warning: round bracket mismatch.\n");
							left_round_bracket_count=0;
						}
						if(left_round_bracket_count==0)
						{
							tsl.push_back(ts);
							ts.clear();
							state=Other;
						}
					}
					else if(is_phrase_line_continuation(phrase))
					{
					}
					else if(is_phrase_directive(phrase))
					{
						if(ts.size()>0)
						{
							tsl.push_back(ts);
							ts.clear();
						}
						tsl.push_back(phrase);
						left_round_bracket_count=0;
						state=Other;
					}
					else
					{
						for(zzxcf_phrase::const_iterator iter=phrase.begin();iter!=phrase.end();iter++)
						{
							ts.push_back(*iter);
						}
					}
					break;
				}
			}
		}
		if(ts.size()>0)
		{
			tsl.push_back(ts);
			ts.clear();
		}
		section=tsl;
	}
	void concat_tokens_3sentence(zzxcf_section&section)
	{
		if(section.size()==0)
		{
			return;
		}
		zzxcf_section tsl;
		zzxcf_phrase ts;
		for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
		{
			zzxcf_phrase phrase= *it;
			if(is_phrase_block_separator(phrase))
			{
				if(ts.size()>0)
				{
					tsl.push_back(ts);
					ts.clear();
				}
				tsl.push_back(phrase);
			}
			else if(is_phrase_line_continuation(phrase))
			{
			}
			else if(is_phrase_semicolon(phrase))
			{
				for(zzxcf_phrase::iterator iter=phrase.begin();iter!=phrase.end();iter++)
				{
					ts.push_back(*iter);
				}
				tsl.push_back(ts);
				ts.clear();
			}
			else
			{
				for(zzxcf_phrase::iterator iter=phrase.begin();iter!=phrase.end();iter++)
				{
					ts.push_back(*iter);
				}
			}
		}
		if(ts.size()>0)
		{
			tsl.push_back(ts);
			ts.clear();
		}
		section=tsl;
	}
	int concat_tokens_6indent(zzxcf_section&section)
	{
		std::string r;
		std::string indentStr;
		int curlyBracketCount=0;
		std::stack<int>curly_bracket_count_stack;
		zzxcf_section tsl;
		for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
		{
			zzxcf_phrase phrase= *it;
			for(zzxcf_phrase::iterator iter=phrase.begin();iter!=phrase.end();iter++)
			{
				r+=iter->c_str();
			}
			if(is_phrase_left_curly_bracket(phrase))
			{
				indentStr="";
				indentStr.append(curlyBracketCount*command_line_namespace::option_number_of_padding_spaces_per_level::number_of_padding_chars_per_level,command_line_namespace::option_number_of_padding_spaces_per_level::padding_char);
				r=indentStr+r+'\n';
				zzxcf_phrase ts;
				ts.push_back(r);
				tsl.push_back(ts);
				r="";
				curlyBracketCount++;
			}
			else if(is_phrase_right_curly_bracket(phrase))
			{
				curlyBracketCount--;
				if(curlyBracketCount<0)
				{
					curlyBracketCount=0;
				}
				indentStr="";
				indentStr.append(curlyBracketCount*command_line_namespace::option_number_of_padding_spaces_per_level::number_of_padding_chars_per_level,command_line_namespace::option_number_of_padding_spaces_per_level::padding_char);
				r=indentStr+r+'\n';
				zzxcf_phrase ts;
				ts.push_back(r);
				tsl.push_back(ts);
				r="";
			}
			else if(is_phrase_label(phrase))
			{
				r+='\n';
				zzxcf_phrase ts;
				ts.push_back(r);
				tsl.push_back(ts);
				r="";
			}
			else if(is_phrase_directive(phrase))
			{
				r+='\n';
				zzxcf_phrase ts;
				ts.push_back(r);
				tsl.push_back(ts);
				r="";
				if(phrase.size()>0)
				{
					if(phrase.begin()->find("#if")==0)
					{
						curly_bracket_count_stack.push(curlyBracketCount);
					}
					else if(phrase.begin()->find("#else")==0||phrase.begin()->find("#elif")==0)
					{
						if(curly_bracket_count_stack.size()>0)
						{
							curlyBracketCount=curly_bracket_count_stack.top();
						}
					}
					else if(phrase.begin()->find("#endif")==0)
					{
						if(curly_bracket_count_stack.size()>0)
						{
							curly_bracket_count_stack.pop();
						}
					}
				}
			}
			else if(is_phrase_single_colon(phrase))
			{
				indentStr="";
				indentStr.append(((curlyBracketCount-1)>=0?(curlyBracketCount-1):0)*command_line_namespace::option_number_of_padding_spaces_per_level::number_of_padding_chars_per_level,command_line_namespace::option_number_of_padding_spaces_per_level::padding_char);
				r=indentStr+r+'\n';
				zzxcf_phrase ts;
				ts.push_back(r);
				tsl.push_back(ts);
				r="";
			}
			else
			{
				indentStr="";
				indentStr.append(curlyBracketCount*command_line_namespace::option_number_of_padding_spaces_per_level::number_of_padding_chars_per_level,command_line_namespace::option_number_of_padding_spaces_per_level::padding_char);
				r=indentStr+r+'\n';
				zzxcf_phrase ts;
				ts.push_back(r);
				tsl.push_back(ts);
				r="";
			}
		}
		if(r.length())
		{
			zzxcf_phrase ts;
			ts.push_back(r);
			tsl.push_back(ts);
		}
		section=tsl;
		return curlyBracketCount;
	}
	zzxcf_token simplify_comment(zzxcf_token&comment,bool flag_case_sensitive)
	{
		assert(is_token_comment(comment));
		zzxcf_token simplified_comment;
		if(is_token_comment(comment))
		{
			if(comment.find("//")==0)
			{
				simplified_comment=comment.substr(2);
			}
			else
			{
				simplified_comment=comment.substr(2,comment.length()-4);
			}
			simplified_comment=trim(simplified_comment);
			std::vector<std::string>lines=split(simplified_comment,'\n');
			simplified_comment.clear();
			for(std::vector<std::string>::iterator it=lines.begin();it!=lines.end();it++)
			{
				simplified_comment+=trim_star_backslash(trim(*it));
			}
			zzxcf_phrase phrase;
			tokenize(simplified_comment,phrase);
			zzxcf_section section;
			section.push_back(phrase);
			take_out_redundant_white_spaces(section);
			simplified_comment.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
				{
					simplified_comment+=iter->c_str();
				}
			}
			if(!flag_case_sensitive)
			{
				simplified_comment=transform_to_lower_string(simplified_comment);
			}
		}
		return simplified_comment;
	}
	zzxcf_token simplify_directive(zzxcf_token&directive,bool flag_case_sensitive=false)
	{
		assert(is_token_directive(directive));
		zzxcf_token simplified_directive;
		if(is_token_directive(directive))
		{
			if(directive.find("#")==0)
			{
				simplified_directive=directive.substr(1);
			}
			simplified_directive=trim(simplified_directive);
			std::vector<std::string>lines=split(simplified_directive,'\n');
			simplified_directive.clear();
			for(std::vector<std::string>::iterator it=lines.begin();it!=lines.end();it++)
			{
				simplified_directive+=trim_star_backslash(trim(*it));
			}
			zzxcf_phrase phrase;
			tokenize(simplified_directive,phrase);
			zzxcf_section section;
			section.push_back(phrase);
			take_out_redundant_white_spaces(section);
			simplified_directive.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
				{
					simplified_directive+=iter->c_str();
				}
			}
			if(!flag_case_sensitive)
			{
				simplified_directive=transform_to_lower_string(simplified_directive);
			}
		}
		return simplified_directive;
	}
	void replace_ifndef_define_endif_directives_with_pragma_once(zzxcf_section&section)
	{
		if(section.size())
		{
			zzxcf_section::const_iterator it=section.end();
			it--;
			while(it!=section.end()&&is_phrase_comment(*it))it--;
			if(it!=section.end())
			{
				if(it->size())
				{
					if((*(it->begin())).find("#endif")==0)
					{
						zzxcf_section::const_iterator it1=section.begin();
						while(it1!=section.end()&&is_phrase_comment(*it1))it1++;
						if(it1!=section.end())
						{
							if(it1->size())
							{
								if((*(it1->begin())).find("#ifndef")==0)
								{
									zzxcf_section::const_iterator it2=it1;
									it2++;
									while(it2!=section.end()&&is_phrase_comment(*it2))it2++;
									if(it2!=section.end())
									{
										if(it2->size())
										{
											if((*(it2->begin())).find("#define")==0)
											{
												section.erase(it);
												section.erase(it2);
												zzxcf_section::const_iterator it3=section.erase(it1);
												zzxcf_phrase phrase;
												phrase.push_back("#pragma once");
												section.insert(it3,phrase);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	int compare_zzxcf_int64_string_pair(zzxcf_int64_string_pair&pre,zzxcf_int64_string_pair&cur)
	{
		if(pre.first==cur.first)
		{
			return strcmp(pre.second.c_str(),cur.second.c_str());
		}
		return pre.first>cur.first;
	}
	void export_comment_map(zzxcf_map&comment_map)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		const DWORD count=1024;
		CHAR file_name[count];
		GetModuleFileNameA(NULL,file_name,count);
		std::string exe_file_name=file_name;
		exe_file_name+=".comment_config";
		zzxcf_map::iterator iter2=comment_map.begin();
		type_count_string_vector comments;
		std::string lines;
		std::list<std::string>line_list;
		for(;iter2!=comment_map.end();iter2++)
		{
			CHAR line[65536]=
			{
				0
			};
			sprintf_s(line,"%I64u\t|\t%i\t|\t%s\t|\t%s",iter2->second.count,iter2->second.operation,iter2->first.c_str(),iter2->second.replacement.c_str());
			std::string comment=line;
			comments.push_back(zzxcf_int64_string_pair(iter2->second.count,comment));
			line_list.push_back(comment);
			comment+="\n";
			lines+=comment;
		}
		file_operation_namespace::write_file(exe_file_name,lines);
		line_list.sort();
		lines.clear();
		for(std::list<std::string>::iterator iter3=line_list.begin();iter3!=line_list.end();iter3++)
		{
			lines+= *iter3;
			lines+="\n";
		}
		file_operation_namespace::write_file(exe_file_name+"_sorted",lines);
		sort(comments.begin(),comments.end(),compare_zzxcf_int64_string_pair);
		lines.clear();
		for(type_count_string_vector::iterator iter4=comments.begin();iter4!=comments.end();iter4++)
		{
			lines+=(*iter4).second;
			lines+="\n";
		}
		file_operation_namespace::write_file(exe_file_name+"_sorted_better",lines);
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void export_id_map(zzxcf_map&id_map)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		const DWORD count=1024;
		CHAR file_name[count];
		GetModuleFileNameA(NULL,file_name,count);
		std::string exe_file_name=file_name;
		exe_file_name+=".id_map";
		zzxcf_map::iterator iter2=id_map.begin();
		type_count_string_vector ids;
		std::string lines;
		for(;iter2!=id_map.end();iter2++)
		{
			CHAR line[65536]=
			{
				0
			};
			sprintf_s(line,"%I64u\t|\t%i\t|\t%s\t|\t%s",iter2->second.count,iter2->second.operation,iter2->first.c_str(),iter2->second.replacement.c_str());
			std::string comment=line;
			ids.push_back(zzxcf_int64_string_pair(iter2->second.count,comment));
			comment+='\n';
			lines+=comment;
		}
		file_operation_namespace::write_file(exe_file_name,lines);
		sort(ids.begin(),ids.end(),compare_zzxcf_int64_string_pair);
		lines.clear();
		for(type_count_string_vector::iterator iter4=ids.begin();iter4!=ids.end();iter4++)
		{
			lines+=(*iter4).second;
			lines+="\n";
		}
		file_operation_namespace::write_file(exe_file_name+"_sorted_better",lines);
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void export_directive_map(zzxcf_map&directive_map,type_directive_list&directive_list)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		const DWORD count=1024;
		CHAR file_name[count];
		GetModuleFileNameA(NULL,file_name,count);
		std::string exe_file_name=file_name;
		exe_file_name+=".directive_map";
		zzxcf_map::iterator iter2=directive_map.begin();
		type_count_string_vector ids;
		std::string lines;
		for(;iter2!=directive_map.end();iter2++)
		{
			CHAR line[65536]=
			{
				0
			};
			sprintf_s(line,"%I64u\t|\t%i\t|\t%s\t|\t%s",iter2->second.count,iter2->second.operation,iter2->first.c_str(),iter2->second.replacement.c_str());
			std::string comment=line;
			ids.push_back(zzxcf_int64_string_pair(iter2->second.count,comment));
			comment+='\n';
			lines+=comment;
		}
		file_operation_namespace::write_file(exe_file_name,lines);
		sort(ids.begin(),ids.end(),compare_zzxcf_int64_string_pair);
		lines.clear();
		for(type_count_string_vector::iterator iter4=ids.begin();iter4!=ids.end();iter4++)
		{
			lines+=(*iter4).second;
			lines+="\n";
		}
		file_operation_namespace::write_file(exe_file_name+"_sorted_better",lines);
		lines.clear();
		for(type_directive_list::iterator iter5=directive_list.begin();iter5!=directive_list.end();iter5++)
		{
			lines+=(*iter5);
			lines+="\n";
		}
		file_operation_namespace::write_file(exe_file_name+"_ordered",lines);
		lines.clear();
		for(type_directive_list::iterator iter5=failed_regrouping_file_paths.begin();iter5!=failed_regrouping_file_paths.end();iter5++)
		{
			lines+=(*iter5);
			lines+="\n";
		}
		file_operation_namespace::write_file(exe_file_name+"_abnormal_termination_file_list",lines);
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void import_comment_map(zzxcf_map&comment_map)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		const DWORD count=1024;
		CHAR file_name[count];
		GetModuleFileNameA(NULL,file_name,count);
		std::string exe_file_name=file_name;
		exe_file_name+=".comment_config";
		std::string lines;
		file_operation_namespace::read_file(exe_file_name,lines);
		std::vector<std::string>line_vector=split(lines,'\n');
		for(std::vector<std::string>::iterator iter2=line_vector.begin();iter2!=line_vector.end();iter2++)
		{
			type_map_value second;
			std::vector<std::string>field_vector=split(*iter2,'|');
			if(field_vector.size()==4)
			{
				second.count=_atoi64(field_vector[0].c_str());
				second.operation=(type_map_value_operation)atoi(field_vector[1].c_str());
				second.replacement=trim(field_vector[3]);
				std::string simplified_comment=trim(field_vector[2]);
				zzxcf_map::iterator iter1=comment_map.find(simplified_comment);
				if(iter1!=comment_map.end())
				{
					iter1->second.count+=second.count;
					iter1->second.operation=second.operation;
					iter1->second.replacement=second.replacement;
				}
				else
				{
					comment_map[simplified_comment]=second;
				}
			}
		}
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void import_id_map(zzxcf_map&id_map,zzxcf_map&inverted_id_map)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		const DWORD file_name_length=1024;
		CHAR file_name[file_name_length];
		GetModuleFileNameA(NULL,file_name,file_name_length);
		std::string exe_file_name=file_name;
		exe_file_name+=".id_map";
		std::string lines;
		file_operation_namespace::read_file(exe_file_name,lines);
		std::vector<std::string>line_vector=split(lines,'\n');
		for(std::vector<std::string>::iterator it=line_vector.begin();it!=line_vector.end();it++)
		{
			type_map_value second;
			std::vector<std::string>fields=split(*it,'|');
			if(fields.size()==4)
			{
				second.count=_atoi64(fields[0].c_str());
				second.operation=(type_map_value_operation)atoi(fields[1].c_str());
				second.replacement=trim(fields[3]);
				std::string id=trim(fields[2]);
				zzxcf_map::iterator it1=id_map.find(id);
				if(it1!=id_map.end())
				{
					it1->second.count+=second.count;
					it1->second.operation=second.operation;
					it1->second.replacement=second.replacement;
				}
				else
				{
					id_map[id]=second;
				}
				std::string replacement=second.replacement;
				second.replacement=id;
				zzxcf_map::iterator it2=inverted_id_map.find(replacement);
				if(it2!=inverted_id_map.end())
				{
					it2->second.count+=second.count;
					it2->second.operation=second.operation;
					it2->second.replacement=second.replacement;
				}
				else
				{
					inverted_id_map[replacement]=second;
				}
			}
		}
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void import_directive_map(zzxcf_map&directive_map)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		const DWORD count=1024;
		CHAR file_name[count];
		GetModuleFileNameA(NULL,file_name,count);
		std::string exe_file_name=file_name;
		exe_file_name+=".directive_map";
		std::string lines;
		file_operation_namespace::read_file(exe_file_name,lines);
		std::vector<std::string>line_vector=split(lines,'\n');
		for(std::vector<std::string>::iterator iter2=line_vector.begin();iter2!=line_vector.end();iter2++)
		{
			type_map_value second;
			std::vector<std::string>field_vector=split(*iter2,'|');
			if(field_vector.size()==4)
			{
				second.count=_atoi64(field_vector[0].c_str());
				second.operation=(type_map_value_operation)atoi(field_vector[1].c_str());
				second.replacement=trim(field_vector[3]);
				std::string simplified_comment=trim(field_vector[2]);
				zzxcf_map::iterator iter1=directive_map.find(simplified_comment);
				if(iter1!=directive_map.end())
				{
					iter1->second.count+=second.count;
					iter1->second.operation=second.operation;
					iter1->second.replacement=second.replacement;
				}
				else
				{
					directive_map[simplified_comment]=second;
				}
			}
		}
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void accumulate_map_comment(zzxcf_section&section,zzxcf_map&comment_map)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
		{
			if((*it).size()>0)
			{
				for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
				{
					if(is_token_comment(*iter))
					{
						std::string simplified_comment=simplify_comment(*iter);
						zzxcf_map::iterator iter1=comment_map.find(simplified_comment);
						if(iter1!=comment_map.end())
						{
							iter1->second.count++;
							if(iter1->second.operation==k_delete)
							{
								(*it).erase(iter);
								it--;
								break;
							}
							else if(iter1->second.operation==k_simplified)
							{
								*iter=std::string("//")+simplified_comment;
							}
							else if(iter1->second.operation==eReplace)
							{
								*iter=iter1->second.replacement;
							}
						}
						else
						{
							type_map_value value;
							value.operation=eKeep;
							value.count=1;
							value.replacement="";
							comment_map[simplified_comment]=value;
						}
					}
				}
			}
			else
			{
				it=section.erase(it);
				if(it!=section.begin())
				{
					it--;
				}
			}
		}
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void accumulate_map_identifier(zzxcf_section&section,zzxcf_map&identifier_map,zzxcf_map&inverted_identifier_map)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		bool goto_begin=false;
		for(zzxcf_section::iterator pointer_to_phrase=section.begin();pointer_to_phrase!=section.end();pointer_to_phrase++)
		{
			if(goto_begin)
			{
				pointer_to_phrase=section.begin();
				goto_begin=false;
			}
			if((*pointer_to_phrase).size()>0)
			{
				zzxcf_phrase::iterator pointer_to_first_token=(*pointer_to_phrase).begin();
				zzxcf_phrase::iterator pointer_to_last_token=(*pointer_to_phrase).end();
				pointer_to_last_token--;
				if((*pointer_to_first_token)=="class"&&(*pointer_to_last_token)!=";")
				{
					if((*pointer_to_last_token)==":")
					{
						pointer_to_last_token--;
					}
					if(is_token_identifier(*pointer_to_last_token))
					{
						std::string identifier=(*pointer_to_last_token);
						if(identifier_map.find(identifier)==identifier_map.end())
						{
							std::string new_identifier=identifier;
							if(is_class_name_of_autodesk_frame_generator(new_identifier))
							{
								new_identifier=beautify_class_name_of_autodesk_frame_generator(new_identifier);
								new_identifier=generate_unique_identifier(inverted_identifier_map,new_identifier);
							}
							type_map_value value;
							value.operation=eKeep;
							value.count=1;
							value.replacement=new_identifier;
							identifier_map[identifier]=value;
							value.replacement=identifier;
							inverted_identifier_map[new_identifier]=value;
						}
					}
				}
				for(zzxcf_phrase::iterator pointer_to_token=(*pointer_to_phrase).begin();pointer_to_token!=(*pointer_to_phrase).end();pointer_to_token++)
				{
					if(is_token_comment(*pointer_to_token)||is_token_directive(*pointer_to_token))
					{
						*pointer_to_token=replace_identifier_in_raw_phrase(*pointer_to_token,identifier_map);
					}
					if(is_token_identifier(*pointer_to_token))
					{
						zzxcf_map::iterator it=identifier_map.find(*pointer_to_token);
						if(it!=identifier_map.end())
						{
							it->second.count++;
							if(it->second.operation==eReplace)
							{
								*pointer_to_token=it->second.replacement;
							}
						}
						else
						{
							type_map_value value;
							value.operation=eKeep;
							value.count=1;
							value.replacement= *pointer_to_token;
							identifier_map[*pointer_to_token]=value;
						}
					}
				}
			}
			else
			{
				assert(0);
				pointer_to_phrase=section.erase(pointer_to_phrase);
				if(pointer_to_phrase!=section.begin())
				{
					pointer_to_phrase--;
				}
				else
				{
					goto_begin=true;
					assert(0);
				}
			}
		}
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	void accumulate_map_directive(zzxcf_section&section,zzxcf_map&directive_map,type_directive_list&directive_list)
	{
		EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
		{
			if((*it).size()>0)
			{
				for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
				{
					if(is_token_directive(*iter))
					{
						std::string directive=(*iter);
						std::string simplified_directive=simplify_directive(directive,true);
						zzxcf_map::iterator iter1=directive_map.find(simplified_directive);
						if(iter1!=directive_map.end())
						{
							iter1->second.count++;
						}
						else
						{
							type_map_value value;
							value.operation=eKeep;
							value.count=1;
							value.replacement="";
							directive_map[simplified_directive]=value;
						}
						if(directive.find("#include")==std::string::npos&&directive.find("#import")==std::string::npos&&directive.find("#pragma")==std::string::npos&&directive.find("#error")==std::string::npos&&directive.find("#define")==std::string::npos&&directive.find("#undef")==std::string::npos)
						{
							directive_list.push_back(directive);
						}
					}
				}
			}
			else
			{
				it=section.erase(it);
				if(it!=section.begin())
				{
					it--;
				}
			}
		}
		LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
	}
	bool replace_identifier(zzxcf_phrase::iterator&pointer_to_token,zzxcf_map&identifier_map)
	{
		if(!is_token_identifier(*pointer_to_token))
		{
			return false;
		}
		zzxcf_map::iterator it=identifier_map.find(*pointer_to_token);
		if(it!=identifier_map.end())
		{
			if(it->second.operation==eReplace)
			{
				*pointer_to_token=it->second.replacement;
				return true;
			}
		}
		return false;
	}
	void replace_identifier_without_head(zzxcf_phrase::iterator&it,std::string&head,format_utility_namespace::zzxcf_map&identifier_map)
	{
		if((*it).length()==0||head.length()==0)
		{
			return;
		}
		if((*it).find(head)!=0)
		{
			return;
		}
		std::string temp1=head;
		std::string temp2=(*it).substr(head.length());
		temp2=replace_identifier_in_raw_phrase(temp2,identifier_map);
		*it=temp1+temp2;
	}
	void replace_identifier_without_head_and_foot(zzxcf_phrase::iterator&it,std::string&head,std::string&foot,format_utility_namespace::zzxcf_map&identifier_map)
	{
		if((*it).length()==0||head.length()==0||foot.length()==0)
		{
			return;
		}
		if((*it).find(head)!=0)
		{
			return;
		}
		if((*it).substr((*it).length()-foot.length()).find(foot)!=0)
		{
			return;
		}
		std::string temp1=head;
		std::string temp2=(*it).substr(head.length(),(*it).length()-head.length()-foot.length());
		std::string temp3=foot;
		temp2=replace_identifier_in_raw_phrase(temp2,identifier_map);
		*it=temp1+temp2+temp3;
	}
	std::string replace_identifier_in_raw_phrase(std::string&raw_phrase,format_utility_namespace::zzxcf_map&identifier_map)
	{
		zzxcf_phrase phrase;
		tokenize(raw_phrase,phrase);
		std::string new_raw_phrase;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(is_token_identifier(*it))
			{
				replace_identifier(it,identifier_map);
			}
			else if(is_token_directive_define(*it))
			{
				std::string head="#";
				replace_identifier_without_head(it,head,identifier_map);
			}
			else if(is_token_cpp_style_comment(*it))
			{
				std::string head="//";
				replace_identifier_without_head(it,head,identifier_map);
			}
			else if(is_token_c_style_comment(*it))
			{
				std::string head="/*";
				std::string foot="*/";
				replace_identifier_without_head_and_foot(it,head,foot,identifier_map);
			}
			new_raw_phrase+=(*it);
		}
		return new_raw_phrase;
	}
	std::string beautify_class_name_of_autodesk_frame_generator(std::string&class_name)
	{
		std::string new_class_name=class_name;
		bool is_interface=false;
		if(new_class_name.length()>1&&new_class_name[0]=='I'&&isupper(new_class_name[1]))
		{
			new_class_name.erase(0,1);
			is_interface=true;
		}
		if(new_class_name.length()>1&&new_class_name[0]=='C'&&isupper(new_class_name[1]))
		{
			new_class_name.erase(0,1);
		}
		if(new_class_name.find("AFG")==0)
		{
			new_class_name.erase(0,3);
		}
		if(new_class_name.find("FG")==0)
		{
			new_class_name.erase(0,2);
		}
		if(new_class_name.find("Afg")==std::string::npos)
		{
			new_class_name.insert(0,"Afg");
		}
		if(is_interface)
		{
			new_class_name.insert(0,"I");
		}
		return new_class_name;
	}
	int is_class_name_of_autodesk_frame_generator(std::string&class_name)
	{
		return class_name.length()>0&&isupper(class_name[0]);
	}
	std::string append_string_with_number(std::string&identifier,int i)
	{
		std::string unique_identifier=identifier;
		char buffer[100];
		sprintf_s(buffer,"__%d__",i);
		unique_identifier+=buffer;
		return unique_identifier;
	}
	std::string generate_unique_identifier(zzxcf_map&identifier_map,std::string&identifier)
	{
		int i=0;
		zzxcf_map::iterator it=identifier_map.find(identifier);
		while(it!=identifier_map.end())
		{
			std::string unique_identifier=append_string_with_number(identifier,i);
			it=identifier_map.find(unique_identifier);
			if(it==identifier_map.end())
			{
				return unique_identifier;
			}
			i++;
		}
		return identifier;
	}
	void take_out_redundant_white_spaces(zzxcf_section&section)
	{
		if(section.size()==0)
		{
			return;
		}
		enum State
		{
			Other,Word,Operator,SpaceAfterOperator
		};
		zzxcf_section tsl;
		for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
		{
			const zzxcf_phrase&phrase=(*it);
			if(phrase.size()==0)
			{
				continue;
			}
			State state=Other;
			zzxcf_phrase ts;
			for(zzxcf_phrase::const_iterator iter=phrase.begin();iter!=phrase.end();iter++)
			{
				const zzxcf_token&token=(*iter);
				if(token.length()==0)
				{
					continue;
				}
				if(is_char_utf8_bom(token[0]))
				{
					zzxcf_phrase phrase;
					phrase.push_back(token);
					tsl.push_back(phrase);
					continue;
				}
				switch(state)
				{
				case Other:
					if(is_token_identifier(token))
					{
						ts.push_back(token);
						state=Word;
					}
					else if(is_token_comment(token))
					{
						if(!command_line_namespace::option_strip_comments.is_turned_on())
						{
							zzxcf_phrase phrase;
							phrase.push_back(token);
							tsl.push_back(phrase);
						}
					}
					else if(!is_token_white_space(token))
					{
						ts.push_back(token);
						state=Operator;
					}
					break;
				case Word:
					if(is_token_identifier(token))
					{
						ts.push_back(" ");
						ts.push_back(token);
					}
					else if(is_token_comment(token))
					{
						if(!command_line_namespace::option_strip_comments.is_turned_on())
						{
							zzxcf_phrase phrase;
							phrase.push_back(token);
							tsl.push_back(phrase);
						}
					}
					else if(!is_token_white_space(token))
					{
						ts.push_back(token);
						state=Operator;
					}
					break;
				case Operator:
					if(is_token_white_space(token))
					{
						state=SpaceAfterOperator;
					}
					else if(is_token_comment(token))
					{
						if(!command_line_namespace::option_strip_comments.is_turned_on())
						{
							zzxcf_phrase phrase;
							phrase.push_back(token);
							tsl.push_back(phrase);
						}
						state=SpaceAfterOperator;
					}
					else if(is_token_identifier(token))
					{
						ts.push_back(token);
						state=Word;
					}
					else
					{
						ts.push_back(token);
					}
					break;
				case SpaceAfterOperator:
					if(is_token_identifier(token))
					{
						ts.push_back(token);
						state=Word;
					}
					else if(is_token_comment(token))
					{
						if(!command_line_namespace::option_strip_comments.is_turned_on())
						{
							zzxcf_phrase phrase;
							phrase.push_back(token);
							tsl.push_back(phrase);
						}
					}
					else if(!is_token_white_space(token))
					{
						ts.push_back(" ");
						ts.push_back(token);
						state=Operator;
					}
					break;
				}
			}
			if(ts.size()>0)
			{
				tsl.push_back(ts);
			}
		}
		section=tsl;
		if(!command_line_namespace::option_do_not_strip_redundant_white_spaces.is_turned_on())
		{
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
				{
					if(is_token_white_space(*iter))
					{
						zzxcf_phrase::iterator preIter=iter;
						preIter--;
						if(preIter!=(*it).end())
						{
							if(is_token_stand_alone(*preIter))
							{
								(*it).erase(iter);
								iter=preIter;
								continue;
							}
						}
						zzxcf_phrase::iterator latterIter=iter;
						latterIter++;
						if(latterIter!=(*it).end())
						{
							if(is_token_stand_alone(*latterIter))
							{
								(*it).erase(iter);
								iter=latterIter;
								continue;
							}
						}
						if(latterIter!=(*it).end()&&preIter!=(*it).end())
						{
							if(*latterIter=="*"&&*preIter=="*")
							{
								(*it).erase(iter);
								iter=latterIter;
								continue;
							}
						}
						if((preIter!=(*it).end())&&(!is_token_identifier(*preIter))||(latterIter!=(*it).end())&&(!is_token_identifier(*latterIter)))
						{
#ifdef _DEBUG
							fprintf(stderr,"No rule cover this white space case, %s%s%s\n",preIter->c_str(),iter->c_str(),latterIter->c_str());
#endif
						}
					}
				}
			}
		}
		if(!command_line_namespace::option_do_not_strip_line_feed_between_right_curly_bracket_and_semicolon.is_turned_on())
		{
			tsl.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				if(is_phrase_semicolon(*it))
				{
					if(tsl.size()>0)
					{
						zzxcf_section::iterator iter=tsl.end();
						iter--;
						if(is_phrase_right_curly_bracket(*iter))
						{
							for(zzxcf_phrase::iterator iterat=(*it).begin();iterat!=(*it).end();iterat++)
							{
								(*iter).push_back(*iterat);
							}
						}
						else
						{
							tsl.push_back(*it);
						}
					}
					else
					{
						tsl.push_back(*it);
					}
				}
				else
				{
					tsl.push_back(*it);
				}
			}
			section=tsl;
		}
		if(!command_line_namespace::option_do_not_add_line_feed_after_single_colon.is_turned_on())
		{
			tsl.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				if(is_phrase_contains_token_single_colon(*it))
				{
					zzxcf_phrase phrase=(*it);
					while(is_phrase_contains_token_single_colon(phrase))
					{
						zzxcf_phrase ts;
						zzxcf_phrase::iterator iter=phrase.begin();
						for(;(iter!=phrase.end())&&(*iter!=":");iter++)
						{
							ts.push_back(*iter);
						}
						if(iter!=phrase.end())
						{
							ts.push_back(*iter);
							iter++;
						}
						if(iter!=phrase.end())
						{
							if(is_token_digit(*iter))
							{
								ts.push_back(*iter);
								iter++;
							}
						}
						while(iter!=phrase.end())
						{
							if((*iter)==","||(*iter)==";"||(*iter)==")")
							{
								ts.push_back(*iter);
								iter++;
							}
							else
							{
								break;
							}
						}
						if(ts.size()>0)
						{
							tsl.push_back(ts);
							ts.clear();
						}
						if(iter!=phrase.end())
						{
							for(;iter!=phrase.end();iter++)
							{
								ts.push_back(*iter);
							}
						}
						phrase=ts;
						if(phrase.size()==0)
						{
							break;
						}
					}
					if(phrase.size()>0)
					{
						tsl.push_back(phrase);
						phrase.clear();
					}
				}
				else
				{
					tsl.push_back(*it);
				}
			}
			section=tsl;
		}
		if(!command_line_namespace::option_do_not_add_line_feed_after_template_declaration.is_turned_on())
		{
			tsl.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				if(is_phrase_template(*it))
				{
					enum State
					{
						Before,In,After
					};
					State state=Before;
					int count=0;
					zzxcf_phrase ts;
					for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
					{
						switch(state)
						{
						case Before:
							if(*iter=="<")
							{
								count++;
								ts.push_back(*iter);
								state=In;
							}
							else
							{
								ts.push_back(*iter);
							}
							break;
						case In:
							if(*iter=="<")
							{
								count++;
								ts.push_back(*iter);
							}
							else if(*iter==">")
							{
								count--;
								ts.push_back(*iter);
								if(count==0)
								{
									state=After;
									if(ts.size()>0)
									{
										tsl.push_back(ts);
										ts.clear();
									}
								}
							}
							else
							{
								ts.push_back(*iter);
							}
							break;
						case After:
							ts.push_back(*iter);
							break;
						}
					}
					if(ts.size()>0)
					{
						tsl.push_back(ts);
						ts.clear();
					}
				}
				else
				{
					tsl.push_back(*it);
				}
			}
			section=tsl;
		}
		if(!command_line_namespace::option_do_not_add_line_feed_after_function_call_alike.is_turned_on())
		{
			tsl.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				enum State
				{
					Other,Identifier,ParenthesisLeft,ParenthesisRight
				};
				State state=Other;
				int count=0;
				zzxcf_phrase ts;
				for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
				{
					switch(state)
					{
					case Other:
						if(is_token_identifier(*iter))
						{
							if(*iter=="return")
							{
							}
							else if(*iter=="new")
							{
							}
							else if(*iter=="while")
							{
							}
							else if(*iter=="decltype")
							{
							}
							else
							{
								state=Identifier;
							}
						}
						ts.push_back(*iter);
						break;
					case Identifier:
						if(*iter=="(")
						{
							count++;
							ts.push_back(*iter);
							state=ParenthesisLeft;
						}
						else
						{
							iter--;
							state=Other;
						}
						break;
					case ParenthesisLeft:
						if(*iter=="(")
						{
							count++;
							ts.push_back(*iter);
						}
						else if(*iter==")")
						{
							count--;
							ts.push_back(*iter);
							if(count==0)
							{
								state=ParenthesisRight;
							}
						}
						else
						{
							ts.push_back(*iter);
						}
						break;
					case ParenthesisRight:
						if(is_token_identifier(*iter))
						{
							if(*iter=="const")
							{
							}
							else if(*iter=="throw")
							{
							}
							else if(ts.size()>0)
							{
								tsl.push_back(ts);
								ts.clear();
							}
							iter--;
							state=Other;
						}
						else
						{
							ts.push_back(*iter);
							state=Other;
						}
						break;
					}
				}
				if(ts.size()>0)
				{
					tsl.push_back(ts);
					ts.clear();
				}
			}
			section=tsl;
		}
		if(!command_line_namespace::option_do_not_add_line_feed_if_more_than_10_commas_in_one_line.is_turned_on())
		{
			tsl.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				if(is_phrase_contains_number_of_token_commas(*it))
				{
					enum State
					{
						Before,In
					};
					State state=Before;
					int count=0;
					zzxcf_phrase ts;
					for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
					{
						switch(state)
						{
						case Before:
							if(*iter=="(")
							{
								count++;
								ts.push_back(*iter);
								state=In;
							}
							else
							{
								ts.push_back(*iter);
								if(*iter==",")
								{
									if(ts.size()>0)
									{
										tsl.push_back(ts);
										ts.clear();
									}
								}
							}
							break;
						case In:
							if(*iter=="(")
							{
								count++;
								ts.push_back(*iter);
							}
							else if(*iter==")")
							{
								count--;
								ts.push_back(*iter);
							}
							else
							{
								ts.push_back(*iter);
								if(*iter==","&&count==0)
								{
									if(ts.size()>0)
									{
										tsl.push_back(ts);
										ts.clear();
										state=Before;
									}
								}
							}
							break;
						}
					}
					if(ts.size()>0)
					{
						tsl.push_back(ts);
						ts.clear();
						state=Before;
					}
				}
				else
				{
					tsl.push_back(*it);
				}
			}
			section=tsl;
		}
		if(true)
		{
			tsl.clear();
			for(zzxcf_section::iterator it=section.begin();it!=section.end();it++)
			{
				if(does_contain_quoted_string(*it))
				{
					zzxcf_phrase ts;
					for(zzxcf_phrase::iterator iter=(*it).begin();iter!=(*it).end();iter++)
					{
						if(is_token_quoted_string(*iter))
						{
							ts.push_back(*iter);
							zzxcf_phrase::iterator iter1=iter;
							iter1++;
							if(iter1!=(*it).end())
							{
								if(is_token_identifier(*iter1))
								{
									tsl.push_back(ts);
									ts.clear();
								}
							}
						}
						else
						{
							ts.push_back(*iter);
						}
					}
					if(ts.size()>0)
					{
						tsl.push_back(ts);
						ts.clear();
					}
				}
				else
				{
					tsl.push_back(*it);
				}
			}
			section=tsl;
			tsl.clear();
		}
		if(true)
		{
			tsl.clear();
			for(zzxcf_section::const_iterator it=section.begin();it!=section.end();it++)
			{
				if(is_token_left_square_bracket(*(*it).begin()))
				{
					zzxcf_phrase ts;
					for(zzxcf_phrase::const_iterator iter=(*it).begin();iter!=(*it).end();iter++)
					{
						if(is_token_right_square_bracket(*iter))
						{
							ts.push_back(*iter);
							zzxcf_phrase::const_iterator iter1=iter;
							iter1++;
							if(iter1!=(*it).end())
							{
								zzxcf_phrase::const_iterator iter2=iter;
								iter2--;
								if(!is_token_left_square_bracket(*iter2)&&(is_token_identifier(*iter1)||is_token_left_square_bracket(*iter1)))
								{
									tsl.push_back(ts);
									ts.clear();
								}
							}
						}
						else
						{
							ts.push_back(*iter);
						}
					}
					if(ts.size()>0)
					{
						tsl.push_back(ts);
						ts.clear();
					}
				}
				else
				{
					tsl.push_back(*it);
				}
			}
			section=tsl;
			tsl.clear();
		}
	}
	int regroup(zzxcf_phrase&phrase,std::string&content,std::string interim_result_file_name)
	{
		zzxcf_section section;
		if(!command_line_namespace::option_do_not_strip_specific_conditional_complilation.is_turned_on())
		{
			class_decorate_strip_specific_conditional_compilation strip_specific_conditional_compilation(interim_result_file_name);
			strip_specific_conditional_compilation.process(phrase);
		}
		if(command_line_namespace::option_strip_utf8_bom.is_turned_on())
		{
			class_decorate_remove_utf8_bom remove_utf8_bom(interim_result_file_name);
			remove_utf8_bom.process(phrase);
		}
		if(command_line_namespace::option_strip_comments.is_turned_on())
		{
			Modifier_replace_cpp_comment_with_space m;
			m.Modify(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".strip_comments");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
				//file_operation_namespace::write_file(file_operation_namespace::generate_unique_file_name(interim_result_file_name,".comments"),strip_comments.get_comments());
			}
		}
		else if(!command_line_namespace::option_do_not_strip_meaningless_comments.is_turned_on())
		{
			class_decorate_strip_meaningless_comments strip_meaningless_comments;
			strip_meaningless_comments.process(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".strip_meaningless_comments");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
				file_operation_namespace::write_file(file_operation_namespace::generate_unique_file_name(interim_result_file_name,".meaningless_comments"),strip_meaningless_comments.get_meaningless_comments());
			}
		}
		if(!command_line_namespace::option_do_not_strip_meaningless_pragmas.is_turned_on())
		{
			class_decorate_strip_meaningless_pragmas strip_meaningless_pragmas;
			strip_meaningless_pragmas.process(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".strip_meaningless_pragmas");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
				file_operation_namespace::write_file(file_operation_namespace::generate_unique_file_name(interim_result_file_name,".meaningless_pragmas"),strip_meaningless_pragmas.get_meaningless_pragmas());
			}
		}
		if(!command_line_namespace::option_do_not_convert_path_separator_from_windows_style_backslash_to_linux_style_slash_in_header_file_include_directive.is_turned_on())
		{
			Modifier_convert_include_header_path_separator_from_backslash_to_slash backslash2slash;
			backslash2slash.Modify(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".convert_path_separator_from_windows_style_backslash_to_linux_style_slash_in_header_file_include_directive");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
			}
		}
		if(!command_line_namespace::option_do_not_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket.is_turned_on())
		{
			class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket;
			surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket.process(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
			}
		}
		if(command_line_namespace::option_surround_if_clause_after_reserve_word_else_with_curly_bracket.is_turned_on())
		{
			class_decorate_surround_if_clause_after_reserve_word_else_with_curly_bracket surround_if_clause_after_reserve_word_else_with_curly_bracket;
			surround_if_clause_after_reserve_word_else_with_curly_bracket.process(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".surround_if_clause_after_reserve_word_else_with_curly_bracket");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
			}
		}
		if(!command_line_namespace::option_do_not_surround_single_sentence_after_reserve_word_for_with_curly_bracket.is_turned_on())
		{
			class_decorate_surround_single_sentence_after_reserve_word_for_with_curly_bracket surround_single_sentence_after_reserve_word_for_with_curly_bracket;
			surround_single_sentence_after_reserve_word_for_with_curly_bracket.process(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".surround_single_sentence_after_reserve_word_for_with_curly_bracket");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
			}
		}
		if(!command_line_namespace::option_do_not_strip_duplicate_semicolon.is_turned_on())
		{
			class_decorate_strip_duplicate_semicolon strip_duplicate_semicolon;
			strip_duplicate_semicolon.process(phrase);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".strip_duplicate_semicolon");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
			}
		}
		int left_curly_bracket_count=0;
		if(!command_line_namespace::option_do_not_apply_new_alignment.is_turned_on())
		{
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				section.clear();
				section.push_back(phrase);
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".before_apply_new_alignment");
				write_section_to_file(section,interim_result_file_name);
				section.clear();
			}
			concat_tokens_0separator(phrase,section);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".concat_tokens_0separator");
				write_section_to_file(section,interim_result_file_name);
			}
			concat_tokens_2round_bracket(section);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".concat_tokens_2round_bracket");
				write_section_to_file(section,interim_result_file_name);
			}
			concat_tokens_3sentence(section);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".concat_tokens_3sentence");
				write_section_to_file(section,interim_result_file_name);
			}
			take_out_redundant_white_spaces(section);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".take_out_redundant_white_spaces");
				write_section_to_file(section,interim_result_file_name);
			}
			if(command_line_namespace::option_replace_ifndef_define_endif_directives_with_pragma_once.is_turned_on())
			{
				replace_ifndef_define_endif_directives_with_pragma_once(section);
				if(command_line_namespace::option_save_interim_result.is_turned_on())
				{
					interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".replace_ifndef_define_endif_directives_with_pragma_once");
					write_section_to_file(section,interim_result_file_name);
				}
			}
			if(command_line_namespace::option_accumulate_map.is_turned_on())
			{
				accumulate_map_comment(section,comment_map);
				accumulate_map_identifier(section,identifier_map,inverse_identifier_map);
				accumulate_map_directive(section,directive_map,directive_list);
			}
			left_curly_bracket_count=concat_tokens_6indent(section);
			if(command_line_namespace::option_save_interim_result.is_turned_on())
			{
				interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".concat_tokens_6indent");
				write_section_to_file(section,interim_result_file_name);
			}
		}
		else
		{
			section.clear();
			section.push_back(phrase);
		}
		for(zzxcf_section::iterator it1=section.begin();it1!=section.end();it1++)
		{
			for(zzxcf_phrase::iterator it2=(*it1).begin();it2!=(*it1).end();it2++)
			{
				content+=it2->c_str();
			}
		}
		return left_curly_bracket_count;
	}
	void retry_format_files(const char**file_paths,size_t file_paths_length)
	{
		for(size_t i=0;i<file_paths_length;i++)
		{
			if(file_paths[i])
			{
				fprintf(stdout,"Retry: %s\n",file_paths[i]);
				class_file_formatter file_formatter(file_paths[i]);
			}
		}
	}
	void format_files_in_process(const std::list<std::string>&file_paths)
	{
		size_t number_of_files=file_paths.size();
		size_t number_of_formatted_files=0;
		for(std::list<std::string>::const_iterator it=file_paths.begin();it!=file_paths.end();it++)
		{
			console_utility_namespace::print_progress(number_of_files,number_of_formatted_files);
			class_file_formatter formatter(*it);
			number_of_formatted_files++;
			console_utility_namespace::print_progress(number_of_files,number_of_formatted_files);
		}
	}
	unsigned long __stdcall format_files_in_thread(void*file_paths)
	{
		class_file_formatter formatter((const char**)file_paths);
		if(!command_line_namespace::option_do_not_print_statistical_data.is_turned_on())
		{
			statistic_utility_namespace::print_thread_statistic();
		}
		if(command_line_namespace::option_notify_user_when_job_done.is_turned_on())
		{
			statistic_utility_namespace::beep();
		}
		return 0;
	}
	unsigned long __stdcall print_progress_in_thread(void*p_progress_data)
	{
		const char**file_paths=((struct_progress_data*)p_progress_data)->file_paths;
		size_t number_of_files=((struct_progress_data*)p_progress_data)->number_of_files;
		while(true)
		{
			size_t number_of_formatted_files=0;
			size_t i=0;
			while(i<number_of_files)
			{
				if(file_paths[i]==0)
				{
					number_of_formatted_files++;
				}
				i++;
			}
			console_utility_namespace::print_progress(number_of_files,number_of_formatted_files);
			Sleep(100);
		}
		return 0;
	}
	HANDLE startup_print_progress_thread(struct_progress_data*p_progress_data)
	{
		return CreateThread(0,0,print_progress_in_thread,p_progress_data,0,0);
	}
	void startup_format_files_threads(const char**file_paths,size_t number_of_files,HANDLE*thread_handles,size_t&thread_handles_length)
	{
		size_t number_of_files_per_thread=namespace_multithread::get_number_of_files_per_thread(number_of_files,thread_handles_length);
		size_t thread_handle_index=0;
		size_t file_path_index=thread_handle_index*number_of_files_per_thread;
		while(thread_handle_index<thread_handles_length&&file_path_index<number_of_files)
		{
			thread_handles[thread_handle_index]=CreateThread(0,0,format_files_in_thread,&(file_paths[file_path_index]),0,0);
			thread_handle_index++;
			file_path_index=thread_handle_index*number_of_files_per_thread;
		}
		thread_handles_length=thread_handle_index;
	}
	void format_files_in_threads(const std::list<std::string>&file_path_list)
	{
		size_t number_of_files=file_path_list.size();
		const char**file_paths=new const char*[number_of_files+1];
		namespace_multithread::initialize_file_paths(file_paths,number_of_files,file_path_list);
		struct_progress_data progress_data;
		progress_data.file_paths=file_paths;
		progress_data.number_of_files=number_of_files;
		HANDLE print_progress_thread_handle=startup_print_progress_thread(&progress_data);
		size_t number_of_threads=namespace_multithread::get_number_of_threads();
		HANDLE*thread_handles=new HANDLE[number_of_threads];
		startup_format_files_threads(file_paths,number_of_files,thread_handles,number_of_threads);
		::WaitForMultipleObjects((DWORD)number_of_threads,thread_handles,TRUE,INFINITE);
		retry_format_files(file_paths,number_of_files);
		namespace_multithread::printf_file_paths(file_paths,number_of_files);
		::TerminateThread(print_progress_thread_handle,0);
		::CloseHandle(print_progress_thread_handle);
		delete[]file_paths;
		delete[]thread_handles;
	}
	void initialize_sqlite3_functions_details(const std::string fileName="functionsdetails.db")
	{
		if(pFunctionsDetailsDatabase==0)
		{
			sqlite3_open(fileName.c_str(),&pFunctionsDetailsDatabase);
		}
		if(pFunctionsDetailsStatementCreateTable==0)
		{
			sqlite3_prepare_v2(pFunctionsDetailsDatabase,sqlFunctionsDetailsCreateTable.c_str(),(int)sqlFunctionsDetailsCreateTable.length(),&pFunctionsDetailsStatementCreateTable,0);
		}
		sqlite3_step(pFunctionsDetailsStatementCreateTable);
		if(pFunctionsDetailsStatementInsertTable==0)
		{
			sqlite3_prepare_v2(pFunctionsDetailsDatabase,sqlFunctionsDetailsInsertTable.c_str(),(int)sqlFunctionsDetailsInsertTable.length(),&pFunctionsDetailsStatementInsertTable,0);
		}
		sqlite3_exec(pFunctionsDetailsDatabase,"begin;",0,0,0);
	}
	void finalize_sqlite3_functions_details()
	{
		sqlite3_exec(pFunctionsDetailsDatabase,"commit;",0,0,0);
		sqlite3_finalize(pFunctionsDetailsStatementCreateTable);
		pFunctionsDetailsStatementCreateTable=0;
		sqlite3_finalize(pFunctionsDetailsStatementInsertTable);
		pFunctionsDetailsStatementInsertTable=0;
		sqlite3_close(pFunctionsDetailsDatabase);
		pFunctionsDetailsDatabase=0;
	}
	void print_functions(const std::list<std::string>&functions)
	{
		for(std::list<std::string>::const_iterator it=functions.begin();it!=functions.end();it++)
		{
			fprintf(stderr,"%s\n",it->c_str());
		}
	}
	void extract_functions(const zzxcf_phrase&phrase,std::list<std::string>&functions)
	{
		enum State
		{
			Other,ReturnType,Space,ClassName,DoubleColonScopeResolutionOperator,MethodName,BracketLeft,BracketRight,CurlyBracketLeft,CurlyBracketRight
		};
		for(zzxcf_phrase::const_iterator it=phrase.begin();it!=phrase.end();it++)
		{
			State state=Other;
			int bracketCount=1;
			std::string nameOfFunction;
			for(zzxcf_phrase::const_iterator iter=it;iter!=phrase.end();iter++)
			{
				switch(state)
				{
				case Other:
					if(is_token_identifier(*iter))
					{
						state=ReturnType;
						it=iter;
					}
					break;
				case ReturnType:
					if(is_token_white_space_comment_directive(*iter)||(*iter)=="*"||(*iter)=="&")
					{
						state=Space;
					}
					else
					{
						state=Other;
					}
					break;
				case Space:
					if(is_token_white_space_comment_directive(*iter)||(*iter)=="*"||(*iter)=="&")
					{
						state=Space;
					}
					else if(is_token_identifier(*iter))
					{
						state=ClassName;
						nameOfFunction=(*iter);
					}
					else
					{
						state=Other;
					}
					break;
				case ClassName:
					if(is_token_scope_resolution_operator(*iter))
					{
						state=DoubleColonScopeResolutionOperator;
						nameOfFunction+=(*iter);
					}
					else if(is_token_left_round_bracket(*iter))
					{
						state=BracketLeft;
					}
					else if(is_token_white_space_comment_directive(*iter))
					{
						state=ClassName;
					}
					else
					{
						state=Other;
					}
					break;
				case DoubleColonScopeResolutionOperator:
					if(is_token_scope_resolution_operator(*iter))
					{
						state=DoubleColonScopeResolutionOperator;
						nameOfFunction+=(*iter);
					}
					else if(is_token_identifier(*iter))
					{
						state=MethodName;
						nameOfFunction+=(*iter);
					}
					else
					{
						state=Other;
					}
					break;
				case MethodName:
					if(is_token_white_space_comment_directive(*iter))
					{
						state=MethodName;
					}
					else if(is_token_left_round_bracket(*iter))
					{
						state=BracketLeft;
					}
					else
					{
						state=Other;
					}
					break;
				case BracketLeft:
					if(is_token_left_round_bracket(*iter))
					{
						bracketCount++;
					}
					else if(is_token_right_round_bracket(*iter))
					{
						bracketCount--;
					}
					if(bracketCount==0)
					{
						state=BracketRight;
					}
					break;
				case BracketRight:
					if(is_token_left_curly_bracket(*iter))
					{
						state=CurlyBracketLeft;
						bracketCount=1;
					}
					else if(is_token_white_space_comment_directive(*iter))
					{
						state=BracketRight;
					}
					else
					{
						state=Other;
					}
					break;
				case CurlyBracketLeft:
					if(is_token_left_curly_bracket(*iter))
					{
						bracketCount++;
					}
					else if(is_token_right_curly_bracket(*iter))
					{
						bracketCount--;
					}
					if(bracketCount==0)
					{
						std::string function;
						for(zzxcf_phrase::const_iterator iterat=it;iterat!=iter;iterat++)
						{
							function+=(*iterat);
						}
						function+=(*iter);
						functions.push_back(function);
						it=iter;
						state=Other;
					}
					break;
				}
				if(state==Other)
				{
					break;
				}
			}
		}
	}
	std::string format_function(const std::string&function)
	{
		zzxcf_phrase phrase;
		tokenize(function,phrase);
		std::string formattedFunction;
		format_utility_namespace::regroup(phrase,formattedFunction);
		return formattedFunction;
	}
	void format_functions(std::list<std::string>&functions)
	{
		for(std::list<std::string>::iterator it=functions.begin();it!=functions.end();it++)
		{
			(*it)=format_function(*it);
		}
	}
	size_t get_function_line_count(const std::string&function)
	{
		return file_operation_namespace::count_number_of_line_feeds(function);
	}
	void extract_function_details(const std::string&function,std::string&typeOfReturn,std::string&nameOfFunction,std::string&declarationOfFunction)
	{
		enum State
		{
			Other,ReturnType,Space,ClassName,DoubleColonScopeResolutionOperator,MethodName,BracketLeft,BracketRight,CurlyBracketLeft,CurlyBracketRight
		};
		zzxcf_phrase phrase;
		tokenize(function,phrase);
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			State state=Other;
			int bracketCount=1;
			for(zzxcf_phrase::iterator iter=it;iter!=phrase.end();iter++)
			{
				switch(state)
				{
				case Other:
					if(is_token_identifier(*iter))
					{
						state=ReturnType;
						it=iter;
						typeOfReturn=(*iter);
						declarationOfFunction=(*iter);
					}
					break;
				case ReturnType:
					if(is_token_white_space_comment_directive(*iter)||(*iter)=="*"||(*iter)=="&")
					{
						state=Space;
						declarationOfFunction+=(*iter);
					}
					else
					{
						state=Other;
					}
					break;
				case Space:
					if(is_token_white_space_comment_directive(*iter)||(*iter)=="*"||(*iter)=="&")
					{
						state=Space;
						declarationOfFunction+=(*iter);
					}
					else if(is_token_identifier(*iter))
					{
						state=ClassName;
						nameOfFunction=(*iter);
						declarationOfFunction+=(*iter);
					}
					else
					{
						state=Other;
					}
					break;
				case ClassName:
					if(is_token_scope_resolution_operator(*iter))
					{
						state=DoubleColonScopeResolutionOperator;
						nameOfFunction+=(*iter);
						declarationOfFunction+=(*iter);
					}
					else if(is_token_left_round_bracket(*iter))
					{
						state=BracketLeft;
						declarationOfFunction+=(*iter);
					}
					else if(is_token_white_space_comment_directive(*iter))
					{
						state=ClassName;
						declarationOfFunction+=(*iter);
					}
					else
					{
						state=Other;
					}
					break;
				case DoubleColonScopeResolutionOperator:
					if(is_token_scope_resolution_operator(*iter))
					{
						state=DoubleColonScopeResolutionOperator;
						nameOfFunction+=(*iter);
						declarationOfFunction+=(*iter);
					}
					else if(is_token_identifier(*iter))
					{
						state=MethodName;
						nameOfFunction+=(*iter);
						declarationOfFunction+=(*iter);
					}
					else
					{
						state=Other;
					}
					break;
				case MethodName:
					if(is_token_white_space_comment_directive(*iter))
					{
						state=MethodName;
						declarationOfFunction+=(*iter);
					}
					else if(is_token_left_round_bracket(*iter))
					{
						state=BracketLeft;
						declarationOfFunction+=(*iter);
					}
					else
					{
						state=Other;
					}
					break;
				case BracketLeft:
					if(is_token_left_round_bracket(*iter))
					{
						bracketCount++;
					}
					else if(is_token_right_round_bracket(*iter))
					{
						bracketCount--;
					}
					declarationOfFunction+=(*iter);
					if(bracketCount==0)
					{
						state=BracketRight;
					}
					break;
				case BracketRight:
					if(is_token_left_curly_bracket(*iter))
					{
						state=CurlyBracketLeft;
						bracketCount=1;
					}
					else if(is_token_white_space_comment_directive(*iter))
					{
						state=BracketRight;
					}
					else
					{
						state=Other;
					}
					break;
				case CurlyBracketLeft:
					if(is_token_left_curly_bracket(*iter))
					{
						bracketCount++;
					}
					else if(is_token_right_curly_bracket(*iter))
					{
						bracketCount--;
					}
					if(bracketCount==0)
					{
						std::string functionStr;
						for(zzxcf_phrase::iterator iterat=it;iterat!=iter;iterat++)
						{
							functionStr+=(*iterat);
						}
						functionStr+=(*iter);
						it=iter;
						state=Other;
					}
					break;
				}
				if(state==Other)
				{
					break;
				}
			}
		}
	}
	void extract_functions_details(const std::string&fileName,const std::string&fileContent,const std::list<std::string>&functions,FunctionsDetails&functionsDetails)
	{
		for(std::list<std::string>::const_iterator it=functions.begin();it!=functions.end();it++)
		{
			struct_function_details functionDetails;
			extract_function_details(*it,functionDetails.function_return_type,functionDetails.function_name,functionDetails.function_declaration);
			functionDetails.file_name=fileName;
			functionDetails.offset=fileContent.find(*it);
			functionDetails.number_of_lines=get_function_line_count(*it);
			functionDetails.function_body=command_line_namespace::option_format_before_saving_function.is_turned_on()?format_function(*it):(*it);
			functionsDetails.insert(make_pair(functionDetails.function_name,functionDetails));
		}
	}
	void save_functions_details_into_database(const FunctionsDetails&functionsDetails)
	{
		for(FunctionsDetails::const_iterator it=functionsDetails.begin();it!=functionsDetails.end();it++)
		{
			struct_function_details functionDetails=(*it).second;
			sqlite3_bind_text(pFunctionsDetailsStatementInsertTable,1,functionDetails.file_name.c_str(),(int)functionDetails.file_name.length(),0);
			sqlite3_bind_text(pFunctionsDetailsStatementInsertTable,2,functionDetails.function_name.c_str(),(int)functionDetails.function_name.length(),0);
			sqlite3_bind_text(pFunctionsDetailsStatementInsertTable,3,functionDetails.function_return_type.c_str(),(int)functionDetails.function_return_type.length(),0);
			sqlite3_bind_text(pFunctionsDetailsStatementInsertTable,4,functionDetails.function_declaration.c_str(),(int)functionDetails.function_declaration.length(),0);
			sqlite3_bind_text(pFunctionsDetailsStatementInsertTable,5,functionDetails.function_body.c_str(),(int)functionDetails.function_body.length(),0);
			sqlite3_bind_int(pFunctionsDetailsStatementInsertTable,6,(int)functionDetails.number_of_lines);
			sqlite3_bind_int(pFunctionsDetailsStatementInsertTable,7,(int)functionDetails.offset);
			if(sqlite3_step(pFunctionsDetailsStatementInsertTable)!=SQLITE_DONE)
			{
				fprintf(stderr,"Error: sqlite3 step '%s' failed.\n",sqlFunctionsDetailsInsertTable.c_str());
			}
			sqlite3_reset(pFunctionsDetailsStatementInsertTable);
		}
	}
	void save_functions_from_source_file(const std::string&fileName)
	{
		if(!file_operation_namespace::file_exists(fileName))
		{
			return;
		}
		std::string fileContent;
		file_operation_namespace::read_file(fileName,fileContent);
		zzxcf_phrase phrase;
		tokenize(fileContent,phrase);
		std::list<std::string>functions;
		extract_functions(phrase,functions);
		FunctionsDetails functionsDetails;
		extract_functions_details(fileName,fileContent,functions,functionsDetails);
		save_functions_details_into_database(functionsDetails);
	}
	void save_functions_from_source_files(const std::list<std::string>&fileList)
	{
		for(std::list<std::string>::const_iterator it=fileList.begin();it!=fileList.end();it++)
		{
			save_functions_from_source_file(*it);
			printf("%s\n",it->c_str());
		}
	}
	unsigned long __stdcall save_functions(void*files)
	{
		const char**_files=(const char**)files;
		while(*_files)
		{
			fprintf(stderr,"%s\n",*_files);
			save_functions_from_source_file(*_files);
			printf("%s\n",*_files);
			*_files=0;
			_files++;
		}
		return 0;
	}
	void startup_threads_for_saving_functions(const char**files,size_t numberOfFiles,HANDLE*threads,size_t&numberOfThreads)
	{
		size_t numberOfFilesPerThread=namespace_multithread::get_number_of_files_per_thread(numberOfFiles,numberOfThreads);
		size_t threadsIndex=0;
		size_t filesIndex=threadsIndex*numberOfFilesPerThread;
		while(threadsIndex<numberOfThreads&&filesIndex<numberOfFiles)
		{
			threads[threadsIndex]=CreateThread(0,0,save_functions,&(files[filesIndex]),0,0);
			threadsIndex++;
			filesIndex=threadsIndex*numberOfFilesPerThread;
		}
		numberOfThreads=threadsIndex;
	}
	void save_functions_using_threads(const std::list<std::string>&fileList)
	{
		size_t numberOfFiles=fileList.size();
		const char**files=new const char*[numberOfFiles+1];
		namespace_multithread::initialize_file_paths(files,numberOfFiles,fileList);
		size_t numberOfThreads=namespace_multithread::get_number_of_threads();
		HANDLE*threads=new HANDLE[numberOfThreads];
		startup_threads_for_saving_functions(files,numberOfFiles,threads,numberOfThreads);
		WaitForMultipleObjects((DWORD)numberOfThreads,threads,TRUE,INFINITE);
		namespace_multithread::printf_file_paths(files,numberOfFiles);
		delete[]files;
		delete[]threads;
	}
	std::string get_name_ext(const std::string&fileName)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char name[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(fileName.c_str(),drive,_MAX_DRIVE,dir,_MAX_DIR,name,_MAX_FNAME,ext,_MAX_EXT);
		std::string nameExtension=name;
		nameExtension+=ext;
		return nameExtension;
	}
	std::string generate_h_file_name_from_cpp_file_name(const std::string&fileName)
	{
		char path[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char name[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(fileName.c_str(),drive,_MAX_DRIVE,dir,_MAX_DIR,name,_MAX_FNAME,ext,_MAX_EXT);
		_makepath_s(path,drive,dir,name,".h");
		return std::string(path);
	}
	std::string generate_impl_file_name(const std::string&fileName)
	{
		char path[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char name[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(fileName.c_str(),drive,_MAX_DRIVE,dir,_MAX_DIR,name,_MAX_FNAME,ext,_MAX_EXT);
		std::string impl_name=name;
		impl_name+="Impl";
		_makepath_s(path,drive,dir,impl_name.c_str(),ext);
		return std::string(path);
	}
	void copy_file(const std::string&from,const std::string&to)
	{
		fprintf(stderr,"Copy file, file is %s\n",from.c_str());
		CopyFileA(from.c_str(),to.c_str(),true);
#ifdef _DEBUG
		int wait_count=50;
		while(!file_operation_namespace::file_exists(to)&&wait_count--)
		{
			perror(from.c_str());
			Sleep(200);
			fprintf(stderr,"Waiting for copying file, file is %s\n",from.c_str());
			CopyFileA(from.c_str(),to.c_str(),true);
		}
#endif
	}
	void let_user_decide_remove_copy(const std::string&file,const std::string&impl_file)
	{
		if(file_operation_namespace::file_exists(file))
		{
			if(file_operation_namespace::file_exists(impl_file))
			{
				if(true||MessageBoxA(GetConsoleWindow(),impl_file.c_str(),"Del",MB_YESNO)==IDYES)
				{
					file_operation_namespace::remove_file(impl_file);
				}
			}
			if(true||MessageBoxA(GetConsoleWindow(),file.c_str(),"Impl",MB_YESNO)==IDYES)
			{
				fprintf(stderr,"Impl %s\n",file.c_str());
				copy_file(file,impl_file);
			}
		}
	}
	void extract_classes(const zzxcf_phrase&phrase,std::list<std::string>&classList,std::list<class_info>&class_info_list)
	{
		enum State
		{
			Other,Class,DeclSpec,DeclSpecL,DeclSpecR,ClassName,SingleColon,ModifierAndInheritedClassNames,LeftCurlyBracket,RightCurlyBracket,Semicolon
		};
		State state=Other;
		std::string r;
		std::string class_name;
		std::string declspec_content;
		std::string modifier_and_inherited_class_names;
		size_t count_curly_bracket=0;
		for(zzxcf_phrase::const_iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(state!=Other)
			{
				r+=(*it);
			}
			if(is_token_white_space(*it)||is_token_comment(*it))
			{
				continue;
			}
			switch(state)
			{
			case Other:
				if(*it=="class")
				{
					r= *it;
					state=Class;
				}
				break;
			case Class:
				if(*it=="__declspec")
				{
					state=DeclSpec;
				}
				else
				{
					class_name= *it;
					state=ClassName;
				}
				break;
			case DeclSpec:
				if(*it=="(")
				{
					state=DeclSpecL;
				}
				else
				{
					printf("Error: %s %s?\n",r.c_str(),it->c_str());
					r="";
					state=Other;
				}
				break;
			case DeclSpecL:
				if(*it==")")
				{
					state=DeclSpecR;
				}
				else
				{
					declspec_content+= *it;
				}
				break;
			case DeclSpecR:
				if(is_token_identifier(*it))
				{
					class_name= *it;
					state=ClassName;
				}
				else
				{
					printf("Error: %s %s?\n",r.c_str(),it->c_str());
					r="";
					state=Other;
				}
				break;
			case ClassName:
				if(*it==";")
				{
					classList.push_back(r);
					state=Other;
					fprintf(stderr,"%s,%s,%s\n%s\n",class_name.c_str(),declspec_content.c_str(),modifier_and_inherited_class_names.c_str(),r.c_str());
					class_info ci;
					ci.body=r;
					ci.class_name=class_name;
					ci.declspec_content=declspec_content;
					ci.modifier_and_inherited_class_names=modifier_and_inherited_class_names;
					class_info_list.push_back(ci);
					r="";
					class_name="";
					declspec_content="";
					modifier_and_inherited_class_names="";
					count_curly_bracket=0;
				}
				else if(*it==":")
				{
					state=SingleColon;
				}
				else if(*it=="{")
				{
					count_curly_bracket=1;
					state=LeftCurlyBracket;
				}
				else
				{
					printf("Error: %s %s?\n",r.c_str(),it->c_str());
					r="";
					state=Other;
				}
				break;
			case SingleColon:
				if(is_token_identifier(*it))
				{
					modifier_and_inherited_class_names= *it;
					state=ModifierAndInheritedClassNames;
				}
				else
				{
					printf("Error: %s %s?\n",r.c_str(),it->c_str());
					r="";
					state=Other;
				}
				break;
			case ModifierAndInheritedClassNames:
				if(*it=="{")
				{
					count_curly_bracket=1;
					state=LeftCurlyBracket;
				}
				else
				{
					modifier_and_inherited_class_names+= *it;
				}
				break;
			case LeftCurlyBracket:
				if(*it=="{")
				{
					count_curly_bracket++;
				}
				else if(*it=="}")
				{
					count_curly_bracket--;
				}
				if(count_curly_bracket==0)
				{
					state=RightCurlyBracket;
				}
				break;
			case RightCurlyBracket:
				if(*it==";")
				{
					classList.push_back(r);
					state=Other;
					fprintf(stderr,"%s,%s,%s\n%s\n",class_name.c_str(),declspec_content.c_str(),modifier_and_inherited_class_names.c_str(),r.c_str());
					class_info ci;
					ci.body=r;
					ci.class_name=class_name;
					ci.declspec_content=declspec_content;
					ci.modifier_and_inherited_class_names=modifier_and_inherited_class_names;
					class_info_list.push_back(ci);
					r="";
					class_name="";
					declspec_content="";
					modifier_and_inherited_class_names="";
					count_curly_bracket=0;
				}
				else
				{
					printf("Error: %s %s?\n",r.c_str(),it->c_str());
					r="";
					state=Other;
				}
				break;
			}
		}
	}
	void print_class_info_list(const std::list<class_info>&cil)
	{
		for(std::list<class_info>::const_iterator it=cil.begin();it!=cil.end();it++)
		{
			fprintf(stdout,"%s,%s,%s\n%s\n",it->class_name.c_str(),it->declspec_content.c_str(),it->modifier_and_inherited_class_names.c_str(),it->body.c_str());
			_getch();
		}
	}
	void delete_impl_file_name(std::list<std::string>&fileList)
	{
		for(std::list<std::string>::iterator it=fileList.begin();it!=fileList.end();it++)
		{
			if((*it).find("Impl.")!=std::string::npos)
			{
				it=fileList.erase(it);
				it--;
			}
		}
	}
	void insert_string(zzxcf_phrase&phrase,zzxcf_phrase::iterator where,const std::string&str)
	{
		zzxcf_phrase ts;
		tokenize(str,ts);
		phrase.insert(where,ts.begin(),ts.end());
	}
	void edit_impl_h_file(const std::string&impl_h_file,const class_info&class_info)
	{
		printf("%s\n",class_info.class_name.c_str());
		std::string content;
		file_operation_namespace::read_file(impl_h_file,content);
		zzxcf_phrase phrase;
		tokenize(content,phrase);
		std::string impl_class_name=class_info.class_name+"Impl";
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(*it==class_info.class_name)
			{
				*it=impl_class_name;
			}
		}
		zzxcf_phrase::iterator it_class=phrase.end();
		zzxcf_phrase::iterator it_class_decl=phrase.end();
		zzxcf_phrase::iterator it_class_paren_end=phrase.end();
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(*it=="class")
			{
				it_class=it;
			}
			if(it_class!=phrase.end()&& *it_class=="class")
			{
				if(*it=="__declspec")
				{
					it_class_decl=it;
				}
			}
			if(it_class_decl!=phrase.end()&& *it_class_decl=="__declspec")
			{
				if(*it==")")
				{
					it_class_paren_end=it;
					it_class_paren_end++;
				}
			}
			if(it!=phrase.end()&& *it==impl_class_name)
			{
				phrase.erase(it_class_decl,it_class_paren_end);
				break;
			}
		}
		std::string forward_class_declaration="class ";
		forward_class_declaration+=class_info.class_name;
		forward_class_declaration+=";\n";
		insert_string(phrase,it_class,forward_class_declaration);
		zzxcf_phrase::iterator it_left_curly_bracket;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(*it==impl_class_name)
			{
				it_left_curly_bracket=it;
				break;
			}
		}
		while(it_left_curly_bracket!=phrase.end())
		{
			if(*it_left_curly_bracket=="{")
			{
				break;
			}
			++it_left_curly_bracket;
		}
		if(it_left_curly_bracket!=phrase.end()&& *it_left_curly_bracket=="{")
		{
			it_left_curly_bracket++;
		}
		std::string interface_hide_constructer="\nprivate:\n\t";
		interface_hide_constructer+=class_info.class_name;
		interface_hide_constructer+="*m_pInterface;";
		interface_hide_constructer+="\nprivate:\n\t";
		interface_hide_constructer+=(impl_class_name+"(const "+impl_class_name+"&);\n\t");
		interface_hide_constructer+=(impl_class_name+"&operator=(const "+impl_class_name+"&);\n");
		interface_hide_constructer+="\npublic:\n\t";
		interface_hide_constructer+=(impl_class_name+"("+class_info.class_name+"*);\n");
		insert_string(phrase,it_left_curly_bracket,interface_hide_constructer);
		std::string new_content;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			new_content+=it->c_str();
		}
		file_operation_namespace::write_file(impl_h_file,new_content);
	}
	void edit_impl_cpp_file(const std::string&impl_cpp_file,const class_info&class_info,const std::string&impl_h_file,const std::string&h_file)
	{
		std::string content;
		file_operation_namespace::read_file(impl_cpp_file,content);
		zzxcf_phrase phrase;
		tokenize(content,phrase);
		std::string impl_class_name=class_info.class_name+"Impl";
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(*it==class_info.class_name)
			{
				*it=impl_class_name;
			}
		}
		std::string head_name=get_name_ext(h_file);
		to_lower_string(head_name);
		std::string include_head="#include \"";
		include_head+=get_name_ext(impl_h_file);
		include_head+="\"";
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			std::string t= *it;
			to_lower_string(t);
			size_t head_pos=t.find(head_name);
			if(t.find("#include")!=std::string::npos&&head_pos!=std::string::npos)
			{
				if(t[head_pos-1]=='\\'||t[head_pos-1]=='/'||t[head_pos-1]=='"')
				{
					*it=include_head;
				}
			}
		}
		std::string new_content;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			new_content+=it->c_str();
		}
		std::string constructor_method="\n";
		constructor_method+=impl_class_name;
		constructor_method+="::";
		constructor_method+=impl_class_name;
		constructor_method+="(";
		constructor_method+=class_info.class_name;
		constructor_method+="*pInterface):m_pInterface(pInterface)\n{\n\n}";
		new_content+=constructor_method;
		file_operation_namespace::write_file(impl_cpp_file,new_content);
	}
	void edit_h_file(const std::string&h_file,const class_info&class_info)
	{
		std::string content;
		file_operation_namespace::read_file(h_file,content);
		std::string include_memory="#include <memory>\n";
		size_t include_pos=content.find("#include");
		content.insert(include_pos!=std::string::npos?include_pos:0,include_memory);
		zzxcf_phrase phrase;
		tokenize(content,phrase);
		zzxcf_phrase::iterator it_class;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(*it=="class")
			{
				it_class=it;
			}
			if(*it==class_info.class_name)
			{
				break;
			}
		}
		std::string impl_class_name=class_info.class_name+"Impl";
		std::string forward_class_declaration="class ";
		forward_class_declaration+=impl_class_name;
		forward_class_declaration+=";\n";
		insert_string(phrase,it_class,forward_class_declaration);
		zzxcf_phrase::iterator it_left_curly_bracket;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(*it==impl_class_name)
			{
				it_left_curly_bracket=it;
				break;
			}
		}
		while(it_left_curly_bracket!=phrase.end())
		{
			if(*it_left_curly_bracket=="{")
			{
				break;
			}
			++it_left_curly_bracket;
		}
		if(it_left_curly_bracket!=phrase.end()&& *it_left_curly_bracket=="{")
		{
			it_left_curly_bracket++;
		}
		std::string interface_hide_constructer="\nprivate:\n\tstd::unique_ptr<";
		interface_hide_constructer+=impl_class_name;
		interface_hide_constructer+=">m_impl;\n";
		insert_string(phrase,it_left_curly_bracket,interface_hide_constructer);
		std::string new_content;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			new_content+=it->c_str();
		}
		copy_file(h_file,file_operation_namespace::generate_unique_file_name(h_file,".orig"));
		file_operation_namespace::write_file(h_file,new_content);
	}
	void edit_cpp_file(const std::string&cpp_file,const class_info&class_info,const std::string&impl_h_file,const std::string&h_file)
	{
		std::string content;
		file_operation_namespace::read_file(cpp_file,content);
		zzxcf_phrase phrase;
		tokenize(content,phrase);
		std::string impl_class_name=class_info.class_name+"Impl";
		std::string head_name=get_name_ext(h_file);
		to_lower_string(head_name);
		std::string include_head="#include \"";
		include_head+=get_name_ext(impl_h_file);
		include_head+="\"\n";
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			std::string t= *it;
			to_lower_string(t);
			size_t head_pos=t.find(head_name);
			if(t.find("#include")!=std::string::npos&&head_pos!=std::string::npos)
			{
				if(t[head_pos-1]=='\\'||t[head_pos-1]=='/'||t[head_pos-1]=='"')
				{
					phrase.insert(it,include_head);
				}
			}
		}
		enum State
		{
			Other,ClassName,DoubleColon,FunctionName,LeftParen,RightParen,LeftCurly,RightCurly
		};
		State state=Other;
		zzxcf_phrase new_function_ts;
		zzxcf_phrase::iterator it_new_function_begin=phrase.end();
		zzxcf_phrase::iterator it_new_function_end=phrase.end();
		std::string function_name,parameter_list,parameter;
		int count;
		std::string body="{\n\treturn this->m_impl->";
		std::string void_body="{\n\tthis->m_impl->";
		std::string func_type;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(is_token_white_space(*it)||is_token_comment(*it)|| *it=="const")
			{
				if(state!=LeftCurly)
				{
					new_function_ts.push_back(*it);
				}
				continue;
			}
			switch(state)
			{
			case Other:
				if(*it==class_info.class_name)
				{
					it_new_function_begin=it;
					new_function_ts.clear();
					new_function_ts.push_back(*it);
					state=ClassName;
				}
				else
				{
					func_type= *it;
				}
				break;
			case ClassName:
				if(*it=="::")
				{
					new_function_ts.push_back(*it);
					state=DoubleColon;
				}
				else
				{
					state=Other;
				}
				break;
			case DoubleColon:
				if(*it==class_info.class_name)
				{
					state=Other;
				}
				else if(is_token_identifier(*it))
				{
					new_function_ts.push_back(*it);
					function_name= *it;
					fprintf(stderr,"Function name %s\n",function_name.c_str());
					state=FunctionName;
				}
				else
				{
					state=Other;
				}
				break;
			case FunctionName:
				if(*it=="(")
				{
					new_function_ts.push_back(*it);
					state=LeftParen;
				}
				else
				{
					state=Other;
				}
				break;
			case LeftParen:
				if(*it=="(")
				{
					count=1;
					new_function_ts.push_back(*it);
					it++;
					while(it!=phrase.end())
					{
						new_function_ts.push_back(*it);
						if(*it=="(")
						{
							count++;
						}
						if(*it==")")
						{
							count--;
						}
						if(count==0)
						{
							break;
						}
						it++;
					}
				}
				else if(*it==")")
				{
					new_function_ts.push_back(*it);
					parameter_list+=parameter;
					fprintf(stderr,"Parameter std::list %s\n",parameter_list.c_str());
					state=RightParen;
				}
				else if(*it==",")
				{
					new_function_ts.push_back(*it);
					parameter_list+=parameter;
					parameter_list+=",";
				}
				else if(is_token_identifier(*it))
				{
					new_function_ts.push_back(*it);
					parameter= *it;
				}
				else
				{
					new_function_ts.push_back(*it);
				}
				break;
			case RightParen:
				if(*it=="{")
				{
					state=LeftCurly;
				}
				else
				{
					state=Other;
				}
				break;
			case LeftCurly:
				count=1;
				while(it!=phrase.end())
				{
					if(*it=="{")
					{
						count++;
					}
					if(*it=="}")
					{
						count--;
					}
					if(count==0)
					{
						it--;
						state=RightCurly;
						break;
					}
					it++;
				}
				break;
			case RightCurly:
				it_new_function_end=it;
				it_new_function_end++;
				insert_string(new_function_ts,new_function_ts.end(),(func_type=="void"?void_body:body)+function_name+"("+parameter_list+");\n}\n");
				phrase.insert(phrase.erase(it_new_function_begin,it_new_function_end),new_function_ts.begin(),new_function_ts.end());
				parameter_list="";
				parameter="";
				it++;
				state=Other;
				break;
			}
		}
		std::string new_content;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			new_content+=it->c_str();
		}
		std::string constructor_method="\n";
		constructor_method+=class_info.class_name;
		constructor_method+="::";
		constructor_method+=class_info.class_name;
		constructor_method+="():m_impl(new ";
		constructor_method+=impl_class_name;
		constructor_method+="(this))\n{\n\n}";
		new_content+=constructor_method;
		copy_file(cpp_file,file_operation_namespace::generate_unique_file_name(cpp_file,".orig"));
		file_operation_namespace::write_file(cpp_file,new_content);
	}
	void save_functions_into_sqlite3_database(std::list<std::string> &to_be_formatted_files)
	{
		format_utility_namespace::initialize_sqlite3_functions_details();
		if(!command_line_namespace::option_do_not_use_multithreads.is_turned_on())
		{
			format_utility_namespace::save_functions_using_threads(to_be_formatted_files);
		}
		else
		{
			format_utility_namespace::save_functions_from_source_files(to_be_formatted_files);
		}
		format_utility_namespace::finalize_sqlite3_functions_details();
	}
	void delete_all_orig_files(int argc,char**argv,int index)
	{
		std::list<std::string>to_be_removed_files;
		file_operation_namespace::search_command_line_orig_files(argc,argv,index,to_be_removed_files);
		for_each(to_be_removed_files.begin(),to_be_removed_files.end(),file_operation_namespace::remove_file);
	}
	void restore_previous_version(std::list<std::string> &to_be_formatted_files)
	{
		std::list<std::string>previous_formatted_files=to_be_formatted_files;
		transform(to_be_formatted_files.begin(),to_be_formatted_files.end(),previous_formatted_files.begin(),file_operation_namespace::get_latest_orig_file);
		std::list<std::string>::const_iterator it1=to_be_formatted_files.begin();
		std::list<std::string>::const_iterator it2=previous_formatted_files.begin();
		for(;it1!=to_be_formatted_files.end()&&it2!=previous_formatted_files.end();it1++,it2++)
		{
			if(it1->length()==0||it2->length()==0)
			{
				fprintf(stderr,"Rename file failed from: \"%s\"\n\t\tto: \"%s\"\n",it2->c_str(),it1->c_str());
				continue;
			}
			if(file_operation_namespace::file_exists(*it1)&&file_operation_namespace::file_exists(*it2))
			{
				file_operation_namespace::remove_file(*it1);
				file_operation_namespace::rename_file(*it2,*it1);
			}
		}
	}
	void convert_to_bridge_pattern(std::list<std::string> &to_be_formatted_files)
	{
		delete_impl_file_name(to_be_formatted_files);
		for(std::list<std::string>::const_iterator it=to_be_formatted_files.begin();it!=to_be_formatted_files.end();it++)
		{
			if(is_cpp_file(*it))
			{
				std::string cpp_file= *it;
				fprintf(stderr,"Cpp file, file is %s\n",cpp_file.c_str());
				std::string h_file=generate_h_file_name_from_cpp_file_name(cpp_file);
				if(!file_operation_namespace::file_exists(h_file))
				{
					continue;
				}
				std::string h_content;
				file_operation_namespace::read_file(h_file,h_content);
				zzxcf_phrase phrase;
				tokenize(h_content,phrase);
				if(count_specific_token(phrase,"dllexport")==0)
				{
					fprintf(stderr,"Token 'dllexport' count = 0, cannot impl, file is %s\n",h_file.c_str());
					continue;
				}
				if(count_specific_token(phrase,"class")>1)
				{
					fprintf(stderr,"Token 'class' count > 1, file is %s\n",h_file.c_str());
				}
				std::list<std::string>classList;
				std::list<class_info>class_info_list;
				extract_classes(phrase,classList,class_info_list);
				fprintf(stderr,"Head file, file is %s\n",h_file.c_str());
				std::string impl_cpp_file=generate_impl_file_name(cpp_file);
				if(false&&file_operation_namespace::file_exists(impl_cpp_file))
				{
					continue;
				}
				fprintf(stderr,"Cpp implementation file, file is %s\n",impl_cpp_file.c_str());
				let_user_decide_remove_copy(cpp_file,impl_cpp_file);
				std::string impl_h_file=generate_impl_file_name(h_file);
				if(false&&file_operation_namespace::file_exists(impl_h_file))
				{
					continue;
				}
				fprintf(stderr,"Head implementation file, file is %s\n",impl_h_file.c_str());
				let_user_decide_remove_copy(h_file,impl_h_file);
				if(!file_operation_namespace::file_exists(impl_cpp_file)|| !file_operation_namespace::file_exists(impl_h_file))
				{
					continue;
				}
#ifdef _DEBUG
				print_class_info_list(class_info_list);
#endif
				for(std::list<class_info>::const_iterator iter=class_info_list.begin();iter!=class_info_list.end();iter++)
				{
					if(iter->declspec_content=="dllexport")
					{
						edit_impl_h_file(impl_h_file,*iter);
						edit_impl_cpp_file(impl_cpp_file,*iter,impl_h_file,h_file);
						edit_h_file(h_file,*iter);
						edit_cpp_file(cpp_file,*iter,impl_h_file,h_file);
					}
				}
			}
		}
	}
	void class_decorate_surround_single_sentence_after_reserve_word_for_with_curly_bracket::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		fix_for(phrase);
	}
	void class_decorate_surround_single_sentence_after_reserve_word_for_with_curly_bracket::stack_push_top_pop(const format_utility_namespace::zzxcf_token&token,std::stack<size_t>&round_bracket_count_stack,size_t&round_bracket_count,std::stack<size_t>&curly_bracket_count_stack,size_t&curly_bracket_count)
	{
		if(token.find("#if")==0)
		{
			round_bracket_count_stack.push(round_bracket_count);
			curly_bracket_count_stack.push(curly_bracket_count);
		}
		else if(token.find("#else")==0||token.find("#elif")==0)
		{
			assert(round_bracket_count_stack.size()>0);
			assert(curly_bracket_count_stack.size()>0);
			if(round_bracket_count_stack.size()>0)
			{
				round_bracket_count=round_bracket_count_stack.top();
			}
			if(curly_bracket_count_stack.size()>0)
			{
				curly_bracket_count=curly_bracket_count_stack.top();
			}
		}
		else if(token.find("#endif")==0)
		{
			assert(round_bracket_count_stack.size()>0);
			assert(curly_bracket_count_stack.size()>0);
			if(round_bracket_count_stack.size()>0)
			{
				round_bracket_count_stack.pop();
			}
			if(curly_bracket_count_stack.size()>0)
			{
				curly_bracket_count_stack.pop();
			}
		}
	}
	void class_decorate_surround_single_sentence_after_reserve_word_for_with_curly_bracket::concat_tokens_5fixing_for(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i)
	{
		typedef enum
		{
			Other,For,ForBracketLeft,ForBracketRight,ForSingleSentenceBegin
		}
		State;
		State st=Other;
		size_t round_bracket_count,curly_bracket_count;
		std::stack<size_t>round_bracket_count_stack,curly_bracket_count_stack;
		for(zzxcf_phrase::iterator iter=i;iter!=phrase.end();iter++)
		{
			const zzxcf_token&token=(*iter);
			switch(st)
			{
			case Other:
				if(token=="for")
				{
					st=For;
				}
				break;
			case For:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="(")
				{
					st=ForBracketLeft;
					round_bracket_count=1;
				}
				else
				{
					iter--;
					i=iter;
					return;
				}
				break;
			case ForBracketLeft:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(token=="(")
				{
					round_bracket_count++;
				}
				else if(token==")")
				{
					round_bracket_count--;
				}
				if(round_bracket_count==0)
				{
					st=ForBracketRight;
				}
				break;
			case ForBracketRight:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(token.find("#else")==0)
				{
					i=iter;
					return;
				}
				else if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="{")
				{
					i=iter;
					return;
				}
				else if(token=="if")
				{
					iter=phrase.insert(iter,"{");
					iter++;
					concat_tokens_5fixing_ifelse(phrase,iter);
					iter++;
					iter=phrase.insert(iter,"}");
					i=iter;
					return;
				}
				else
				{
					st=ForSingleSentenceBegin;
					iter=phrase.insert(iter,"{");
					round_bracket_count=0;
					curly_bracket_count=0;
				}
				break;
			case ForSingleSentenceBegin:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(token=="(")
				{
					round_bracket_count++;
				}
				else if(token==")")
				{
					round_bracket_count--;
				}
				else if(token=="{")
				{
					curly_bracket_count++;
				}
				else if(token=="}")
				{
					curly_bracket_count--;
				}
				if((token==";"||token=="}")&&round_bracket_count==0&&curly_bracket_count==0)
				{
					if(token=="}")
					{
						iter++;
						if(iter!=phrase.end()&&(*iter==";"))
						{
							iter++;
						}
					}
					else
					{
						iter++;
					}
					iter=phrase.insert(iter,"}");
					i=iter;
					return;
				}
				break;
			}
		}
	}
	void class_decorate_surround_single_sentence_after_reserve_word_for_with_curly_bracket::fix_for(zzxcf_phrase&phrase)
	{
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if((*it)=="for")
			{
				zzxcf_phrase::iterator iter=it;
				concat_tokens_5fixing_for(phrase,iter);
			}
		}
	}	
	void class_decorate_strip_meaningless_pragmas::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		pragmas=remove_pragmas(phrase);
	}
	std::string class_decorate_strip_meaningless_pragmas::get_meaningless_pragmas()
	{
		return pragmas;
	}
	std::string class_decorate_strip_meaningless_pragmas::remove_pragmas(zzxcf_phrase&phrase)
	{
		std::string r;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(is_token_directive_line(*it)||is_token_directive_pragma_region_endregion(*it))
			{
				r+=(*it)+"\n";
				*it=" ";
			}
		}
		return r;
	}
	class_decorate_remove_utf8_bom::class_decorate_remove_utf8_bom(std::string interim_result_file_name)
	{
		this->interim_result_file_name=interim_result_file_name;
	}
	void class_decorate_remove_utf8_bom::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		remove_utf8_bom(phrase);
		zzxcf_section section;
		if(command_line_namespace::option_save_interim_result.is_turned_on())
		{
			section.clear();
			section.push_back(phrase);
			interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".remove_utf8_bom");
			write_section_to_file(section,interim_result_file_name);
			section.clear();
		}
	}
	void class_decorate_remove_utf8_bom::remove_utf8_bom(zzxcf_phrase&phrase)
	{
		char bom[4]="\xEF\xBB\xBF";
		std::string s1,s3;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(split_string_with_substr(std::string(*it),std::string(bom),s1,s3)!=std::string::npos)
			{
				fprintf(stderr,"Remove UTF-8 BOM %s\n",it->c_str());
				*it=s1+" "+s3;
			}
		}
	}
	class_decorate_strip_specific_conditional_compilation::class_decorate_strip_specific_conditional_compilation(std::string interim_result_file_name)
	{
		this->interim_result_file_name=interim_result_file_name;
	}
	void class_decorate_strip_specific_conditional_compilation::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		strip_specific_conditional_compilation(phrase);
		zzxcf_section section;
		if(command_line_namespace::option_save_interim_result.is_turned_on())
		{
			section.clear();
			section.push_back(phrase);
			interim_result_file_name=file_operation_namespace::generate_unique_file_name(interim_result_file_name,".strip_specific_conditional_compilation");
			write_section_to_file(section,interim_result_file_name);
			section.clear();
		}
	}
	void class_decorate_strip_specific_conditional_compilation::strip_specific_conditional_compilation(format_utility_namespace::zzxcf_phrase&phrase)
	{
		for(zzxcf_phrase::iterator iter_if=phrase.begin();iter_if!=phrase.end();iter_if++)
		{
			if(is_token_directive(*iter_if))
			{
				std::string directive=(*iter_if);
				std::string simplified_directive=simplify_directive(directive,true);
				zzxcf_map::iterator iter1=directive_map.find(simplified_directive);
				if(iter1!=directive_map.end())
				{
					if(iter1->second.operation==k_delete)
					{
						if((*iter_if).find("#if")==0)
						{
							zzxcf_phrase::iterator iter2=iter_if;
							long if_count=0;
							for(;iter2!=phrase.end();iter2++)
							{
								if((*iter2).find("#if")==0)
								{
									if_count++;
								}
								else if((*iter2).find("#endif")==0)
								{
									if_count--;
								}
								if(if_count==0)
								{
									break;
								}
							}
							if(iter2!=phrase.end())
							{
								iter2++;
							}
							iter_if=phrase.erase(iter_if,iter2);
							iter_if--;
						}
						else
						{
							iter_if=phrase.erase(iter_if);
							iter_if--;
						}
					}
					else if(iter1->second.operation==k_delete_if_block||iter1->second.operation==k_delete_else_block)
					{
						if((*iter_if).find("#if")==0)
						{
							zzxcf_phrase::iterator iter_endif=iter_if,iter_else=iter_if;
							long if_count=0;
							for(;iter_endif!=phrase.end();iter_endif++)
							{
								if((*iter_endif).find("#if")==0)
								{
									if_count++;
								}
								else if((*iter_endif).find("#endif")==0)
								{
									if_count--;
								}
								else if((*iter_endif)=="#else")
								{
									if(if_count==1)
									{
										iter_else=iter_endif;
									}
								}
								if(if_count==0)
								{
									break;
								}
							}
							if(iter1->second.operation==k_delete_if_block)
							{
								if(iter_else==iter_if)
								{
									if(iter_endif!=phrase.end())
									{
										iter_endif++;
									}
									iter_if=phrase.erase(iter_if,iter_endif);
									iter_if--;
								}
								else
								{
									if(iter_else!=phrase.end())
									{
										iter_else++;
									}
									phrase.erase(iter_endif);
									iter_if=phrase.erase(iter_if,iter_else);
									iter_if--;
								}
							}
							else
							{
								if(iter_else!=iter_if)
								{
									if(iter_endif!=phrase.end())
									{
										iter_endif++;
									}
									phrase.erase(iter_else,iter_endif);
									iter_if=phrase.erase(iter_if);
									iter_if--;
								}
							}
						}
					}
				}
			}
		}
	}
	void class_decorate_strip_meaningless_comments::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		empty_comments=remove_empty_comments(phrase);
	}
	std::string class_decorate_strip_meaningless_comments::get_meaningless_comments()
	{
		return empty_comments;
	}
	std::string class_decorate_strip_meaningless_comments::remove_empty_comments(zzxcf_phrase&phrase)
	{
		std::string r;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if(is_token_empty_comment(*it))
			{
				r+=(*it)+"\n";
				*it=" ";
			}
		}
		return r;
	}	
	void class_decorate_strip_duplicate_semicolon::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		remove_redundant_semicolon(phrase);
	}
	void class_decorate_strip_duplicate_semicolon::remove_redundant_semicolon(zzxcf_phrase&phrase)
	{
		enum State
		{
			Other,LeftCurlyBracket,Semicolon,For,ParenthesisLeft
		};
		State state=Other;
		int round_bracket_Count=0;
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			switch(state)
			{
			case Other:
				if(is_token_left_curly_bracket(*it))
				{
					state=LeftCurlyBracket;
				}
				else if(is_token_semicolon(*it))
				{
					state=Semicolon;
				}
				else if(*it=="for")
				{
					state=For;
				}
				break;
			case For:
				if(is_token_left_round_bracket(*it))
				{
					state=ParenthesisLeft;
					round_bracket_Count=1;
				}
				break;
			case ParenthesisLeft:
				if(is_token_left_round_bracket(*it))
				{
					round_bracket_Count++;
				}
				else if(is_token_right_round_bracket(*it))
				{
					round_bracket_Count--;
				}
				if(round_bracket_Count==0)
				{
					state=Other;
				}
				break;
			case LeftCurlyBracket:
			case Semicolon:
				if(is_token_white_space(*it))
				{
				}
				else if(is_token_semicolon(*it))
				{
					it=phrase.erase(it);
				}
				else if(*it=="for")
				{
					state=For;
				}
				else
				{
					state=Other;
				}
				break;
			}
		}
	}
	void class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		fix_if_else(phrase);
	}
	void class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket::concat_tokens_5fixing_ifelse(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i)
	{
		typedef enum
		{
			Other,If,IfBracketLeft,IfBracketRight,IfCurlyBracketLeft,IfEnd,IfSingleSentenceBegin,IfEndElse,IfEndElseCurlyBracketLeft,IfEndElseEnd,IfEndElseSingleSentenceBegin
		}
		State;
		State st=Other;
		long bracketCount,curlyBracketCount;
		for(zzxcf_phrase::iterator iter=i;iter!=phrase.end();iter++)
		{
			const zzxcf_token&token=(*iter);
			switch(st)
			{
			case Other:
				if(token=="if")
				{
					st=If;
				}
				break;
			case If:
				if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="(")
				{
					st=IfBracketLeft;
					bracketCount=1;
				}
				else
				{
					iter--;
					i=iter;
					return;
				}
				break;
			case IfBracketLeft:
				if(token=="(")
				{
					bracketCount++;
				}
				else if(token==")")
				{
					bracketCount--;
				}
				if(bracketCount==0)
				{
					st=IfBracketRight;
				}
				break;
			case IfBracketRight:
				if(token.find("#else")==0)
				{
					i=iter;
					return;
				}
				else if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="{")
				{
					st=IfCurlyBracketLeft;
					bracketCount=1;
				}
				else if(token=="if")
				{
					iter=phrase.insert(iter,"{");
					iter++;
					concat_tokens_5fixing_ifelse(phrase,iter);
					iter++;
					iter=phrase.insert(iter,"}");
					st=IfEnd;
				}
				else if(token=="do"||token=="switch"||token=="try")
				{
					i=iter;
					return;
				}
				else
				{
					st=IfSingleSentenceBegin;
					iter=phrase.insert(iter,"{");
					bracketCount=0;
					curlyBracketCount=0;
				}
				break;
			case IfCurlyBracketLeft:
				if(token=="{")
				{
					bracketCount++;
				}
				else if(token=="}")
				{
					bracketCount--;
				}
				if(bracketCount==0)
				{
					st=IfEnd;
				}
				break;
			case IfSingleSentenceBegin:
				if(token=="(")
				{
					bracketCount++;
				}
				else if(token==")")
				{
					bracketCount--;
				}
				else if(token=="{")
				{
					curlyBracketCount++;
				}
				else if(token=="}")
				{
					curlyBracketCount--;
				}
				if(token=="else"&&bracketCount==0&&curlyBracketCount==0)
				{
					iter=phrase.insert(iter,";");
					iter--;
					continue;
				}
				if(token=="}"&&bracketCount==0&&curlyBracketCount<0)
				{
					curlyBracketCount++;
					iter=phrase.insert(iter,";");
					iter--;
					continue;
				}
				if((token==";"||token=="}")&&bracketCount==0&&curlyBracketCount==0)
				{
					if(token=="}")
					{
						iter++;
						if(iter!=phrase.end()&& *iter==";")
						{
							iter++;
						}
					}
					else
					{
						iter++;
					}
					iter=phrase.insert(iter,"}");
					st=IfEnd;
				}
				break;
			case IfEnd:
				if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="else")
				{
					st=IfEndElse;
				}
				else
				{
					iter--;
					while(is_token_white_space_comment_directive(*iter))iter--;
					i=iter;
					return;
				}
				break;
			case IfEndElse:
				if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="if")
				{
					if(command_line_namespace::option_surround_if_clause_after_reserve_word_else_with_curly_bracket.is_turned_on())
					{
						iter=phrase.insert(iter,"{");
						iter++;
					}
					concat_tokens_5fixing_ifelse(phrase,iter);
					if(command_line_namespace::option_surround_if_clause_after_reserve_word_else_with_curly_bracket.is_turned_on())
					{
						iter++;
						iter=phrase.insert(iter,"}");
					}
					i=iter;
					return;
				}
				else if(token=="for")
				{
					iter=phrase.insert(iter,"{");
					iter++;
					concat_tokens_5fixing_for(phrase,iter);
					iter++;
					iter=phrase.insert(iter,"}");
					i=iter;
					return;
				}
				else if(token=="{")
				{
					st=IfEndElseCurlyBracketLeft;
					bracketCount=1;
				}
				else
				{
					iter=phrase.insert(iter,"{");
					st=IfEndElseSingleSentenceBegin;
					bracketCount=0;
					curlyBracketCount=0;
				}
				break;
			case IfEndElseCurlyBracketLeft:
				if(token=="{")
				{
					bracketCount++;
				}
				else if(token=="}")
				{
					bracketCount--;
				}
				if(bracketCount==0)
				{
					i=iter;
					return;
				}
				break;
			case IfEndElseSingleSentenceBegin:
				if(token=="(")
				{
					bracketCount++;
				}
				else if(token==")")
				{
					bracketCount--;
				}
				else if(token=="{")
				{
					curlyBracketCount++;
				}
				else if(token=="}")
				{
					curlyBracketCount--;
				}
				if((token==";"||token=="}")&&bracketCount==0&&curlyBracketCount==0)
				{
					if(token=="}")
					{
						iter++;
						if(*iter==";")
						{
							iter++;
						}
					}
					else
					{
						iter++;
					}
					iter=phrase.insert(iter,"}");
					i=iter;
					return;
				}
				break;
			}
		}
	}
	void class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket::stack_push_top_pop(const format_utility_namespace::zzxcf_token&token,std::stack<size_t>&round_bracket_count_stack,size_t&round_bracket_count,std::stack<size_t>&curly_bracket_count_stack,size_t&curly_bracket_count)
	{
		if(token.find("#if")==0)
		{
			round_bracket_count_stack.push(round_bracket_count);
			curly_bracket_count_stack.push(curly_bracket_count);
		}
		else if(token.find("#else")==0||token.find("#elif")==0)
		{
			assert(round_bracket_count_stack.size()>0);
			assert(curly_bracket_count_stack.size()>0);
			if(round_bracket_count_stack.size()>0)
			{
				round_bracket_count=round_bracket_count_stack.top();
			}
			if(curly_bracket_count_stack.size()>0)
			{
				curly_bracket_count=curly_bracket_count_stack.top();
			}
		}
		else if(token.find("#endif")==0)
		{
			assert(round_bracket_count_stack.size()>0);
			assert(curly_bracket_count_stack.size()>0);
			if(round_bracket_count_stack.size()>0)
			{
				round_bracket_count_stack.pop();
			}
			if(curly_bracket_count_stack.size()>0)
			{
				curly_bracket_count_stack.pop();
			}
		}
	}
	void class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket::concat_tokens_5fixing_for(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i)
	{
		typedef enum
		{
			Other,For,ForBracketLeft,ForBracketRight,ForSingleSentenceBegin
		}
		State;
		State st=Other;
		size_t round_bracket_count,curly_bracket_count;
		std::stack<size_t>round_bracket_count_stack,curly_bracket_count_stack;
		for(zzxcf_phrase::iterator iter=i;iter!=phrase.end();iter++)
		{
			const zzxcf_token&token=(*iter);
			switch(st)
			{
			case Other:
				if(token=="for")
				{
					st=For;
				}
				break;
			case For:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="(")
				{
					st=ForBracketLeft;
					round_bracket_count=1;
				}
				else
				{
					iter--;
					i=iter;
					return;
				}
				break;
			case ForBracketLeft:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(token=="(")
				{
					round_bracket_count++;
				}
				else if(token==")")
				{
					round_bracket_count--;
				}
				if(round_bracket_count==0)
				{
					st=ForBracketRight;
				}
				break;
			case ForBracketRight:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(token.find("#else")==0)
				{
					i=iter;
					return;
				}
				else if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="{")
				{
					i=iter;
					return;
				}
				else if(token=="if")
				{
					iter=phrase.insert(iter,"{");
					iter++;
					concat_tokens_5fixing_ifelse(phrase,iter);
					iter++;
					iter=phrase.insert(iter,"}");
					i=iter;
					return;
				}
				else
				{
					st=ForSingleSentenceBegin;
					iter=phrase.insert(iter,"{");
					round_bracket_count=0;
					curly_bracket_count=0;
				}
				break;
			case ForSingleSentenceBegin:
				if(is_token_directive(token))
				{
					stack_push_top_pop(token,round_bracket_count_stack,round_bracket_count,curly_bracket_count_stack,curly_bracket_count);
				}
				else if(token=="(")
				{
					round_bracket_count++;
				}
				else if(token==")")
				{
					round_bracket_count--;
				}
				else if(token=="{")
				{
					curly_bracket_count++;
				}
				else if(token=="}")
				{
					curly_bracket_count--;
				}
				if((token==";"||token=="}")&&round_bracket_count==0&&curly_bracket_count==0)
				{
					if(token=="}")
					{
						iter++;
						if(iter!=phrase.end()&&(*iter==";"))
						{
							iter++;
						}
					}
					else
					{
						iter++;
					}
					iter=phrase.insert(iter,"}");
					i=iter;
					return;
				}
				break;
			}
		}
	}
	void class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket::fix_if_else(zzxcf_phrase&phrase)
	{
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if((*it)=="if")
			{
				zzxcf_phrase::iterator iter=it;
				concat_tokens_5fixing_ifelse(phrase,iter);
			}
		}
	}
	void class_decorate_surround_if_clause_after_reserve_word_else_with_curly_bracket::process(zzxcf_phrase&phrase)
	{
		if(innerDecorator)
		{
			innerDecorator->process(phrase);
		}
		fix_else_if(phrase);
	}
	void class_decorate_surround_if_clause_after_reserve_word_else_with_curly_bracket::concat_tokens_5fixing_elseif(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i)
	{
		enum State
		{
			Other,If,IfBracketLeft,IfBracketRight,IfCurlyBracketLeft,IfEnd,IfSingleSentenceBegin,IfEndElse,IfEndElseCurlyBracketLeft,IfEndElseEnd,IfEndElseSingleSentenceBegin
		};
		State st=Other;
		long bracketCount,curlyBracketCount;
		for(zzxcf_phrase::iterator iter=i;iter!=phrase.end();iter++)
		{
			const zzxcf_token&token=(*iter);
			switch(st)
			{
			case Other:
				if(token=="if")
				{
					st=If;
				}
				break;
			case If:
				if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="(")
				{
					st=IfBracketLeft;
					bracketCount=1;
				}
				else
				{
					iter--;
					i=iter;
					return;
				}
				break;
			case IfBracketLeft:
				if(token=="(")
				{
					bracketCount++;
				}
				else if(token==")")
				{
					bracketCount--;
				}
				if(bracketCount==0)
				{
					st=IfBracketRight;
				}
				break;
			case IfBracketRight:
				if(token.find("#else")==0)
				{
					i=iter;
					return;
				}
				else if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="{")
				{
					st=IfCurlyBracketLeft;
					bracketCount=1;
				}
				else if(token=="if")
				{
					concat_tokens_5fixing_elseif(phrase,iter);
					st=IfEnd;
				}
				else if(token=="do"||token=="switch"||token=="try")
				{
					i=iter;
					return;
				}
				else
				{
					st=IfSingleSentenceBegin;
					bracketCount=0;
					curlyBracketCount=0;
				}
				break;
			case IfCurlyBracketLeft:
				if(token=="{")
				{
					bracketCount++;
				}
				else if(token=="}")
				{
					bracketCount--;
				}
				if(bracketCount==0)
				{
					st=IfEnd;
				}
				break;
			case IfSingleSentenceBegin:
				if(token=="(")
				{
					bracketCount++;
				}
				else if(token==")")
				{
					bracketCount--;
				}
				else if(token=="{")
				{
					curlyBracketCount++;
				}
				else if(token=="}")
				{
					curlyBracketCount--;
				}
				if(token=="else"&&bracketCount==0&&curlyBracketCount==0)
				{
					iter=phrase.insert(iter,";");
					iter--;
					continue;
				}
				if(token=="}"&&bracketCount==0&&curlyBracketCount<0)
				{
					curlyBracketCount++;
					iter=phrase.insert(iter,";");
					iter--;
					continue;
				}
				if((token==";"||token=="}")&&bracketCount==0&&curlyBracketCount==0)
				{
					st=IfEnd;
				}
				break;
			case IfEnd:
				if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="else")
				{
					st=IfEndElse;
				}
				else
				{
					iter--;
					while(is_token_white_space_comment_directive(*iter))iter--;
					i=iter;
					return;
				}
				break;
			case IfEndElse:
				if(is_token_white_space_comment_directive(token))
				{
				}
				else if(token=="if")
				{
					iter=phrase.insert(iter,"{");
					iter++;
					concat_tokens_5fixing_elseif(phrase,iter);
					iter++;
					iter=phrase.insert(iter,"}");
					i=iter;
					return;
				}
				else if(token=="{")
				{
					st=IfEndElseCurlyBracketLeft;
					bracketCount=1;
				}
				else
				{
					st=IfEndElseSingleSentenceBegin;
					bracketCount=0;
					curlyBracketCount=0;
				}
				break;
			case IfEndElseCurlyBracketLeft:
				if(token=="{")
				{
					bracketCount++;
				}
				else if(token=="}")
				{
					bracketCount--;
				}
				if(bracketCount==0)
				{
					i=iter;
					return;
				}
				break;
			case IfEndElseSingleSentenceBegin:
				if(token=="(")
				{
					bracketCount++;
				}
				else if(token==")")
				{
					bracketCount--;
				}
				else if(token=="{")
				{
					curlyBracketCount++;
				}
				else if(token=="}")
				{
					curlyBracketCount--;
				}
				if((token==";"||token=="}")&&bracketCount==0&&curlyBracketCount==0)
				{
					i=iter;
					return;
				}
				break;
			}
		}
	}
	void class_decorate_surround_if_clause_after_reserve_word_else_with_curly_bracket::fix_else_if(zzxcf_phrase&phrase)
	{
		for(zzxcf_phrase::iterator it=phrase.begin();it!=phrase.end();it++)
		{
			if((*it)=="if")
			{
				zzxcf_phrase::iterator iter=it;
				concat_tokens_5fixing_elseif(phrase,iter);
			}
		}
	}
	class_decorator::class_decorator(zzxcf_phrase&phrase,std::string&fileName,const std::string itermediateFileExtension,bool flagInsertSeparator)
	{
		this->phrase= &phrase;
		this->fileName= &fileName;
		this->itermediateFileExtension=itermediateFileExtension;
	}
	class_decorator::class_decorator():
	phrase(0),fileName(0)
	{
	}
	std::shared_ptr<class_decorator>class_decorator::get_inner_decorator()
	{
		return innerDecorator;
	}
	void class_decorator::set_inner_decorator(std::shared_ptr<class_decorator>d)
	{
		innerDecorator=d;
	}
	void class_decorator::process(zzxcf_phrase phrase)
	{
	}
	void class_decorator::process()
	{
		if(innerDecorator)
		{
			innerDecorator->process();
		}
		if(command_line_namespace::option_save_interim_result.is_turned_on())
		{
			save_itermediate();
		}
	}
	void class_decorator::save_itermediate()
	{
		zzxcf_section section;
		section.push_back(*phrase);
		*fileName=file_operation_namespace::generate_unique_file_name(*fileName,itermediateFileExtension);
		write_section_to_file(section,*fileName);
	}
	class_decorator::~class_decorator()
	{
	}
	class_file_formatter::class_file_formatter(std::string file_path):
	m_number_of_files(0),m_number_of_input_lines(0),m_number_of_output_tokens(0),m_number_of_output_lines(0)
	{
		try
		{
			const char*file_paths[2];
			file_paths[0]=file_path.c_str();
			file_paths[1]=0;
			format_files(file_paths);
		}
		catch(...)
		{
			fprintf(stderr,"!!!exception!!! file path \"%s\"\n",file_path.c_str());
		}
	}
	class_file_formatter::class_file_formatter(const char**file_paths):
	m_number_of_files(0),m_number_of_input_lines(0),m_number_of_output_tokens(0),m_number_of_output_lines(0)
	{
		try
		{
			format_files(file_paths);
		}
		catch(...)
		{
			while(*file_paths==0)
			{
				file_paths++;
			}
			if(*file_paths!=0)
			{
				fprintf(stderr,"!!!exception!!! file path \"%s\"\n",*file_paths);
			}
		}
	}
	void class_file_formatter::format_files(const char**file_paths)
	{
		while(*file_paths)
		{
			const char*current_file_path=(*file_paths);
			*file_paths=0;
			if(!command_line_namespace::option_do_not_backup_files.is_turned_on())
			{
				::CopyFileA(current_file_path,file_operation_namespace::generate_unique_file_name(current_file_path,".orig").c_str(),true);
			}
			if(command_line_namespace::option_do_not_touch_readonly_files.is_turned_on())
			{
				if(file_operation_namespace::is_readonly(current_file_path))
				{
					*file_paths=current_file_path;
					file_paths++;
					continue;
				}
			}
			std::string old_file_content;
			if(!file_operation_namespace::read_file(current_file_path,old_file_content))
			{
				*file_paths=current_file_path;
				file_paths++;
				continue;
			}
			if(command_line_namespace::option_do_not_touch_utf8_encoded_files.is_turned_on())
			{
				if(file_operation_namespace::is_utf8_encoded_file_content(old_file_content))
				{
					*file_paths=current_file_path;
					file_paths++;
					continue;
				}
			}
			std::string new_file_content;
			size_t number_of_input_tokens=0,number_of_output_tokens=0;
			size_t old_file_content_number_of_line_feeds=0,new_file_content_number_of_line_feeds=0;
			if(old_file_content.length()!=0)
			{
				zzxcf_phrase phrase;
				tokenize(old_file_content,phrase);
				number_of_input_tokens=phrase.size();
				int hr=regroup(phrase,new_file_content,current_file_path);
				if(0!=hr)
				{
					EnterCriticalSection(&namespace_multithread::critical_section_of_map_operation);
					failed_regrouping_file_paths.push_back(current_file_path);
					LeaveCriticalSection(&namespace_multithread::critical_section_of_map_operation);
				}
				phrase.clear();
				if(new_file_content.length()!=0)
				{
					tokenize(new_file_content,phrase);
				}
				number_of_output_tokens=phrase.size();
				m_number_of_files++;
				old_file_content_number_of_line_feeds=file_operation_namespace::count_number_of_line_feeds(old_file_content);
				m_number_of_input_lines+=old_file_content_number_of_line_feeds;
				m_number_of_input_tokens+=number_of_input_tokens;
				m_number_of_output_tokens+=number_of_output_tokens;
				new_file_content_number_of_line_feeds=file_operation_namespace::count_number_of_line_feeds(new_file_content);
				m_number_of_output_lines+=new_file_content_number_of_line_feeds;
			}
			if(!command_line_namespace::option_do_not_touch_files.is_turned_on())
			{
				if(old_file_content!=new_file_content|| !command_line_namespace::option_do_not_touch_unchanged_files.is_turned_on())
				{
					file_operation_namespace::write_file(current_file_path,new_file_content);
					if(!command_line_namespace::option_do_not_print_statistical_data.is_turned_on())
					{
						fprintf(stdout,"Formatted, number of input lines(%zd), number of input tokens(%zd), number of output lines(%zd), number of output tokens(%zd), file path \"%s\"\n",old_file_content_number_of_line_feeds,number_of_input_tokens,new_file_content_number_of_line_feeds,number_of_output_tokens,current_file_path);
					}
				}
				else
				{
					if(!command_line_namespace::option_do_not_print_statistical_data.is_turned_on())
					{
						fprintf(stdout,"Unchanged, number of input lines(%zd), number of input tokens(%zd), file path \"%s\"\n",old_file_content_number_of_line_feeds,number_of_input_tokens,current_file_path);
					}
				}
			}
			file_paths++;
		}
		if(!command_line_namespace::option_do_not_print_statistical_data.is_turned_on())
		{
			fprintf(stdout,"Number of files(%I64u), number of input tokens(%I64u), number of input lines(%I64u), number of output tokens(%I64u), number of output lines(%I64u)\n",m_number_of_files,m_number_of_input_tokens,m_number_of_input_lines,m_number_of_output_tokens,m_number_of_output_lines);
			EnterCriticalSection(&namespace_multithread::critical_section_of_statistic);
			statistic_utility_namespace::total_number_of_files+=m_number_of_files;
			statistic_utility_namespace::total_number_of_input_tokens+=m_number_of_input_tokens;
			statistic_utility_namespace::total_number_of_input_lines+=m_number_of_input_lines;
			statistic_utility_namespace::total_number_of_output_tokens+=m_number_of_output_tokens;
			statistic_utility_namespace::total_number_of_output_lines+=m_number_of_output_lines;
			LeaveCriticalSection(&namespace_multithread::critical_section_of_statistic);
		}
	}
}
