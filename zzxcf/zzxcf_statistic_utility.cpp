#include "zzxcf_statistic_utility.h"
namespace statistic_utility_namespace
{
	extern LARGE_INTEGER program_time_frequency=
	{
	};
	extern LARGE_INTEGER program_start_time=
	{
	};
	extern LARGE_INTEGER program_stop_time=
	{
	};
	extern ULONGLONG total_number_of_files=0;
	extern ULONGLONG total_number_of_input_lines=0;
	extern ULONGLONG total_number_of_output_lines=0;
	extern ULONGLONG total_number_of_input_tokens=0;
	extern ULONGLONG total_number_of_output_tokens=0;
	std::string get_system_time_std_string(const SYSTEMTIME&system_time)
	{
		char buffer[1024];
		::sprintf_s(buffer,"Year: %u, Month: %u, DayOfWeek: %u, Day: %u, Hour: %u, Minute: %u, Second: %u, Milliseconds: %u",system_time.wYear,system_time.wMonth,system_time.wDayOfWeek,system_time.wDay,system_time.wHour,system_time.wMinute,system_time.wSecond,system_time.wMilliseconds);
		::sprintf_s(buffer,"%04u-%02u-(%u)%02u %02u:%02u:%02u.%03u",system_time.wYear==1601?system_time.wYear-1601:system_time.wYear,system_time.wYear==1601?system_time.wMonth-1:system_time.wMonth,system_time.wYear==1601?system_time.wDayOfWeek-1:system_time.wDayOfWeek,system_time.wYear==1601?system_time.wDay-1:system_time.wDay,system_time.wYear==1601?system_time.wHour-8:system_time.wHour,system_time.wMinute,system_time.wSecond,system_time.wMilliseconds);
		return buffer;
	}
	std::string get_file_time_std_string(const FILETIME&file_time)
	{
		SYSTEMTIME system_time;
		::memset(&system_time,0,sizeof(SYSTEMTIME));
		::FileTimeToSystemTime(&file_time,&system_time);
		return get_system_time_std_string(system_time);
	}
	std::string get_local_time_std_string(const FILETIME&file_time)
	{
		SYSTEMTIME system_time,local_time;
		::memset(&system_time,0,sizeof(SYSTEMTIME));
		::memset(&local_time,0,sizeof(SYSTEMTIME));
		::FileTimeToSystemTime(&file_time,&system_time);
		TIME_ZONE_INFORMATION time_zone_information;
		::memset(&time_zone_information,0,sizeof(TIME_ZONE_INFORMATION));
		::GetTimeZoneInformation(&time_zone_information);
		::SystemTimeToTzSpecificLocalTime(&time_zone_information,&system_time,&local_time);
		return get_system_time_std_string(local_time);
	}
	std::string get_io_counters_std_string(const IO_COUNTERS&io_counters)
	{
		char buffer[1024];
		::sprintf_s(buffer,"\nNumber of read operations(%I64u), number of write operations(%I64u), number of other operations(%I64u),\nNumber of read transfer(%I64u), number of write transfer(%I64u), number of other transfer(%I64u)",io_counters.ReadOperationCount,io_counters.WriteOperationCount,io_counters.OtherOperationCount,io_counters.ReadTransferCount,io_counters.WriteTransferCount,io_counters.OtherTransferCount);
		return std::string(buffer);
	}
	void print_thread_statistic()
	{
		FILETIME creationTime,exitTime,kernelTime,userTime;
		memset(&creationTime,0,sizeof(FILETIME));
		memset(&exitTime,0,sizeof(FILETIME));
		memset(&kernelTime,0,sizeof(FILETIME));
		memset(&userTime,0,sizeof(FILETIME));
		GetThreadTimes(GetCurrentThread(),&creationTime,&exitTime,&kernelTime,&userTime);
		fprintf(stdout,"\nThread(%lu) Creation Time(%s)\n",GetCurrentThreadId(),get_local_time_std_string(creationTime).c_str());
		fprintf(stdout,"Thread(%lu) Kernel Time(%s)\n",GetCurrentThreadId(),get_local_time_std_string(kernelTime).c_str());
		fprintf(stdout,"Thread(%lu) User Time(%s)\n",GetCurrentThreadId(),get_local_time_std_string(userTime).c_str());
	}
	void print_statistical_data()
	{
		FILETIME creationTime,exitTime,kernelTime,userTime;
		memset(&creationTime,0,sizeof(FILETIME));
		memset(&exitTime,0,sizeof(FILETIME));
		memset(&kernelTime,0,sizeof(FILETIME));
		memset(&userTime,0,sizeof(FILETIME));
		GetProcessTimes(GetCurrentProcess(),&creationTime,&exitTime,&kernelTime,&userTime);
		fprintf(stdout,"\nProcess(%lu) Creation Time(%s)\n",GetCurrentProcessId(),get_local_time_std_string(creationTime).c_str());
		fprintf(stdout,"Process(%lu) Kernel Time(%s)\n",GetCurrentProcessId(),get_local_time_std_string(kernelTime).c_str());
		fprintf(stdout,"Process(%lu) User Time(%s)\n",GetCurrentProcessId(),get_local_time_std_string(userTime).c_str());
		IO_COUNTERS ioCounters;
		GetProcessIoCounters(GetCurrentProcess(),&ioCounters);
		fprintf(stderr,"%s\n",get_io_counters_std_string(ioCounters).c_str());
		printf("\nTotal number of files(%I64u), total number of input lines(%I64u), total number of input tokens(%I64u), total number of output lines(%I64u), total number of output tokens(%I64u)\n",total_number_of_files,total_number_of_input_lines,total_number_of_input_tokens,total_number_of_output_lines,total_number_of_output_tokens);
		printf("Total time consumed: (%I64d-%I64d)/(%I64d/1000)=%I64d(ms)\n\n",program_stop_time.QuadPart,program_start_time.QuadPart,program_time_frequency.QuadPart,(program_stop_time.QuadPart-program_start_time.QuadPart)/(program_time_frequency.QuadPart/1000));
	}
	void beep()
	{
		Beep(2000,500);
	}
	void notify_user()
	{
		FlashWindow(GetForegroundWindow(),true);
		SetForegroundWindow(GetConsoleWindow());
		SetActiveWindow(GetConsoleWindow());
		SetFocus(GetConsoleWindow());
	}
}
