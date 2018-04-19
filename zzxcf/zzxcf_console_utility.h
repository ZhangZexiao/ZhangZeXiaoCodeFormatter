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
#include "macro.h"
namespace console_utility_namespace
{
	void print_progress(size_t total_number_of_files,size_t processed_number_of_files);
	void print_zzxcf_logo();
	void print_summary();
	void print_zzxcf_usage(int argc,char**argv);
	void at_zzxcf_exit();
	class class_console_output_redirector
	{
	private:
		int m_console_output_fd= -1;
		std::string m_file_path;
		int m_old_fileno= -1,m_new_fileno= -1;
		FILE*m_FILE_ptr=0;
	private:
		class_console_output_redirector();
		class_console_output_redirector(const class_console_output_redirector&);
		class_console_output_redirector&operator=(const class_console_output_redirector&);
	public:
		class_console_output_redirector(int console_output_handle,std::string file_path)
		{
			m_console_output_fd=console_output_handle;
			m_file_path=file_path;
		}
		void turn_on()
		{
			turn_off();
			int hr= ::fopen_s(&m_FILE_ptr,m_file_path.c_str(),"w");
			ON_TRUE_RETURN(0!=hr,);
			m_old_fileno= ::_dup(m_console_output_fd);
			ON_TRUE_RETURN(-1==m_old_fileno,);
			m_new_fileno= ::_fileno(m_FILE_ptr);
			ON_TRUE_RETURN(-1==m_new_fileno,);
			hr= ::_dup2(m_new_fileno,m_console_output_fd);
			ON_TRUE_RETURN(-1==hr,);
			ON_TRUE_RETURN(0!=hr,);
		}
		void turn_off()
		{
			ON_TRUE_RETURN(0==m_FILE_ptr,);
			int hr= ::fflush(::__acrt_iob_func(m_console_output_fd));
			ON_TRUE_RETURN(0!=hr,);
			hr= ::_dup2(m_old_fileno,m_console_output_fd);
			ON_TRUE_RETURN(-1==hr,);
			ON_TRUE_RETURN(0!=hr,);
			hr= ::fclose(m_FILE_ptr);
			ON_TRUE_RETURN(0!=hr,);
			m_FILE_ptr=0;
		}
		~class_console_output_redirector()
		{
			turn_off();
		}
	};
}
