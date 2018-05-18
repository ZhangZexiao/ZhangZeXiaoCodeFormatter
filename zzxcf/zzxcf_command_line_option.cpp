#include "zzxcf_command_line_option.h"
namespace command_line_namespace
{
	size_t zzxcf_command_line_option_number_of_commas::number_of_commas=10;
	command_line_option_parser::type_options command_line_option_parser::m_total_command_line_options;
	size_t option_number_of_padding_spaces_per_level::number_of_padding_chars_per_level=1;
	unsigned char option_number_of_padding_spaces_per_level::padding_char='\t';
	DWORD zzxcf_command_line_option_number_of_threads::number_of_threads=1;
	extern zzxcf_command_line_option_number_of_commas option_number_of_commas("-number_of_commas","-number_of_commas","Customize the number of commas for formatting files.");
	extern zzxcf_command_line_option_number_of_threads option_number_of_threads("-number_of_threads","-number_of_threads","Customize the number of threads for formatting files when multithread mode is turned on, exceed 64 at your own risk.");
	extern option_number_of_padding_spaces_per_level option_do_not_padding_with_tab("-number_of_padding_spaces_per_level_option_do_not_padding_with_tab","-number_of_padding_spaces_per_level_option_do_not_padding_with_tab","Indent lines with space ' '(Default: tab '\\t'), [0-9]* spaces(' ') per tab('\\t').");
	extern command_line_option_parser option_print_help("-help","-h","Print help message.");
	extern command_line_option_parser option_accumulate_map("-accumulate_map","-accumulate","Accumulate maps.");
	extern command_line_option_parser option_cleanup_orig_files("-cleanup_orig_files","-cleanup","Delete all matched *.orig files.");
	extern command_line_option_parser option_convert_to_bridge_pattern("-convert_to_bridge_pattern","-bridge","Generate impl file.");
	extern command_line_option_parser option_do_not_add_line_feed_after_function_call_alike("-do_not_add_line_feed_after_function_call_alike","-do_not_add_line_feed_after_function_call_alike","Don't add line feed after function call.");
	extern command_line_option_parser option_do_not_add_line_feed_after_single_colon("-do_not_add_line_feed_after_single_colon","-do_not_add_line_feed_after_single_colon","Don't add line feed after single colon.");
	extern command_line_option_parser option_do_not_add_line_feed_after_template_declaration("-do_not_add_line_feed_after_template_declaration","-do_not_add_line_feed_after_template_declaration","Don't add line feed after template declaration.");
	extern command_line_option_parser option_do_not_add_line_feed_if_more_than_10_commas_in_one_line("-do_not_add_line_feed_if_more_than_10_commas_in_one_line","-do_not_add_line_feed_if_more_than_10_commas_in_one_line","Don't add line feed when more than 10 commas one line.");
	extern command_line_option_parser option_do_not_apply_new_alignment("-do_not_apply_new_alignment","-do_not_apply_new_alignment","Don't format file content.");
	extern command_line_option_parser option_do_not_backup_files("-do_not_backup_files","-nobackup","Don't backup files.");
	extern command_line_option_parser option_do_not_convert_path_separator_from_windows_style_backslash_to_linux_style_slash_in_header_file_include_directive("-do_not_convert_path_separator_from_windows_style_backslash_to_linux_style_slash_in_header_file_include_directive","-no_backslash_to_slash","Don't change from \\ to / in #include line.");
	extern command_line_option_parser option_do_not_print_logo("-do_not_print_logo","-nologo","Don't print logo.");
	extern command_line_option_parser option_do_not_redirect_stderr_to_file("-do_not_redirect_stderr_to_file","-no_stderr_to_file","Don't save stderr to log file .stderr");
	extern command_line_option_parser option_do_not_strip_duplicate_semicolon("-do_not_strip_duplicate_semicolon","-do_not_strip_duplicate_semicolon","Don't remove redundant semicolons.");
	extern command_line_option_parser option_do_not_strip_line_feed_between_right_curly_bracket_and_semicolon("-do_not_strip_line_feed_between_right_curly_bracket_and_semicolon","-do_not_strip_line_feed_between_right_curly_bracket_and_semicolon","Don't remove line feed between right curly bracket and semicolon.");
	extern command_line_option_parser option_do_not_strip_meaningless_comments("-do_not_strip_meaningless_comments","-do_not_strip_meaningless_comments","Suppress remove empty comments //.");
	extern command_line_option_parser option_do_not_strip_meaningless_pragmas("-do_not_strip_meaningless_pragmas","-do_not_strip_meaningless_pragmas","Don't remove #line, #pragma region, #pragma endregion.");
	extern command_line_option_parser option_do_not_strip_redundant_white_spaces("-do_not_strip_redundant_white_spaces","-no_strip_space","Don't remove redundant white spaces.");
	extern command_line_option_parser option_do_not_strip_specific_conditional_complilation("-do_not_strip_specific_conditional_complilation","-do_not_strip_specific_conditional_complilation","Do_not_strip_specific_conditional_complilation.");
	extern command_line_option_parser option_do_not_surround_single_sentence_after_reserve_word_for_with_curly_bracket("-do_not_surround_single_sentence_after_reserve_word_for_with_curly_bracket","-do_not_surround_single_sentence_after_reserve_word_for_with_curly_bracket","Don't surround \"FOR\" code block with curly bracket.");
	extern command_line_option_parser option_do_not_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket("-do_not_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket","-do_not_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket","Don't surround \"IF\" \"ELSE\" code block with curly bracket.");
	extern command_line_option_parser option_do_not_touch_files("-do_not_touch_files","-dryrun","Don't touch files.");
	extern command_line_option_parser option_do_not_touch_readonly_files("-do_not_touch_readonly_files","-noreadonly","Don't touch readonly files.");
	extern command_line_option_parser option_do_not_touch_unchanged_files("-do_not_touch_unchanged_files","-nounchanged","Don't touch unchanged files.");
	extern command_line_option_parser option_do_not_touch_utf8_encoded_files("-do_not_touch_utf8_encoded_files","-noutf8","Don't touch UTF-8 files.");
	extern command_line_option_parser option_do_not_use_multithreads("-do_not_use_multithreads","-nomt","Don't using multi threading.");
	extern command_line_option_parser option_do_not_search_for_absolute_file_path("-do_not_search_for_absolute_file_path","-do_not_search_for_absolute_file_path","Don't search for absolute file path.");
	extern command_line_option_parser option_export_maps("-export_map","-export","Export maps.");
	extern command_line_option_parser option_format_before_saving_function("-format_before_saving_function","-format_before_saving_function","Tidy function before saving into sqlite3 database.");
	extern command_line_option_parser option_import_maps("-import_map","-import","Import maps.");
	extern command_line_option_parser option_insert_separator_between_phrases_when_saving_interim_result("-insert_separator_between_phrases_when_saving_interim_result","-insert_separator_between_phrases_when_saving_interim_result","Insert separator when saving intermediates.");
	extern command_line_option_parser option_do_not_print_statistical_data("-no_statistic","-ns","Don't print statistic.");
	extern command_line_option_parser option_notify_user_when_job_done("-notify_user_when_job_done","-notify","Notify user when the job done.");
	extern command_line_option_parser option_redirect_stdout_to_file("-redirect_stdout_to_file","-stdout_to_file","Save stdout to log file .stdout");
	extern command_line_option_parser option_replace_ifndef_define_endif_directives_with_pragma_once("-replace_ifndef_define_endif_directives_with_pragma_once","-apply_pragma_once","Replace conditional compilation with #pragma once.");
	extern command_line_option_parser option_restore_previous_version("-restore_previous_status","-restore","Restore all matched files. Latest matched orig files will be used to replace matched files.");
	extern command_line_option_parser option_save_functions_into_sqlite3_database("-save_function_into_database","-save_function_into_database","Save functions into sqlite3 database.");
	extern command_line_option_parser option_save_interim_result("-save_interim_result","-save_interim_result","Save intermediates for debugging.");
	extern command_line_option_parser option_search_subdirectories("-search_subdirectories","-ss","Search files in subdirectories.");
	extern command_line_option_parser option_strip_comments("-strip_comments","-rc","Remove comments // and /**/.");
	extern command_line_option_parser option_strip_utf8_bom("-strip_utf8_bom","-strip_utf8_bom","Remove UTF-8 BOM 0xEF 0xBB 0xBF.");
	extern command_line_option_parser option_surround_if_clause_after_reserve_word_else_with_curly_bracket("-surround_if_clause_after_reserve_word_else_with_curly_bracket","-surround_if_clause_after_reserve_word_else_with_curly_bracket","Surround \"IF\" branch of \"ELSE IF\" with curly bracket.");
	command_line_option_parser::command_line_option_parser(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on)
	{
		m_descriptive_name=descriptive_name;
		m_simplified_name=simplified_name;
		m_description=description;
		m_is_turned_on=is_turned_on;
		m_total_command_line_options.push_back(this);
	}
	void command_line_option_parser::turn_on()
	{
		m_is_turned_on=true;
	}
	void command_line_option_parser::turn_off()
	{
		m_is_turned_on=false;
	}
	bool command_line_option_parser::is_my_name(const std::string&name)
	{
		return name==m_descriptive_name||name==m_simplified_name;
	}
	std::string command_line_option_parser::get_descriptive_name()const
	{
		return m_descriptive_name;
	}
	std::string command_line_option_parser::get_simplified_name()const
	{
		return m_simplified_name;
	}
	std::string command_line_option_parser::get_description()const
	{
		return m_description;
	}
	bool command_line_option_parser::is_turned_on()const
	{
		return m_is_turned_on;
	}
	std::string command_line_option_parser::get_usage()const
	{
		return get_descriptive_name()+"|"+get_simplified_name()+"\n\t"+get_description()+"\n\n";
	}
	void command_line_option_parser::print_usage()const
	{
		printf("%s",get_usage().c_str());
	}
	std::string command_line_option_parser::get_status_statement()const
	{
		const std::string status_statement_template
		{
			"Option \"%s|%s\" is turned %s.\n"
		};
		const std::string on
		{
			"on"
		};
		const std::string off
		{
			"off"
		};
		const size_t buffer_count=1024;
		char status_statement[buffer_count];
		::sprintf_s(status_statement,buffer_count,status_statement_template.c_str(),m_descriptive_name.c_str(),m_simplified_name.c_str(),m_is_turned_on?on.c_str():off.c_str());
		return std::string(status_statement);
	}
	void command_line_option_parser::print_state()const
	{
		printf("%s",get_status_statement().c_str());
	}
	command_line_option_parser::type_options&command_line_option_parser::get_total_command_line_options()
	{
		return m_total_command_line_options;
	}
	std::string command_line_option_parser::get_descriptive_names()
	{
		std::string r;
		for(type_options::iterator it=m_total_command_line_options.begin();it!=m_total_command_line_options.end();it++)
		{
			r+=(*it)->get_descriptive_name();
			r+=" ";
		}
		return r;
	}
	std::string command_line_option_parser::get_simplified_names()
	{
		std::string r;
		for(type_options::iterator it=m_total_command_line_options.begin();it!=m_total_command_line_options.end();it++)
		{
			r+=(*it)->get_simplified_name();
			r+=" ";
		}
		return r;
	}
	std::string command_line_option_parser::get_descriptive_names_in_bracket()
	{
		std::string r;
		for(type_options::iterator it=m_total_command_line_options.begin();it!=m_total_command_line_options.end();it++)
		{
			r+="[";
			r+=(*it)->get_descriptive_name();
			r+="]";
			r+=" ";
		}
		return r;
	}
	std::string command_line_option_parser::get_simplified_names_in_bracket()
	{
		std::string r;
		for(type_options::iterator it=m_total_command_line_options.begin();it!=m_total_command_line_options.end();it++)
		{
			r+="[";
			r+=(*it)->get_simplified_name();
			r+="]";
			r+=" ";
		}
		return r;
	}
	std::string command_line_option_parser::get_usages()
	{
		std::string r;
		for(type_options::iterator it=m_total_command_line_options.begin();it!=m_total_command_line_options.end();it++)
		{
			r+=(*it)->get_usage();
		}
		return r;
	}
	bool command_line_option_parser::is_option_name(std::string&given_std_string)
	{
		for(type_options::iterator it=m_total_command_line_options.begin();it!=m_total_command_line_options.end();it++)
		{
			if((*it)->is_my_name(given_std_string))
			{
				return true;
			}
		}
		return false;
	}
	bool command_line_option_parser::is_unknown_option_name(std::string&given_std_string)
	{
		if(!is_option_name(given_std_string))
		{
			if(given_std_string.find("-")==0)
			{
				return true;
			}
		}
		return false;
	}
	command_line_option_parser*command_line_option_parser::get_option(std::string&given_std_string)
	{
		for(type_options::iterator it=m_total_command_line_options.begin();it!=m_total_command_line_options.end();it++)
		{
			if((*it)->is_my_name(given_std_string))
			{
				return*it;
			}
		}
		return 0;
	}
	void command_line_option_parser::print_warning_message_unknown_option(std::string unknown_option)
	{
		fprintf(stderr,"Unknown option %s\n",unknown_option.c_str());
	}
	void command_line_option_parser::parse(int argc,char**argv,int&i)
	{
		for(;i<argc;i++)
		{
			std::string arg=argv[i];
			if(is_option_name(arg))
			{
				command_line_option_parser*option_ptr=get_option(arg);
				if(option_ptr)
				{
					option_ptr->turn_on();
					option_ptr->print_state();
				}
			}
			else if(is_unknown_option_name(arg))
			{
				print_warning_message_unknown_option(arg);
			}
			else
			{
				break;
			}
		}
		printf("\n");
	}
	option_with_padding_digits::option_with_padding_digits(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on):
	command_line_option_parser(descriptive_name,simplified_name,description,is_turned_on)
	{
	}
	bool option_with_padding_digits::is_my_name(const std::string&name)
	{
		if(name.find(command_line_option_parser::get_descriptive_name())==0)
		{
			if(name.length()>command_line_option_parser::get_descriptive_name().length()&& ::isdigit(name[command_line_option_parser::get_descriptive_name().length()]))
			{
				m_padding_digits=name.substr(command_line_option_parser::get_descriptive_name().length());
				return true;
			}
		}
		if(name.find(command_line_option_parser::get_simplified_name())==0)
		{
			if(name.length()>command_line_option_parser::get_simplified_name().length()&& ::isdigit(name[command_line_option_parser::get_simplified_name().length()]))
			{
				m_padding_digits=name.substr(command_line_option_parser::get_simplified_name().length());
				return true;
			}
		}
		return false;
	}
	std::string option_with_padding_digits::get_descriptive_name()const
	{
		return command_line_option_parser::get_descriptive_name()+m_padding_digits_regex;
	}
	std::string option_with_padding_digits::get_simplified_name()const
	{
		return command_line_option_parser::get_simplified_name()+m_padding_digits_regex;
	}
	option_number_of_padding_spaces_per_level::option_number_of_padding_spaces_per_level(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on):
	option_with_padding_digits(descriptive_name,simplified_name,description,is_turned_on)
	{
	}
	void option_number_of_padding_spaces_per_level::turn_on()
	{
		command_line_option_parser::turn_on();
		padding_char=' ';
		if(m_padding_digits.length()>0&& ::isdigit(m_padding_digits[0]))
		{
			number_of_padding_chars_per_level=atoi(m_padding_digits.c_str());
		}
		else
		{
			number_of_padding_chars_per_level=4;
		}
	}
	void option_number_of_padding_spaces_per_level::turn_off()
	{
		command_line_option_parser::turn_off();
		padding_char='\t';
		number_of_padding_chars_per_level=1;
	}
	zzxcf_command_line_option_number_of_threads::zzxcf_command_line_option_number_of_threads(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on):
	option_with_padding_digits(descriptive_name,simplified_name,description,is_turned_on)
	{
	}
	void zzxcf_command_line_option_number_of_threads::turn_on()
	{
		command_line_option_parser::turn_on();
		if(m_padding_digits.length()>0&& ::isdigit(m_padding_digits[0]))
		{
			number_of_threads=atoi(m_padding_digits.c_str());
		}
		else
		{
			number_of_threads=1;
		}
	}
	void zzxcf_command_line_option_number_of_threads::turn_off()
	{
		command_line_option_parser::turn_off();
		number_of_threads=1;
	}
	zzxcf_command_line_option_number_of_commas::zzxcf_command_line_option_number_of_commas(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on):
	option_with_padding_digits(descriptive_name,simplified_name,description,is_turned_on)
	{
	}
	void zzxcf_command_line_option_number_of_commas::turn_on()
	{
		command_line_option_parser::turn_on();
		if(m_padding_digits.length()>0&& ::isdigit(m_padding_digits[0]))
		{
			number_of_commas=atoi(m_padding_digits.c_str());
		}
		else
		{
			number_of_commas=10;
		}
	}
	void zzxcf_command_line_option_number_of_commas::turn_off()
	{
		command_line_option_parser::turn_off();
		number_of_commas=10;
	}
}
