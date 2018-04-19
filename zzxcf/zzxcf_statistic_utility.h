#pragma once
#include "stdafx.h"
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
namespace statistic_utility_namespace
{
	std::string get_system_time_std_string(const SYSTEMTIME&system_time);
	std::string get_file_time_std_string(const FILETIME&file_time);
	std::string get_local_time_std_string(const FILETIME&file_time);
	std::string get_io_counters_std_string(const IO_COUNTERS&io_counters);
	void print_thread_statistic();
	void print_statistical_data();
	void beep();
	void notify_user();
	extern LARGE_INTEGER program_time_frequency;
	extern LARGE_INTEGER program_start_time;
	extern LARGE_INTEGER program_stop_time;
	extern ULONGLONG total_number_of_files;
	extern ULONGLONG total_number_of_input_lines;
	extern ULONGLONG total_number_of_output_lines;
	extern ULONGLONG total_number_of_input_tokens;
	extern ULONGLONG total_number_of_output_tokens;
}
