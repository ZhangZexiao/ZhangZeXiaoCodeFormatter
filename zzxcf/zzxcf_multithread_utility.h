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
namespace namespace_multithread
{
	void printf_file_paths(const char*const*const file_paths,size_t file_paths_length);
	DWORD get_number_of_processors();
	DWORD get_number_of_threads();
	DWORD get_number_of_files_per_thread(size_t number_of_files,size_t number_of_threads);
	void initialize_file_paths(const char**const file_paths,const size_t file_paths_length,const std::list<std::string>&file_path_list);
	extern CRITICAL_SECTION critical_section_of_statistic;
	extern CRITICAL_SECTION critical_section_of_map_operation;
}
