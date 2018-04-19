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
namespace file_operation_namespace
{
	std::string extract_file_name_from_file_path(const std::string&file_path);
	bool file_exists(const std::string&file_path);
	bool remove_file(const std::string&file_path);
	bool rename_file(const std::string&old_file_path,const std::string&new_file_path);
	bool is_readonly(const std::string&file_path);
	bool clear_readonly(const std::string&file_path);
	size_t get_file_size(const std::string&file_path);
	bool open_file(const std::string&file_path,const std::string&mode,FILE**pp_file);
	bool close_file(FILE*p_file);
	bool read_file(const std::string&file_path,std::string&content);
	bool write_file(const std::string&file_path,const std::string&content);
	std::string generate_unique_file_name(std::string file_name,const std::string&file_extension);
	std::string get_latest_orig_file(std::string file_name);
	void search_files_in_directory(std::string file_matching_pattern,std::list<std::string>&files);
	void search_files_in_directory_subdirectory(std::string file_matching_pattern,std::list<std::string>&files);
	void search_files(int argc,char**argv,int index,std::list<std::string>&files);
	std::string get_file_extension(const std::string&file_name);
	std::string strip_file_extension(const std::string&file_name);
	std::string replace_file_extension(const std::string&file_name,const std::string&new_file_extension);
	std::string get_orig_file_matching_pattern(const std::string&file_name);
	void search_command_line_orig_files(int argc,char**argv,int index,std::list<std::string>&files);
	size_t find_utf8_bom(const std::string&file_content);
	bool is_utf8_encoded_file(const std::string&file_path);
	bool is_utf8_encoded_file_content(const std::string&file_content);
	size_t count_number_of_line_feeds(const std::string&source);
}
