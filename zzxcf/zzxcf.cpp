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
#pragma comment(lib,"sqlite3.lib")
#include "macro.h"
#include "zzxcf_command_line_option.h"
#include "zzxcf_performance_monitor.h"
#include "zzxcf_file_operation.h"
#include "zzxcf_multithread_utility.h"
#include "zzxcf_console_utility.h"
#include "zzxcf_statistic_utility.h"
#include "zzxcf_format_utility.h"
#include "../sqlite/sqlite3.h"
int main(int argc,char**argv)
{
	int in_out_argv_position=1;
	command_line_namespace::command_line_option_parser::parse(argc,argv,in_out_argv_position);
	std::list<std::string>out_to_be_formatted_files;
	file_operation_namespace::search_files(argc,argv,in_out_argv_position,out_to_be_formatted_files);
	if(command_line_namespace::option_restore_previous_version.is_turned_on())
	{
		format_utility_namespace::restore_previous_version(out_to_be_formatted_files);
		return 0;
	}
	if(command_line_namespace::option_cleanup_orig_files.is_turned_on())
	{
		format_utility_namespace::delete_all_orig_files(argc,argv,in_out_argv_position);
	}
	::atexit(console_utility_namespace::at_zzxcf_exit);
	std::string new_stdout_file=file_operation_namespace::generate_unique_file_name(argv[0],".stdout");
	std::string new_stderr_file=file_operation_namespace::generate_unique_file_name(argv[0],".stderr");
	console_utility_namespace::class_console_output_redirector redirect_stdout(1,new_stdout_file),redirect_stderr(2,new_stderr_file);
	if(command_line_namespace::option_redirect_stdout_to_file.is_turned_on())
	{
		redirect_stdout.turn_on();
	}
	if(!command_line_namespace::option_do_not_redirect_stderr_to_file.is_turned_on())
	{
		redirect_stderr.turn_on();
	}
	QueryPerformanceCounter(&statistic_utility_namespace::program_start_time);
	QueryPerformanceFrequency(&statistic_utility_namespace::program_time_frequency);
	if(command_line_namespace::option_print_help.is_turned_on())
	{
		console_utility_namespace::print_zzxcf_usage(argc,argv);
		return 0;
	}
	if(!command_line_namespace::option_do_not_print_logo.is_turned_on())
	{
		console_utility_namespace::print_zzxcf_logo();
	}
	if(command_line_namespace::option_convert_to_bridge_pattern.is_turned_on())
	{
		format_utility_namespace::convert_to_bridge_pattern(out_to_be_formatted_files);
		return 0;
	}
	if(command_line_namespace::option_save_functions_into_sqlite3_database.is_turned_on()&&command_line_namespace::option_do_not_touch_files.is_turned_on())
	{
		format_utility_namespace::save_functions_into_sqlite3_database(out_to_be_formatted_files);
		return 0;
	}
	if(command_line_namespace::option_import_maps.is_turned_on())
	{
		import_comment_map(format_utility_namespace::comment_map);
		import_directive_map(format_utility_namespace::directive_map);
		import_id_map(format_utility_namespace::identifier_map,format_utility_namespace::inverse_identifier_map);
	}
	if(command_line_namespace::option_do_not_use_multithreads.is_turned_on())
	{
		format_utility_namespace::format_files_in_process(out_to_be_formatted_files);
	}
	else
	{
		InitializeCriticalSection(&namespace_multithread::critical_section_of_statistic);
		InitializeCriticalSection(&namespace_multithread::critical_section_of_map_operation);
		format_utility_namespace::format_files_in_threads(out_to_be_formatted_files);
	}
	QueryPerformanceCounter(&statistic_utility_namespace::program_stop_time);
	if(!command_line_namespace::option_do_not_print_statistical_data.is_turned_on())
	{
		statistic_utility_namespace::print_statistical_data();
	}
	if(command_line_namespace::option_notify_user_when_job_done.is_turned_on())
	{
		statistic_utility_namespace::notify_user();
	}
	if(command_line_namespace::option_export_maps.is_turned_on())
	{
		export_comment_map(format_utility_namespace::comment_map);
		export_id_map(format_utility_namespace::identifier_map);
		export_directive_map(format_utility_namespace::directive_map,format_utility_namespace::directive_list);
	}
	return 0;
}
