#include "zzxcf_console_utility.h"
#include "zzxcf_performance_monitor.h"
#include "zzxcf_file_operation.h"
#include "zzxcf_command_line_option.h"
#include "..\common\Macro.h"
namespace console_utility_namespace
{
	void print_progress(size_t total_number_of_files,size_t processed_number_of_files)
	{
		static namespace_performance::class_performance_monitor cpu_performance_monitor;
		static namespace_performance::class_performance_monitor disk_performance_monitor;
		static LONGLONG total_cpu=0;
		static LONGLONG cpu_perf_count=0;
		static LONGLONG total_disk=0;
		static LONGLONG disk_perf_count=0;
		total_cpu+=cpu_performance_monitor.get_cpu_busy_percentage();
		cpu_perf_count++;
		if(cpu_perf_count%30==0)
		{
			total_cpu/=cpu_perf_count;
			cpu_perf_count=1;
		}
		total_disk+=(100-disk_performance_monitor.get_disk_idle_percentage());
		disk_perf_count++;
		if(disk_perf_count%30==0)
		{
			total_disk/=disk_perf_count;
			disk_perf_count=1;
		}
		static DWORD tick_program_started= ::GetTickCount();
		const size_t buffer_count=1024;
		static char buffer[buffer_count];
		::sprintf_s(buffer,buffer_count,"Progress %02d%%(%zd/%zd)|Elapsed Time %ld(ms)|CPU Usage %02I64d%%|Disk Usage %02I64d%%|ZZXCF by Zhang Ze Xiao",(int)((float)processed_number_of_files/(float)total_number_of_files*100),processed_number_of_files,total_number_of_files,GetTickCount()-tick_program_started,total_cpu/cpu_perf_count,total_disk/disk_perf_count);
		::SetConsoleTitleA(buffer);
		HWND hwnd= ::GetDesktopWindow();
		if(hwnd)
		{
			HDC hdc= ::GetWindowDC(hwnd);
			if(hdc)
			{
				static RECT rect=
				{
					0
				};
				static SIZE text_size=
				{
					0
				};
				static bool is_initialized=false;
				if(!is_initialized)
				{
					is_initialized=true;
					::GetWindowRect(hwnd,&rect);
					::GetTextExtentPoint32A(hdc,buffer,(int)strlen(buffer),&text_size);
				}
				::TextOutA(hdc,(rect.right-text_size.cx)/2,rect.top,buffer,(int)strlen(buffer));
				::UpdateWindow(hwnd);
			}
		}
	}
	void print_zzxcf_logo()
	{
		SetConsoleTitle(_T("ZZXCF by Zhang Ze Xiao"));
		HANDLE std_output_handle=GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO default_console_screen_buffer_info;
		GetConsoleScreenBufferInfo(std_output_handle,&default_console_screen_buffer_info);
		SetConsoleTextAttribute(std_output_handle,FOREGROUND_RED|FOREGROUND_INTENSITY);
		printf("C/C++/C#/Java Source Code Formatter/Compressor/Beautifier/Optimizer/Decorator\n");
		SetConsoleTextAttribute(std_output_handle,FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		printf("Copyright (C) 2014-2015-2016-2017 Zhang Ze Xiao. All Rights Reserved.\n");
		SetConsoleTextAttribute(std_output_handle,FOREGROUND_BLUE|FOREGROUND_INTENSITY);
		printf("Written by Zhang Ze Xiao @ Shanghai.\n");
		SetConsoleTextAttribute(std_output_handle,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		printf("Report bugs to zhangzexiaozhang@163.com\n");
		printf("Send request to zhangzexiaozhang@163.com to get latest version.\n");
		SetConsoleTextAttribute(std_output_handle,FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
		printf("Compiled at %s %s using ",__DATE__,__TIME__);
#ifdef _DEBUG
		printf("DEBUG configuration.\n\n");
#else
		printf("RELEASE configuration.\n\n");
#endif
		SetConsoleTextAttribute(std_output_handle,default_console_screen_buffer_info.wAttributes);
	}
	void print_summary()
	{
		static char*summary[]=
		{
			"\nZZXCF is a tool target to C, C++, C#, Java source code. It works on source files with extensions such as *.c, *.cc, *.cpp, *.cs, *.java, *.h, *.hpp. Its competitors: clang-format.exe, AStyle.exe, ...\n",
			"Compare with ZZXCF's competitors, what advantages does ZZXCF have?\n",
			"1.Fast, ZZXCF utilizes multi-threading to format source code. How many working threads at run time? The number equals to the number of CPU cores of your computer. Other threads including main thread, and status thread which shows formatting progress, CPU usage, disk usage.\n",
			"2.Small, less than 1MB.\n",
			"3.Target to C family languages, including C, C++, C#, Java, etc.\n",
			"4.Remove comments option. Bad comments are useless, you can remove all comments with -rc option.\n",
			"5.Supports wild card [*?] in file name and subdirectories searching with -ss option.\n",
			"6.Surrounds single sentence after \"if\", \"else\", \"for\" with curly brackets {}.\n",
			"7.Remove useless semicolon [;], in case \"{;...}\", \";;\".\n",
			"8.Production-quality. Passed more than 2,000,000 lines of business source code validation, including formatting, compilation, automation testing and no error.\n",
			"9.Progress showing and CPU usage, disk usage updating.\n",
			"10.Extracts functions in source code and save into sqlite3 database.\n",
			"11.Unify head file path separator from slash to backslash.\n",
			"\n\nZZXCF's implementation language is C++, and has about 150K C++ source code in zzxcf.cpp file.\n",
			"Why all code in one source file?\n",
			"It's convenient to send a file through email, where there is a network connection, there is a way to review zzxcf.cpp code, especially on smart phone.\n",
			"It's convenient to compile a file through command line. If you put commands into a batch script file, you can do lots of things with only one mouse clicking.\n",
			"\nCore feature of ZZXCF includes:\n",
			"Lexer which is a state machine used to generate phrase from source file content.\n",
			"Decorators which are used to add more useful phrase such as curly brackets.\n",
			"Removers which are used to delete useless phrase such as comments, white spaces, preprocessor directives.\n",
			"Regroupers which are used to regroup phrase to a coding style you want/like.\n",
			"Each specific decorator, remover, regrouper can be activated or deactivated by command line option.\n",
			"In future version of ZZXCF, I want these decorators, removers, regroupers taken into action in a sequence which specified by command line options, so you can control/custom these doer's running sequence, expecting the result you like. For example, you only want to remove comments, then you can activate comments remover and deactivate other doers.\n",
			"\nOther features of ZZXCF includes:\n",
			"Command line parser, file operation utility, multi-threading utility, hardware performance real time detection utility, character and std::string utility, sqlite3 database operation utility, ...\n",
			"\nIt's easy to extend the ability of ZZXCF, for example, add more decorators, more removers, and more regroupers.\n"
		};
		HANDLE std_output_handle=GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO default_console_screen_buffer_info;
		GetConsoleScreenBufferInfo(std_output_handle,&default_console_screen_buffer_info);
		SetConsoleTextAttribute(std_output_handle,FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
		for(size_t i=0;i<sizeof(summary)/sizeof(summary[0]);i++)
		{
			printf("%s",summary[i]);
		}
		SetConsoleTextAttribute(std_output_handle,default_console_screen_buffer_info.wAttributes);
		printf("\n\n");
	}
	void print_zzxcf_usage(int argc,char**argv)
	{
		assert(argc>0);
		print_zzxcf_logo();
		print_summary();
		std::string name=file_operation_namespace::extract_file_name_from_file_path(argv[0]);
		printf("Usage#1: %s %s fileName fileName ...\n\n",name.c_str(),command_line_namespace::command_line_option_parser::get_descriptive_names_in_bracket().c_str());
		printf("Usage#2: %s %s fileName fileName ...\n\n",name.c_str(),command_line_namespace::command_line_option_parser::get_simplified_names_in_bracket().c_str());
		printf("Example#1: %s %s *.cpp *.c *.h *.cs\n\n",name.c_str(),command_line_namespace::command_line_option_parser::get_descriptive_names().c_str());
		printf("Example#2: %s %s *.cpp *.c *.h *.cs\n\n",name.c_str(),command_line_namespace::command_line_option_parser::get_simplified_names().c_str());
		printf("%s",command_line_namespace::command_line_option_parser::get_usages().c_str());
		printf("fileName\n\tFile name supports wild-card '*' or '?'.\n\n");
		_getch();
	}
	void at_zzxcf_exit()
	{
		if(command_line_namespace::option_notify_user_when_job_done.is_turned_on())
		{
			MessageBox(GetConsoleWindow(),_T("JOB DONE!\n\n\n\tby Zhang Ze Xiao @ Shanghai"),_T("ZhangZeXiaoZhang@163.com"),MB_OK);
		}
	}
	class_console_output_redirector::class_console_output_redirector()
	{
	}
	class_console_output_redirector::class_console_output_redirector(const class_console_output_redirector&)
	{
	}
	class_console_output_redirector&class_console_output_redirector::operator=(const class_console_output_redirector&)
	{
		return*this;
	}
}
