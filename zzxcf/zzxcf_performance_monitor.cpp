#include "zzxcf_performance_monitor.h"
namespace namespace_performance
{
	template<class T>
	class_performance_counter<T>::class_performance_counter()
	{
	}
	template<class T>
	class_performance_counter<T>::~class_performance_counter()
	{
	}
	template<class T>
	T class_performance_counter<T>::get_performance_counter_value(PERF_DATA_BLOCK**pointer_to_pointer_of_performance_data_block,DWORD ObjectNameTitleIndex,DWORD counter_index,LPCTSTR pointer_to_performance_instance_name)
	{
		query_performance_data(pointer_to_pointer_of_performance_data_block,ObjectNameTitleIndex);
		PPERF_OBJECT_TYPE pointer_to_performance_object=NULL;
		T value=
		{
			0
		};
		if(*pointer_to_pointer_of_performance_data_block)
		{
			pointer_to_performance_object=get_pointer_to_first_performance_object(*pointer_to_pointer_of_performance_data_block);
			for(DWORD i=0;i<(*pointer_to_pointer_of_performance_data_block)->NumObjectTypes;i++)
			{
				if(pointer_to_performance_object->ObjectNameTitleIndex==ObjectNameTitleIndex)
				{
					value=get_performance_counter_value(pointer_to_performance_object,counter_index,pointer_to_performance_instance_name);
					break;
				}
				pointer_to_performance_object=get_pointer_to_next_performance_object(pointer_to_performance_object);
			}
		}
		return value;
	}
	template<class T>
	void class_performance_counter<T>::query_performance_data(PERF_DATA_BLOCK**pointer_to_pointer_of_performance_data_block,DWORD object_index)
	{
		static class_performance_data_buffer buffer(ORIGINAL_NUMBER_OF_BYTES_FOR_PERFORMANCE_DATA);
		buffer.zero_buffer();
		TCHAR value_name[32];
		_stprintf_s(value_name,_T("%ld"),object_index);
		DWORD buffer_size=buffer.get_buffer_size();
		while(RegQueryValueEx(HKEY_PERFORMANCE_DATA,value_name,NULL,NULL,buffer,&buffer_size)==ERROR_MORE_DATA)
		{
			buffer.realloc_buffer(INCREMENTAL_NUMBER_OF_BYTES_FOR_PERFORMANCE_DATA+buffer.get_buffer_size());
		}
		*pointer_to_pointer_of_performance_data_block=(PERF_DATA_BLOCK*)buffer.m_buffer;
	}
	template<class T>
	T class_performance_counter<T>::get_performance_counter_value(PPERF_OBJECT_TYPE pointer_to_performance_object,DWORD counter_name_title_index,LPCTSTR pointer_to_performance_instance_name)
	{
		PPERF_COUNTER_DEFINITION pointer_to_performance_counter=NULL;
		PPERF_INSTANCE_DEFINITION pointer_to_performance_instance=NULL;
		PPERF_COUNTER_BLOCK pointer_to_performance_counter_block=NULL;
		pointer_to_performance_counter=get_pointer_to_first_performance_counter(pointer_to_performance_object);
		for(DWORD i=0;i<pointer_to_performance_object->NumCounters;i++)
		{
			if(pointer_to_performance_counter->CounterNameTitleIndex==counter_name_title_index)
			{
				break;
			}
			pointer_to_performance_counter=get_pointer_to_next_performance_counter(pointer_to_performance_counter);
		}
		if(pointer_to_performance_object->NumInstances==PERF_NO_INSTANCES)
		{
			pointer_to_performance_counter_block=(PPERF_COUNTER_BLOCK)((LPBYTE)pointer_to_performance_object+pointer_to_performance_object->DefinitionLength);
		}
		else
		{
			pointer_to_performance_instance=get_pointer_to_first_performance_instance(pointer_to_performance_object);
			_bstr_t current_performance_instance_name;
			_bstr_t performance_instance_name=pointer_to_performance_instance_name;
			for(int i=0;i<pointer_to_performance_object->NumInstances;i++)
			{
				current_performance_instance_name=(wchar_t*)((PBYTE)pointer_to_performance_instance+pointer_to_performance_instance->NameOffset);
				if(!_tcsicmp((LPCTSTR)current_performance_instance_name,(LPCTSTR)performance_instance_name))
				{
					pointer_to_performance_counter_block=(PPERF_COUNTER_BLOCK)((LPBYTE)pointer_to_performance_instance+pointer_to_performance_instance->ByteLength);
					break;
				}
				pointer_to_performance_instance=get_pointer_to_next_performance_instance(pointer_to_performance_instance);
			}
		}
		if(pointer_to_performance_counter_block)
		{
			T*counter_value=NULL;
			counter_value=(T*)((LPBYTE)pointer_to_performance_counter_block+pointer_to_performance_counter->CounterOffset);
			return*counter_value;
		}
		return-1;
	}
	template<class T>
	PPERF_OBJECT_TYPE class_performance_counter<T>::get_pointer_to_first_performance_object(PPERF_DATA_BLOCK p_perf_data_block)
	{
		return((PPERF_OBJECT_TYPE)((PBYTE)p_perf_data_block+p_perf_data_block->HeaderLength));
	}
	template<class T>
	PPERF_OBJECT_TYPE class_performance_counter<T>::get_pointer_to_next_performance_object(PPERF_OBJECT_TYPE p_perf_object)
	{
		return((PPERF_OBJECT_TYPE)((PBYTE)p_perf_object+p_perf_object->TotalByteLength));
	}
	template<class T>
	PPERF_COUNTER_DEFINITION class_performance_counter<T>::get_pointer_to_first_performance_counter(PPERF_OBJECT_TYPE pointer_to_performance_object)
	{
		return((PPERF_COUNTER_DEFINITION)((PBYTE)pointer_to_performance_object+pointer_to_performance_object->HeaderLength));
	}
	template<class T>
	PPERF_COUNTER_DEFINITION class_performance_counter<T>::get_pointer_to_next_performance_counter(PPERF_COUNTER_DEFINITION pointer_to_performance_object)
	{
		return((PPERF_COUNTER_DEFINITION)((PBYTE)pointer_to_performance_object+pointer_to_performance_object->ByteLength));
	}
	template<class T>
	PPERF_INSTANCE_DEFINITION class_performance_counter<T>::get_pointer_to_first_performance_instance(PPERF_OBJECT_TYPE pointer_to_performance_object)
	{
		return((PPERF_INSTANCE_DEFINITION)((PBYTE)pointer_to_performance_object+pointer_to_performance_object->DefinitionLength));
	}
	template<class T>
	PPERF_INSTANCE_DEFINITION class_performance_counter<T>::get_pointer_to_next_performance_instance(PPERF_INSTANCE_DEFINITION pointer_to_performance_instance)
	{
		PPERF_COUNTER_BLOCK pointer_to_performance_counter_block;
		pointer_to_performance_counter_block=(PPERF_COUNTER_BLOCK)((PBYTE)pointer_to_performance_instance+pointer_to_performance_instance->ByteLength);
		return((PPERF_INSTANCE_DEFINITION)((PBYTE)pointer_to_performance_counter_block+pointer_to_performance_counter_block->ByteLength));
	}
	template<class T>
	class_performance_counter<T>::class_performance_data_buffer::class_performance_data_buffer(UINT size)
	{
		m_size=size;
		m_buffer=(LPBYTE)malloc(m_size*sizeof(BYTE));
	}
	template<class T>
	class_performance_counter<T>::class_performance_data_buffer::~class_performance_data_buffer()
	{
		free(m_buffer);
	}
	template<class T>
	void*class_performance_counter<T>::class_performance_data_buffer::realloc_buffer(UINT size)
	{
		m_size=size;
		m_buffer=(LPBYTE)realloc(m_buffer,m_size);
		return m_buffer;
	}
	template<class T>
	void class_performance_counter<T>::class_performance_data_buffer::zero_buffer()
	{
		memset(m_buffer,NULL,m_size);
	}
	template<class T>
	class_performance_counter<T>::class_performance_data_buffer::operator LPBYTE()
	{
		return m_buffer;
	}
	template<class T>
	UINT class_performance_counter<T>::class_performance_data_buffer::get_buffer_size()
	{
		return m_size;
	}
	class_performance_monitor::class_performance_monitor()
	{
		m_first_time=true;
		m_old_counter_value=0;
		memset(&m_old_performance_counter_time,0,sizeof(m_old_performance_counter_time));
	}
	class_performance_monitor::~class_performance_monitor()
	{
	}
	void class_performance_monitor::enable_performance_counters()
	{
		CRegKey reg_key;
		if(reg_key.Open(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Services\\PerfOS\\Performance"))==ERROR_SUCCESS)
		{
			reg_key.SetDWORDValue(_T("Disable Performance Counters"),0);
			reg_key.Close();
		}
		if(reg_key.Open(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Services\\PerfProc\\Performance"))==ERROR_SUCCESS)
		{
			reg_key.SetDWORDValue(_T("Disable Performance Counters"),0);
			reg_key.Close();
		}
		if(reg_key.Open(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Services\\PerfDisk\\Performance"))==ERROR_SUCCESS)
		{
			reg_key.SetDWORDValue(_T("Disable Performance Counters"),0);
			reg_key.Close();
		}
	}
	int class_performance_monitor::get_disk_idle_percentage()
	{
		if(m_first_time)
		{
			enable_performance_counters();
		}
		class_performance_counter<LONGLONG>performance_counter;
		TCHAR sz_instance[256]=
		{
			0
		};
		DWORD object_index=PHYSICALDISK_OBJECT_INDEX;
		DWORD counter_index=DISK_IDLE_TIME_COUNTER_INDEX;
		_tcscpy_s(sz_instance,_T("_Total"));
		int disk_idle_percentage=0;
		LONGLONG counter_value=0;
		PPERF_DATA_BLOCK pointer_to_performance_data_block=NULL;
		LARGE_INTEGER performance_counter_time=
		{
			0
		};
		counter_value=performance_counter.get_performance_counter_value(&pointer_to_performance_data_block,object_index,counter_index,sz_instance);
		if(!pointer_to_performance_data_block)
		{
			return 0;
		}
		performance_counter_time=pointer_to_performance_data_block->PerfTime100nSec;
		if(m_first_time)
		{
			m_first_time=false;
			m_old_counter_value=counter_value;
			m_old_performance_counter_time=performance_counter_time;
			return 0;
		}
		LONGLONG delta_counter_value=counter_value-m_old_counter_value;
		double delta_performance_counter_time=(double)performance_counter_time.QuadPart-(double)m_old_performance_counter_time.QuadPart;
		m_old_counter_value=counter_value;
		m_old_performance_counter_time=performance_counter_time;
		double idle_percentage_1=(double)delta_counter_value/delta_performance_counter_time;
		double busy_percentage_100=(1.0-idle_percentage_1)*100.0;
		disk_idle_percentage=(int)(busy_percentage_100+0.5);
		if(disk_idle_percentage<0)
		{
			return 0;
		}
		return disk_idle_percentage;
	}
	int class_performance_monitor::get_cpu_busy_percentage()
	{
		if(m_first_time)
		{
			enable_performance_counters();
		}
		class_performance_counter<LONGLONG>performance_counter;
		TCHAR sz_instance[256]=
		{
			0
		};
		DWORD object_index;
		DWORD counter_index;
		object_index=PROCESSOR_OBJECT_INDEX;
		counter_index=PROCESSOR_TIME_COUNTER_INDEX;
		_tcscpy_s(sz_instance,_T("_Total"));
		int cpu_performance=0;
		LONGLONG counter_value=0;
		PPERF_DATA_BLOCK pointer_to_performance_data_block=NULL;
		LARGE_INTEGER PerfTime100nSec=
		{
			0
		};
		counter_value=performance_counter.get_performance_counter_value(&pointer_to_performance_data_block,object_index,counter_index,sz_instance);
		if(pointer_to_performance_data_block==NULL)
		{
			return 0;
		}
		PerfTime100nSec=pointer_to_performance_data_block->PerfTime100nSec;
		if(m_first_time)
		{
			m_first_time=false;
			m_old_counter_value=counter_value;
			m_old_performance_counter_time=PerfTime100nSec;
			return 0;
		}
		LONGLONG delta_counter_value=counter_value-m_old_counter_value;
		double DeltaPerfTime100nSec=(double)PerfTime100nSec.QuadPart-(double)m_old_performance_counter_time.QuadPart;
		m_old_counter_value=counter_value;
		m_old_performance_counter_time=PerfTime100nSec;
		double idle_percentage_1=(double)delta_counter_value/DeltaPerfTime100nSec;
		double busy_percentage_100=(1.0-idle_percentage_1)*100.0;
		cpu_performance=(int)(busy_percentage_100+0.5);
		if(cpu_performance<0)
		{
			return 0;
		}
		return cpu_performance;
	}
}
