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
#include "zzxcf_command_line_option.h"
#include "zzxcf_file_operation.h"
#include "zzxcf_statistic_utility.h"
#include "zzxcf_console_utility.h"
#include "../sqlite/sqlite3.h"
namespace format_utility_namespace
{
	typedef enum
	{
		eKeep,k_delete,k_simplified,eReplace,k_delete_if_block,k_delete_else_block
	}
	type_map_value_operation;
	typedef struct
	{
		__int64 count;
		type_map_value_operation operation;
		std::string replacement;
	}
	type_map_value;
	typedef std::string zzxcf_token;
	typedef std::list<zzxcf_token>zzxcf_phrase;
	typedef std::list<zzxcf_phrase>zzxcf_section;
	typedef std::map<std::string,type_map_value>zzxcf_map;
	typedef std::pair<__int64,std::string>zzxcf_int64_string_pair;
	typedef std::vector<zzxcf_int64_string_pair>type_count_string_vector;
	typedef std::list<std::string>type_directive_list;
	typedef struct struct_progress_data
	{
		const char**file_paths;
		size_t number_of_files;
	}
	struct_progress_data;
	struct class_info
	{
		std::string body;
		std::string class_name;
		std::string declspec_content;
		std::string modifier_and_inherited_class_names;
	};
	typedef struct struct_function_details
	{
		std::string file_name;
		size_t offset;
		size_t number_of_lines;
		std::string function_return_type;
		std::string function_name;
		std::string function_declaration;
		std::string function_body;
	}
	struct_function_details;
	typedef std::map<std::string,struct_function_details>FunctionsDetails;
	void retry_format_files(const char**file_paths,size_t file_paths_length);
	void format_files_in_process(const std::list<std::string>&file_paths);
	unsigned long __stdcall format_files_in_thread(void*file_paths);
	unsigned long __stdcall print_progress_in_thread(void*p_progress_data);
	HANDLE startup_print_progress_thread(struct_progress_data*p_progress_data);
	void startup_format_files_threads(const char**file_paths,size_t number_of_files,HANDLE*thread_handles,DWORD&thread_handles_length);
	void format_files_in_threads(const std::list<std::string>&file_path_list);
	void initialize_sqlite3_functions_details(const std::string fileName);
	void finalize_sqlite3_functions_details();
	void print_functions(const std::list<std::string>&functions);
	void extract_functions(const zzxcf_phrase&phrase,std::list<std::string>&functions);
	std::string format_function(const std::string&function);
	void format_functions(std::list<std::string>&functions);
	size_t get_function_line_count(const std::string&function);
	void extract_function_details(const std::string&function,std::string&typeOfReturn,std::string&nameOfFunction,std::string&declarationOfFunction);
	void extract_functions_details(const std::string&fileName,const std::string&fileContent,const std::list<std::string>&functions,FunctionsDetails&functionsDetails);
	void save_functions_details_into_database(const FunctionsDetails&functionsDetails);
	void save_functions_from_source_file(const std::string&fileName);
	void save_functions_from_source_files(const std::list<std::string>&fileList);
	unsigned long __stdcall save_functions(void*files);
	void startup_threads_for_saving_functions(const char**files,size_t numberOfFiles,HANDLE*threads,DWORD&numberOfThreads);
	void save_functions_using_threads(const std::list<std::string>&fileList);
	void to_lower_string(std::string&s);
	bool is_cpp_file(const std::string&fileName);
	std::string get_name_ext(const std::string&fileName);
	std::string generate_h_file_name_from_cpp_file_name(const std::string&fileName);
	std::string generate_impl_file_name(const std::string&fileName);
	void copy_file(const std::string&from,const std::string&to);
	void let_user_decide_remove_copy(const std::string&file,const std::string&impl_file);
	size_t count_specific_token(const zzxcf_phrase&phrase,const zzxcf_token&token);
	void extract_classes(const zzxcf_phrase&phrase,std::list<std::string>&classList,std::list<class_info>&class_info_list);
	void print_class_info_list(const std::list<class_info>&cil);
	void delete_impl_file_name(std::list<std::string>&fileList);
	void insert_string(zzxcf_phrase&phrase,zzxcf_phrase::iterator where,const std::string&str);
	void edit_impl_h_file(const std::string&impl_h_file,const class_info&class_info);
	void edit_impl_cpp_file(const std::string&impl_cpp_file,const class_info&class_info,const std::string&impl_h_file,const std::string&h_file);
	void edit_h_file(const std::string&h_file,const class_info&class_info);
	void edit_cpp_file(const std::string&cpp_file,const class_info&class_info,const std::string&impl_h_file,const std::string&h_file);
	void save_functions_into_sqlite3_database(std::list<std::string>&to_be_formatted_files);
	void delete_all_orig_files(int argc,char**argv,int index);
	void restore_previous_version(std::list<std::string>&to_be_formatted_files);
	void convert_to_bridge_pattern(std::list<std::string>&to_be_formatted_files);
	void concat_tokens_0separator(const zzxcf_phrase&phrase,zzxcf_section&section);
	void concat_tokens_2round_bracket(zzxcf_section&section);
	void concat_tokens_3sentence(zzxcf_section&section);
	bool does_contain_quoted_string(const zzxcf_phrase&phrase);
	bool does_contain_bracket(const zzxcf_phrase&phrase);
	void take_out_redundant_white_spaces(zzxcf_section&section);
	bool is_phrase_label(const zzxcf_phrase&phrase);
	bool is_char_utf8_bom_EF(const char c);
	bool is_char_utf8_bom_BB(const char c);
	bool is_char_utf8_bom_BF(const char c);
	bool is_char_utf8_bom(const char c);
	bool is_char_identifier(const unsigned char c);
	bool is_char_white_space(const unsigned char c);
	bool is_token_identifier(const zzxcf_token&token);
	bool is_token_digit(const zzxcf_token&token);
	bool is_token_white_space(const zzxcf_token&token);
	bool is_token_left_round_bracket(const zzxcf_token&token);
	bool is_token_right_round_bracket(const zzxcf_token&token);
	bool is_token_round_bracket(const zzxcf_token&token);
	bool is_token_left_square_bracket(const zzxcf_token&token);
	bool is_token_right_square_bracket(const zzxcf_token&token);
	bool is_token_square_bracket(const zzxcf_token&token);
	bool is_token_left_curly_bracket(const zzxcf_token&token);
	bool is_token_right_curly_bracket(const zzxcf_token&token);
	bool is_token_curly_bracket(const zzxcf_token&token);
	bool is_token_semicolon(const zzxcf_token&token);
	bool is_token_comma(const zzxcf_token&token);
	bool is_token_quoted_string(const zzxcf_token&token);
	bool is_token_quoted_char(const zzxcf_token&token);
	bool is_token_stand_alone(const zzxcf_token&token);
	bool is_token_scope_resolution_operator(const zzxcf_token&token);
	bool is_token_c_style_comment(const zzxcf_token&token);
	bool is_token_cpp_style_comment(const zzxcf_token&token);
	bool is_token_comment(const zzxcf_token&token);
	bool is_token_empty_comment(const zzxcf_token&token);
	bool is_token_directive(const zzxcf_token&token);
	bool is_token_directive_define(const zzxcf_token&token);
	bool is_token_directive_include(const zzxcf_token&token);
	bool is_token_directive_line(const zzxcf_token&token);
	bool is_token_directive_pragma_region_endregion(const zzxcf_token&token);
	bool is_token_white_space_comment_directive(const zzxcf_token&token);
	bool is_token_asm(const zzxcf_token&token);
	bool is_token_line_continuation(const zzxcf_token&token);
	bool is_token_single_colon(const zzxcf_token&token);
	bool is_token_question_mark(const zzxcf_token&token);
	bool is_token_separator(const zzxcf_token&token);
	bool is_phrase_separator(const zzxcf_phrase&phrase);
	bool is_phrase_comment(const zzxcf_phrase&phrase);
	bool is_phrase_template(const zzxcf_phrase&phrase);
	bool is_phrase_directive_line(const zzxcf_phrase&phrase);
	bool is_phrase_directive_include(const zzxcf_phrase&phrase);
	bool is_phrase_contains_token_single_colon(const zzxcf_phrase&phrase);
	bool is_phrase_contains_number_of_token_commas(const zzxcf_phrase&phrase);
	bool is_phrase_separator_char(const zzxcf_phrase&phrase,unsigned char c);
	bool is_phrase_directive(const zzxcf_phrase&phrase);
	bool is_phrase_left_curly_bracket(const zzxcf_phrase&phrase);
	bool is_phrase_right_curly_bracket(const zzxcf_phrase&phrase);
	bool is_phrase_left_round_bracket(const zzxcf_phrase&phrase);
	bool is_phrase_right_round_bracket(const zzxcf_phrase&phrase);
	bool is_phrase_semicolon(const zzxcf_phrase&phrase);
	bool is_phrase_line_continuation(const zzxcf_phrase&phrase);
	bool is_phrase_single_colon(const zzxcf_phrase&phrase);
	int concat_tokens_6indent(zzxcf_section&section);
	bool is_char_asterisk_slash_hash(const unsigned char c);
	std::string&trim_left_star_backslash(std::string&s);
	std::string&trim_right_star_backslash(std::string&s);
	std::string&trim_star_backslash(std::string&s);
	std::string&transform_to_lower_string(std::string&s);
	std::string&trim_left(std::string&s);
	std::string&trim_right(std::string&s);
	std::string&trim(std::string&s);
	std::vector<std::string>&split(const std::string&s,char delim,std::vector<std::string>&elems);
	std::vector<std::string>split(const std::string&s,char delim);
	zzxcf_token simplify_comment(zzxcf_token&comment,bool flag_case_sensitive=false);
	zzxcf_token simplify_directive(zzxcf_token&directive,bool flag_case_sensitive);
	void replace_ifndef_define_endif_directives_with_pragma_once(zzxcf_section&section);
	int compare_zzxcf_int64_string_pair(zzxcf_int64_string_pair&pre,zzxcf_int64_string_pair&cur);
	void export_comment_map(zzxcf_map&comment_map);
	void export_id_map(zzxcf_map&id_map);
	void export_directive_map(zzxcf_map&directive_map,type_directive_list&directive_list);
	void import_comment_map(zzxcf_map&comment_map);
	void import_id_map(zzxcf_map&id_map,zzxcf_map&inverted_id_map);
	void import_directive_map(zzxcf_map&directive_map);
	void accumulate_map_comment(zzxcf_section&section,zzxcf_map&comment_map);
	bool replace_identifier(zzxcf_phrase::iterator&pointer_to_token,zzxcf_map&identifier_map);
	void replace_identifier_without_head(zzxcf_phrase::iterator&it,std::string&head,format_utility_namespace::zzxcf_map&identifier_map);
	void replace_identifier_without_head_and_foot(zzxcf_phrase::iterator&it,std::string&head,std::string&foot,format_utility_namespace::zzxcf_map&identifier_map);
	bool is_phrase_block_separator(const zzxcf_phrase&phrase);
	size_t split_string_with_substr(const std::string&source,const std::string&substr,std::string&before_substr,std::string&after_substr);
	const std::string&get_count_line_format();
	void write_section_to_file(const zzxcf_section&section,const std::string&fileName);
	std::string replace_identifier_in_raw_phrase(std::string&raw_phrase,format_utility_namespace::zzxcf_map&identifier_map);
	std::string beautify_class_name_of_autodesk_frame_generator(std::string&class_name);
	int is_class_name_of_autodesk_frame_generator(std::string&class_name);
	std::string append_string_with_number(std::string&identifier,int i);
	std::string generate_unique_identifier(zzxcf_map&identifier_map,std::string&identifier);
	void accumulate_map_identifier(zzxcf_section&section,zzxcf_map&identifier_map,zzxcf_map&inverted_identifier_map);
	void accumulate_map_directive(zzxcf_section&section,zzxcf_map&directive_map,type_directive_list&directive_list);
	void strip_specific_conditional_compilation(format_utility_namespace::zzxcf_phrase&phrase);
	int regroup(zzxcf_phrase&phrase,std::string&content,std::string interim_result_file_name="");
	bool does_string_contain_specific_token(const std::string&s,const zzxcf_token&token);
	size_t count_specific_token(const std::string&s,const zzxcf_token&token);
	class class_decorator
	{
	protected:
		std::shared_ptr<class_decorator>innerDecorator;
		zzxcf_phrase*phrase;
		std::string*fileName;
		std::string itermediateFileExtension;
	public:
		class_decorator(zzxcf_phrase&phrase,std::string&fileName,const std::string itermediateFileExtension,bool flagInsertSeparator);
		class_decorator();
		std::shared_ptr<class_decorator>get_inner_decorator();
		void set_inner_decorator(std::shared_ptr<class_decorator>d);
		virtual void process(zzxcf_phrase phrase);
		virtual void process();
		void save_itermediate();
		virtual~class_decorator();
	};
	class class_decorate_surround_if_clause_after_reserve_word_else_with_curly_bracket:
	public class_decorator
	{
	public:
		virtual void process(zzxcf_phrase&phrase);
	private:
		void concat_tokens_5fixing_elseif(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i);
		void fix_else_if(zzxcf_phrase&phrase);
	};
	class class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket:
	public class_decorator
	{
	public:
		virtual void process(zzxcf_phrase&phrase);
	protected:
		void concat_tokens_5fixing_ifelse(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i);
	private:
		void stack_push_top_pop(const format_utility_namespace::zzxcf_token&token,std::stack<size_t> &round_bracket_count_stack,size_t&round_bracket_count,std::stack<size_t> &curly_bracket_count_stack,size_t&curly_bracket_count);
		void concat_tokens_5fixing_for(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i);
		void fix_if_else(zzxcf_phrase&phrase);
	};
	class class_decorate_strip_duplicate_semicolon:
	public class_decorator
	{
	public:
		virtual void process(zzxcf_phrase&phrase);
	private:
		void remove_redundant_semicolon(zzxcf_phrase&phrase);
	};
	class class_decorate_strip_comments:
	public class_decorator
	{
	public:
		virtual void process(zzxcf_phrase&phrase);
		std::string get_comments();
	private:
		std::string comments;
		std::string remove_comments(zzxcf_phrase&phrase);
	};
	class class_decorate_strip_meaningless_comments:
	public class_decorator
	{
	public:
		virtual void process(zzxcf_phrase&phrase);
		std::string get_meaningless_comments();
	private:
		std::string empty_comments;
		std::string remove_empty_comments(zzxcf_phrase&phrase);
	};
	class class_decorate_remove_utf8_bom:
	public class_decorator
	{
	public:
		class_decorate_remove_utf8_bom(std::string interim_result_file_name);
		virtual void process(zzxcf_phrase&phrase);
	private:
		void remove_utf8_bom(zzxcf_phrase&phrase);
		std::string interim_result_file_name;
	};
	class class_decorate_strip_specific_conditional_compilation:
	public class_decorator
	{
	public:
		class_decorate_strip_specific_conditional_compilation(std::string interim_result_file_name);
		virtual void process(zzxcf_phrase&phrase);
	private:
		void strip_specific_conditional_compilation(zzxcf_phrase&phrase);
		std::string interim_result_file_name;
	};
	class class_decorate_strip_meaningless_pragmas:
	public class_decorator
	{
	public:
		virtual void process(zzxcf_phrase&phrase);
		std::string get_meaningless_pragmas();
	private:
		std::string pragmas;
		std::string remove_pragmas(zzxcf_phrase&phrase);
	};
	class class_decorate_surround_single_sentence_after_reserve_word_for_with_curly_bracket:
	public class_decorate_surround_single_sentence_after_reserve_words_if_and_else_with_curly_bracket
	{
	public:
		virtual void process(zzxcf_phrase&phrase);
	private:
		void stack_push_top_pop(const format_utility_namespace::zzxcf_token&token,std::stack<size_t> &round_bracket_count_stack,size_t&round_bracket_count,std::stack<size_t> &curly_bracket_count_stack,size_t&curly_bracket_count);
		void concat_tokens_5fixing_for(zzxcf_phrase&phrase,zzxcf_phrase::iterator&i);
		void fix_for(zzxcf_phrase&phrase);
	};
	class class_file_formatter
	{
	public:
		class_file_formatter(std::string file_path);
		class_file_formatter(const char**file_paths);
	private:
		ULONGLONG m_number_of_files,m_number_of_input_lines,m_number_of_input_tokens,m_number_of_output_tokens,m_number_of_output_lines;
		void format_files(const char**file_paths);
	};
	extern zzxcf_map comment_map;
	extern zzxcf_map identifier_map;
	extern zzxcf_map inverse_identifier_map;
	extern zzxcf_map directive_map;
	extern type_directive_list directive_list;
	extern type_directive_list failed_regrouping_file_paths;
	extern sqlite3*pFunctionsDetailsDatabase;
	extern sqlite3_stmt*pFunctionsDetailsStatementCreateTable;
	extern sqlite3_stmt*pFunctionsDetailsStatementInsertTable;
	extern const std::string sqlFunctionsDetailsCreateTable;
	extern const std::string sqlFunctionsDetailsInsertTable;
}
