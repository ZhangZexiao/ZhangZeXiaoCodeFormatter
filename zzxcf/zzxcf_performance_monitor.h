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
namespace namespace_performance
{
	template<class T>
	class class_performance_counter
	{
		const unsigned long ORIGINAL_NUMBER_OF_BYTES_FOR_PERFORMANCE_DATA=100*1024;
		const unsigned long INCREMENTAL_NUMBER_OF_BYTES_FOR_PERFORMANCE_DATA=10*1024;
	public:
		class_performance_counter();
		~class_performance_counter();
		T get_performance_counter_value(PERF_DATA_BLOCK**pointer_to_pointer_of_performance_data_block,DWORD ObjectNameTitleIndex,DWORD counter_index,LPCTSTR pointer_to_performance_instance_name=NULL);
	protected:
		class class_performance_data_buffer
		{
		public:
			class_performance_data_buffer(UINT size);
			~class_performance_data_buffer();
			void*realloc_buffer(UINT size);
			void zero_buffer();
			operator LPBYTE();
			UINT get_buffer_size();
		public:
			LPBYTE m_buffer;
		private:
			UINT m_size;
		};
		void query_performance_data(PERF_DATA_BLOCK**pointer_to_pointer_of_performance_data_block,DWORD object_index);
		T get_performance_counter_value(PPERF_OBJECT_TYPE pointer_to_performance_object,DWORD counter_name_title_index,LPCTSTR pointer_to_performance_instance_name);
		PPERF_OBJECT_TYPE get_pointer_to_first_performance_object(PPERF_DATA_BLOCK p_perf_data_block);
		PPERF_OBJECT_TYPE get_pointer_to_next_performance_object(PPERF_OBJECT_TYPE p_perf_object);
		PPERF_COUNTER_DEFINITION get_pointer_to_first_performance_counter(PPERF_OBJECT_TYPE pointer_to_performance_object);
		PPERF_COUNTER_DEFINITION get_pointer_to_next_performance_counter(PPERF_COUNTER_DEFINITION pointer_to_performance_object);
		PPERF_INSTANCE_DEFINITION get_pointer_to_first_performance_instance(PPERF_OBJECT_TYPE pointer_to_performance_object);
		PPERF_INSTANCE_DEFINITION get_pointer_to_next_performance_instance(PPERF_INSTANCE_DEFINITION pointer_to_performance_instance);
	};
	class class_performance_monitor
	{
		const unsigned long SYSTEM_OBJECT_INDEX=2;
		const unsigned long PROCESS_OBJECT_INDEX=230;
		const unsigned long PROCESSOR_OBJECT_INDEX=238;
		const unsigned long TOTAL_PROCESSOR_TIME_COUNTER_INDEX=240;
		const unsigned long PROCESSOR_TIME_COUNTER_INDEX=6;
		const unsigned long PHYSICALDISK_OBJECT_INDEX=234;
		const unsigned long DISK_TIME_COUNTER_INDEX=200;
		const unsigned long DISK_IDLE_TIME_COUNTER_INDEX=1482;
	public:
		class_performance_monitor();
		~class_performance_monitor();
		void enable_performance_counters();
		int get_disk_idle_percentage();
		int get_cpu_busy_percentage();
	private:
		bool m_first_time;
		LONGLONG m_old_counter_value;
		LARGE_INTEGER m_old_performance_counter_time;
	};
}
