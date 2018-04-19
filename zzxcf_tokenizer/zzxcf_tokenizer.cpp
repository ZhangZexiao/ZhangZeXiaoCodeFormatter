#include "stdafx.h"
#include "zzxcf_tokenizer.h"
bool is_char_utf8_bom_EF(char c)
{
	return 0xEF==c;
}
bool is_char_utf8_bom_BB(char c)
{
	return 0xBB==c;
}
bool is_char_utf8_bom_BF(char c)
{
	return 0xBF==c;
}
bool is_char_white_space(unsigned char c)
{
	return isspace(c)!=0;
}
bool is_char_identifier(unsigned char c)
{
	return isalnum(c)!=0||'_'==c;
}
size_t split_std_string_with_substr(const std::string&source,const std::string&substr,std::string&before_substr,std::string&after_substr)
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
void strip_off_operators(const std::string&s,std::list<std::string>&phrase)
{
	assert(s.length()>0);
	ON_TRUE_RETURN(s.length()==0,);
	std::string scope_resolution_operator="::",before,after;
	if(split_std_string_with_substr(s,scope_resolution_operator,before,after)!=std::string::npos)
	{
		if(before.length()>0)
		{
			strip_off_operators(before,phrase);
		}
		phrase.push_back(scope_resolution_operator);
		if(after.length()>0)
		{
			strip_off_operators(after,phrase);
		}
	}
	else
	{
		enum State
		{
			Other,EF,BB
		};
		State state=Other;
		std::string r;
		for(size_t i=0;i<s.length();i++)
		{
			unsigned char c=s[i];
			switch(state)
			{
			case Other:
				if(is_char_utf8_bom_EF(c))
				{
					r=c;
					state=EF;
				}
				else
				{
					r=c;
					phrase.push_back(r);
				}
				break;
			case EF:
				if(is_char_utf8_bom_BB(c))
				{
					r+=c;
					state=BB;
				}
				else
				{
					phrase.push_back(r);
					fprintf(stderr,"Warning: EF [BB]?\n");
					r=c;
					phrase.push_back(r);
					state=Other;
				}
				break;
			case BB:
				if(is_char_utf8_bom_BF(c))
				{
					r+=c;
					phrase.push_back(r);
					state=Other;
				}
				else
				{
					phrase.push_back(r);
					fprintf(stderr,"Warning: EF BB [BF]?\n");
					r=c;
					phrase.push_back(r);
					state=Other;
				}
				break;
			}
		}
		if(state!=Other)
		{
			if(r.length()>0)
			{
				phrase.push_back(r);
			}
			fprintf(stderr,"Warning: EF [BB]? [BF]?\n");
		}
	}
}
void strip_off_2spaces_LEFT_operaters(const std::string&s,std::list<std::string>&phrase)
{
	assert(s.length()>0);
	ON_TRUE_RETURN(s.length()==0,);
	enum State
	{
		Other,Space
	};
	State state=Other;
	std::string r;
	for(size_t i=0;i<s.length();i++)
	{
		unsigned char c=s[i];
		switch(state)
		{
		case Other:
			if(is_char_white_space(c))
			{
				if(r.length())
				{
					strip_off_operators(r,phrase);
				}
				r=c;
				state=Space;
			}
			else
			{
				r+=c;
			}
			break;
		case Space:
			if(is_char_white_space(c))
			{
				r+=c;
			}
			else
			{
				if(r.length())
				{
					phrase.push_back(r);
				}
				r=c;
				state=Other;
			}
			break;
		}
	}
	if(r.length())
	{
		switch(state)
		{
		case Other:
			strip_off_operators(r,phrase);
			break;
		case Space:
			phrase.push_back(r);
			break;
		}
	}
}
void strip_off_3words_LEFT_spaces_operaters(const std::string&s,std::list<std::string>&phrase)
{
	assert(s.length()>0);
	ON_TRUE_RETURN(s.length()==0,);
	enum State
	{
		Other,Word
	};
	State state=Other;
	std::string r;
	for(size_t i=0;i<s.length();i++)
	{
		unsigned char c=s[i];
		switch(state)
		{
		case Other:
			if(is_char_identifier(c))
			{
				if(r.length())
				{
					strip_off_2spaces_LEFT_operaters(r,phrase);
				}
				r=c;
				state=Word;
			}
			else
			{
				r+=c;
			}
			break;
		case Word:
			if(is_char_identifier(c))
			{
				r+=c;
			}
			else
			{
				if(r.length())
				{
					phrase.push_back(r);
				}
				r=c;
				state=Other;
			}
			break;
		}
	}
	if(r.length())
	{
		switch(state)
		{
		case Other:
			strip_off_2spaces_LEFT_operaters(r,phrase);
			break;
		case Word:
			phrase.push_back(r);
			break;
		}
	}
}
void strip_off_4comments_directives_strings_chars_LEFT_words_spaces_operaters(const std::string&s,std::list<std::string>&phrase)
{
	assert(s.length()>0);
	ON_TRUE_RETURN(s.length()==0,);
	enum State
	{
		Other,
		CharQuote,
		CharQuoteBackSlash,
		Directive,
		DirectiveBackSlash,
		DirectiveCharQuote,
		DirectiveCharQuoteBackSlash,
		DirectiveInclude,
		DirectiveQuote,
		DirectiveQuoteBackSlash,
		DirectiveSlash,
		DirectiveSlashAsterisk,
		DirectiveSlashAsteriskAsterisk,
		DirectiveSlashSlash,
		Quote,
		QuoteBackSlash,
		At,
		AtQuote,
		Slash,
		SlashAsterisk,
		SlashAsteriskAsterisk,
		SlashSlash,
		SlashSlashBackSlash
	};
	State state=Other;
	std::string r,comment;
	for(size_t i=0;i<s.length();i++)
	{
		unsigned char c=s[i];
		switch(state)
		{
		case Other:
			if(c=='#')
			{
				if(r.length())
				{
					strip_off_3words_LEFT_spaces_operaters(r,phrase);
				}
				r='#';
				state=Directive;
			}
			else if(c=='\'')
			{
				if(r.length())
				{
					strip_off_3words_LEFT_spaces_operaters(r,phrase);
				}
				r='\'';
				state=CharQuote;
			}
			else if(c=='\"')
			{
				if(r.length())
				{
					strip_off_3words_LEFT_spaces_operaters(r,phrase);
				}
				r='\"';
				state=Quote;
			}
			else if(c=='@')
			{
				if(r.length())
				{
					strip_off_3words_LEFT_spaces_operaters(r,phrase);
				}
				r='@';
				state=At;
			}
			else if(c=='/')
			{
				state=Slash;
			}
			else if(c==0xEF||c==0xBB||c==0xBF)
			{
#ifdef _WIN64
				fprintf(stderr,"UTF-8 BOM(Byte Order Mark) char 0x%X, offset %I64u.\n",c,i);
#else
				fprintf(stderr,"UTF-8 BOM(Byte Order Mark) char 0x%X, offset %d.\n",c,i);
#endif
				r+=c;
			}
			else if(c&0x80)
			{
#ifdef _WIN64
				fprintf(stderr,"Unknown char 0x%X, offset %I64u.\n",c,i);
#else
				fprintf(stderr,"Unknown char 0x%X, offset %d.\n",c,i);
#endif
				r+=c;
			}
			else
			{
				r+=c;
			}
			break;
		case Slash:
			if(c=='/')
			{
				if(r.length())
				{
					strip_off_3words_LEFT_spaces_operaters(r,phrase);
				}
				r="//";
				state=SlashSlash;
			}
			else if(c=='*')
			{
				if(r.length())
				{
					strip_off_3words_LEFT_spaces_operaters(r,phrase);
				}
				r="/*";
				state=SlashAsterisk;
			}
			else
			{
				r+='/';
				r+=c;
				state=Other;
			}
			break;
		case SlashSlash:
			if(c=='\n')
			{
				if(r.length())
				{
					phrase.push_back(r);
				}
				r='\n';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else if(c=='\\')
			{
				r+='\\';
				state=SlashSlashBackSlash;
			}
			else
			{
				r+=c;
			}
			break;
		case SlashSlashBackSlash:
			r+=c;
			if(c!='\\')
			{
				state=SlashSlash;
			}
			break;
		case SlashAsterisk:
			if(c=='*')
			{
				r+='*';
				state=SlashAsteriskAsterisk;
			}
			else
			{
				r+=c;
			}
			break;
		case SlashAsteriskAsterisk:
			if(c=='/')
			{
				r+='/';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else if(c=='*')
			{
				r+=c;
			}
			else
			{
				r+=c;
				state=SlashAsterisk;
			}
			break;
		case At:
			if(c=='\"')
			{
				r+='\"';
				state=AtQuote;
			}
			else
			{
				i--;
				state=Other;
			}
			break;
		case AtQuote:
			if(c=='\"')
			{
				r+='\"';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else
			{
				r+=c;
			}
			break;
		case Quote:
			if(c=='\"')
			{
				r+='\"';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else if(c=='\\')
			{
				r+='\\';
				state=QuoteBackSlash;
			}
			else
			{
				r+=c;
			}
			break;
		case QuoteBackSlash:
			r+=c;
			state=Quote;
			break;
		case CharQuote:
			if(c=='\'')
			{
				r+='\'';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else if(c=='\\')
			{
				r+='\\';
				state=CharQuoteBackSlash;
			}
			else
			{
				r+=c;
			}
			break;
		case CharQuoteBackSlash:
			r+=c;
			state=CharQuote;
			break;
		case Directive:
			if(c=='/')
			{
				comment="";
				state=DirectiveSlash;
			}
			else if(c=='\\')
			{
				r+='\\';
				state=DirectiveBackSlash;
			}
			else if(c=='\n')
			{
				if(r.length())
				{
					phrase.push_back(r);
				}
				r='\n';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else if(c=='\"')
			{
				r+='\"';
				state=DirectiveQuote;
			}
			else if(c=='\'')
			{
				r+='\'';
				state=DirectiveCharQuote;
			}
			else if(c=='<')
			{
				r+='<';
				if(r.find("#include")==0)
				{
					state=DirectiveInclude;
				}
			}
			else
			{
				r+=c;
			}
			break;
		case DirectiveBackSlash:
			r+=c;
			state=Directive;
			break;
		case DirectiveSlash:
			if(c=='/')
			{
				comment="//";
				state=DirectiveSlashSlash;
			}
			else if(c=='*')
			{
				comment="/*";
				state=DirectiveSlashAsterisk;
			}
			else
			{
				r+='/';
				r+=c;
				state=Directive;
			}
			break;
		case DirectiveSlashSlash:
			if(c=='\n')
			{
				if(comment.length())
				{
					phrase.push_back(comment);
					comment="";
				}
				if(r.length())
				{
					phrase.push_back(r);
				}
				r='\n';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else
			{
				comment+=c;
			}
			break;
		case DirectiveSlashAsterisk:
			comment+=c;
			if(c=='*')
			{
				state=DirectiveSlashAsteriskAsterisk;
			}
			break;
		case DirectiveSlashAsteriskAsterisk:
			if(c=='/')
			{
				r+=' ';
				comment+='/';
				phrase.push_back(comment);
				comment="";
				state=Directive;
			}
			else if(c!='*')
			{
				comment+=c;
				state=DirectiveSlashAsterisk;
			}
			break;
		case DirectiveQuote:
			if(c=='\"')
			{
				r+='\"';
				state=Directive;
			}
			else if(c=='\\')
			{
				r+='\\';
				state=DirectiveQuoteBackSlash;
			}
			else
			{
				r+=c;
			}
			break;
		case DirectiveQuoteBackSlash:
			r+=c;
			state=DirectiveQuote;
			break;
		case DirectiveCharQuote:
			if(c=='\'')
			{
				r+='\'';
				state=Directive;
			}
			else if(c=='\n')
			{
				if(r.length())
				{
					phrase.push_back(r);
				}
				r='\n';
				phrase.push_back(r);
				r="";
				state=Other;
			}
			else if(c=='\\')
			{
				r+='\\';
				state=DirectiveCharQuoteBackSlash;
			}
			else
			{
				r+=c;
			}
			break;
		case DirectiveCharQuoteBackSlash:
			r+=c;
			state=DirectiveCharQuote;
			break;
		case DirectiveInclude:
			if(c=='>')
			{
				r+='>';
				state=Directive;
			}
			else
			{
				r+=c;
			}
			break;
		}
	}
	if(state==Slash||state==DirectiveSlash)
	{
		r+='/';
	}
	if(r.length())
	{
		if(state==Other)
		{
			strip_off_3words_LEFT_spaces_operaters(r,phrase);
		}
		else
		{
			phrase.push_back(r);
		}
	}
}
ZZXCF_TOKENIZER_API void tokenize(const std::string&s,std::list<std::string>&phrase)
{
	strip_off_4comments_directives_strings_chars_LEFT_words_spaces_operaters(s,phrase);
}
