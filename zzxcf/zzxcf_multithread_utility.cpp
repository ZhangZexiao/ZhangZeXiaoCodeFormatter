#include "zzxcf_multithread_utility.h"
#include "zzxcf_command_line_option.h"
namespace namespace_multithread
{
	extern CRITICAL_SECTION critical_section_of_statistic=
	{
	};
	extern CRITICAL_SECTION critical_section_of_map_operation=
	{
	};
	void printf_file_paths(const char*const*const file_paths,size_t file_paths_length)
	{
		for(size_t i=0;i<file_paths_length;i++)
		{
			if(file_paths[i])
			{
				printf("!%s\n",file_paths[i]);
			}
		}
	}
	DWORD get_number_of_processors()
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		return si.dwNumberOfProcessors;
	}
	DWORD get_number_of_threads()
	{
		if(command_line_namespace::option_number_of_threads.is_turned_on())
		{
			return command_line_namespace::zzxcf_command_line_option_number_of_threads::number_of_threads;
		}
		DWORD number_of_threads=get_number_of_processors();
		number_of_threads*=3;
		number_of_threads=number_of_threads>64?64:number_of_threads;
		return number_of_threads;
	}
	DWORD get_number_of_files_per_thread(size_t number_of_files,size_t number_of_threads)
	{
		DWORD number_of_files_per_thread=(DWORD)(number_of_files/number_of_threads);
		number_of_files_per_thread=number_of_files_per_thread<1?1:number_of_files_per_thread;
		return number_of_files_per_thread;
	}
	void initialize_file_paths(const char**const file_paths,const size_t file_paths_length,const std::list<std::string>&file_path_list)
	{
		for(size_t i=0;i<=file_paths_length;i++)
		{
			file_paths[i]=0;
		}
		std::list<std::string>::const_iterator it=file_path_list.begin();
		for(size_t i=0;i<file_paths_length,it!=file_path_list.end();i++,it++)
		{
			file_paths[i]=it->c_str();
		}
	}
}
