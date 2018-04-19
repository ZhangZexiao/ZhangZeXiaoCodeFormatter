#pragma once
#include <algorithm>
#include <cctype>
#include <functional> 
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>
#include <io.h>
#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>
#include <comdef.h>	
#include <atlbase.h>
#include <crtdbg.h>
namespace command_line_namespace
{
	class command_line_option_parser
	{
	public:
		typedef std::list<command_line_option_parser*>type_options;
	private:
		std::string m_descriptive_name;
		std::string m_simplified_name;
		std::string m_description;
		bool m_is_turned_on;
		static type_options m_total_command_line_options;
	private:
		command_line_option_parser();
		command_line_option_parser(const command_line_option_parser&);
		command_line_option_parser&operator=(const command_line_option_parser&);
	public:
		command_line_option_parser(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on=false);
		virtual void turn_on();
		virtual void turn_off();
		virtual bool is_my_name(const std::string&name);
		virtual std::string get_descriptive_name()const;
		virtual std::string get_simplified_name()const;
		std::string get_description()const;
		bool is_turned_on()const;
		std::string get_usage()const;
		void print_usage()const;
		std::string get_status_statement()const;
		void print_state()const;
		static type_options&get_total_command_line_options();
		static std::string get_descriptive_names();
		static std::string get_simplified_names();
		static std::string get_descriptive_names_in_bracket();
		static std::string get_simplified_names_in_bracket();
		static std::string get_usages();
		static bool is_option_name(std::string&given_std_string);
		static bool is_unknown_option_name(std::string&given_std_string);
		static command_line_option_parser*get_option(std::string&given_std_string);
		static void print_warning_message_unknown_option(std::string unknown_option);
		static void parse(int argc,char**argv,int&i);
	};
	class option_with_padding_digits:
	public command_line_option_parser
	{
	public:
		option_with_padding_digits(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on=false);
		virtual void turn_on()=0;
		virtual void turn_off()=0;
		virtual bool is_my_name(const std::string&name);
		virtual std::string get_descriptive_name()const;
		virtual std::string get_simplified_name()const;
	protected:
		std::string m_padding_digits;
	private:
		std::string m_padding_digits_regex
		{
			"[0-9]*"
		};
	};
	class option_number_of_padding_spaces_per_level:
	public option_with_padding_digits
	{
	public:
		static size_t number_of_padding_chars_per_level;
		static unsigned char padding_char;
		option_number_of_padding_spaces_per_level(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on=false);
		virtual void turn_on();
		virtual void turn_off();
	};
	class zzxcf_command_line_option_number_of_threads:
	public option_with_padding_digits
	{
	public:
		static DWORD number_of_threads;
		zzxcf_command_line_option_number_of_threads(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on=false);
		virtual void turn_on();
		virtual void turn_off();
	};
	class zzxcf_command_line_option_number_of_commas:
	public option_with_padding_digits
	{
	public:
		static size_t number_of_commas;
		zzxcf_command_line_option_number_of_commas(const std::string descriptive_name,const std::string simplified_name,const std::string description,bool is_turned_on=false);
		virtual void turn_on();
		virtual void turn_off();
	};
	extern zzxcf_command_line_option_number_of_commas option_number_of_commas;
	extern zzxcf_command_line_option_number_of_threads option_number_of_threads;
	extern option_number_of_padding_spaces_per_level option_do_not_padding_with_tab;
	extern command_line_option_parser option_print_help;
	extern command_line_option_parser option_accumulate_map;
	extern command_line_option_parser option_cleanup_orig_files;
	extern command_line_option_parser option_convert_to_bridge_pattern;
	extern command_line_option_parser option_do_not_add_line_feed_after_function_call_alike;
	extern command_line_option_parser option_do_not_add_line_feed_after_single_colon;
	extern command_line_option_parser option_do_not_add_line_feed_after_template_declaration;
	extern command_line_option_parser option_do_not_add_line_feed_if_more_than_10_commas_in_one_line;
	extern command_line_option_parser option_do_not_apply_new_alignment;
	extern command_line_option_parser option_do_not_backup_files;
	extern command_line_option_parser option_do_not_convert_path_separator_from_windows_style_backslash_to_linux_style_slash_in_header_file_include_directive;
	extern command_line_option_parser option_do_not_print_logo;
	extern command_line_option_parser option_do_not_redirect_stderr_to_file;
	extern command_line_option_parser option_do_not_strip_duplicate_semicolon;
	extern command_line_option_parser option_do_not_strip_line_feed_between_right_curly_bracket_and_semicolon;
	extern command_line_option_parser option_do_not_strip_meaningless_comments;
	extern command_line_option_parser option_do_not_strip_meaningless_pragmas;
	extern command_line_option_parser option_do_not_strip_redundant_white_spaces;
	extern command_line_option_parser option_do_not_strip_specific_conditional_complilation;
	extern command_line_option_parser option_do_not_surround_single_sentence_after_reserve_word_for_with_curly_bracket;
	extern command_line_option_parser option_do_not_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket;
	extern command_line_option_parser option_do_not_touch_files;
	extern command_line_option_parser option_do_not_touch_readonly_files;
	extern command_line_option_parser option_do_not_touch_unchanged_files;
	extern command_line_option_parser option_do_not_touch_utf8_encoded_files;
	extern command_line_option_parser option_do_not_use_multithreads;
	extern command_line_option_parser option_do_not_search_for_absolute_file_path;
	extern command_line_option_parser option_export_maps;
	extern command_line_option_parser option_format_before_saving_function;
	extern command_line_option_parser option_import_maps;
	extern command_line_option_parser option_insert_separator_between_phrases_when_saving_interim_result;
	extern command_line_option_parser option_do_not_print_statistical_data;
	extern command_line_option_parser option_notify_user_when_job_done;
	extern command_line_option_parser option_redirect_stdout_to_file;
	extern command_line_option_parser option_replace_ifndef_define_endif_directives_with_pragma_once;
	extern command_line_option_parser option_restore_previous_version;
	extern command_line_option_parser option_save_functions_into_sqlite3_database;
	extern command_line_option_parser option_save_interim_result;
	extern command_line_option_parser option_search_subdirectories;
	extern command_line_option_parser option_strip_comments;
	extern command_line_option_parser option_strip_utf8_bom;
	extern command_line_option_parser option_surround_if_clause_after_reserve_word_else_with_curly_bracket;
}
