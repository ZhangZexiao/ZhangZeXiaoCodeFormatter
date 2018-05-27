#include "zzxcf_file_operation.h"
#include "zzxcf_command_line_option.h"
#include "..\common\Macro.h"
namespace file_operation_namespace
{
	std::string extract_file_name_from_file_path(const std::string&file_path)
	{
		assert(file_path.length()>0);
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char file_name[_MAX_FNAME];
		char ext[_MAX_EXT];
		::_splitpath_s(file_path.c_str(),drive,_MAX_DRIVE,dir,_MAX_DIR,file_name,_MAX_FNAME,ext,_MAX_EXT);
		return std::string(file_name);
	}
	bool file_exists(const std::string&file_path)
	{
		assert(file_path.length()>0);
		int hr= ::_access(file_path.c_str(),0);
		if(0==hr)
		{
			::fprintf(stdout,"File exists: %s\n",file_path.c_str());
		}
		else
		{
			::fprintf(stdout,"File NOT exists: %s\n",file_path.c_str());
		}
		return 0==hr;
	}
	bool remove_file(const std::string&file_path)
	{
		assert(file_path.length()>0);
		::fprintf(stdout,"Remove file: %s\n",file_path.c_str());
		int hr= ::remove(file_path.c_str());
		if(0!=hr)
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			::strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: remove file failure, error message \"%s\", file path \"%s\"\n",buffer,file_path.c_str());
		}
		assert(hr==0);
		return hr==0;
	}
	bool rename_file(const std::string&old_file_path,const std::string&new_file_path)
	{
		assert(old_file_path.length()>0&&new_file_path.length()>0);
		::fprintf(stdout,"Rename file:\n\tOld file path: %s\n\tNew file path: %s\n",old_file_path.c_str(),new_file_path.c_str());
		int hr= ::rename(old_file_path.c_str(),new_file_path.c_str());
		if(0!=hr)
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: rename file failure, error message \"%s\", \"%s\"\t\"%s\"\n",buffer,old_file_path.c_str(),new_file_path.c_str());
		}
		assert(hr==0);
		return hr==0;
	}
	bool is_readonly(const std::string&file_path)
	{
		assert(file_path.length()>0);
		assert(file_exists(file_path));
		int hr= ::_access(file_path.c_str(),2);
		if(0!=hr)
		{
			::fprintf(stdout,"Is readonly: %s\n",file_path.c_str());
		}
		else
		{
			::fprintf(stdout,"Is NOT readonly: %s\n",file_path.c_str());
		}
		return 0!=hr;
	}
	bool clear_readonly(const std::string&file_path)
	{
		assert(file_path.length()>0);
		assert(file_exists(file_path));
		assert(is_readonly(file_path));
		::fprintf(stdout,"Clear readonly: %s\n",file_path.c_str());
		int hr= ::_chmod(file_path.c_str(),S_IWRITE);
		if(0!=hr)
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: clear readonly failure, error message \"%s\", file path \"%s\"\n",buffer,file_path.c_str());
		}
		assert(0==hr);
		return 0==hr;
	}
	size_t get_file_size(const std::string&file_path)
	{
		assert(file_path.length()>0);
		struct stat s;
		int hr= ::stat(file_path.c_str(),&s);
		if(0!=hr)
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: get file size failure, error message \"%s\", file path \"%s\"\n",buffer,file_path.c_str());
		}
		assert(0==hr);
		ON_TRUE_RETURN(0==hr,s.st_size);
		return 0;
	}
	bool open_file(const std::string&file_path,const std::string&mode,FILE**pp_file)
	{
		assert(0!=pp_file&&0==(*pp_file)&&file_path.length()>0&&mode.length()>0);
		::fopen_s(pp_file,file_path.c_str(),mode.c_str());
		if(0==(*pp_file))
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: open file failure, error message \"%s\", file path \"%s\"\n",buffer,file_path.c_str());
		}
		assert(0!=(*pp_file));
		return 0!=(*pp_file);
	}
	bool close_file(FILE*p_file)
	{
		assert(p_file!=0);
		ON_TRUE_RETURN(p_file==0,true);
		int hr= ::fclose(p_file);
		if(0!=hr)
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: close file failure, error message \"%s\"\n",buffer);
		}
		assert(0==hr);
		return 0==hr;
	}
	bool read_file(const std::string&file_path,std::string&content)
	{
		assert(file_path.length()>0&&0==content.length());
		assert(file_exists(file_path));
		fprintf(stdout,"Read file, file path \"%s\"\n",file_path.c_str());
		content.clear();
		ON_TRUE_RETURN(!file_exists(file_path),false);
		FILE*p_file=0;
		ON_TRUE_RETURN(!open_file(file_path,"r",&p_file),false);
		const size_t size_of_buffer=40960;
		char buffer[size_of_buffer+1];
		::memset(buffer,0,size_of_buffer+1);
		try
		{
			while(::fgets(buffer,size_of_buffer,p_file))
			{
				content+=buffer;
			}
		}
		catch(...)
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: read file failure, error message \"%s\", file path \"%s\"\n",buffer,file_path.c_str());
			ON_TRUE_RETURN(!close_file(p_file),false);
			return false;
		}
		ON_TRUE_RETURN(!close_file(p_file),false);
		return true;
	}
	bool write_file(const std::string&file_path,const std::string&content)
	{
		assert(file_path.length()>0);
		fprintf(stdout,"Write file, file path \"%s\"\n",file_path.c_str());
		if(file_exists(file_path))
		{
			if(is_readonly(file_path))
			{
				fprintf(stderr,"Warning: is readonly, file path \"%s\"\n",file_path.c_str());
				clear_readonly(file_path);
			}
		}
		FILE*p_file=0;
		ON_TRUE_RETURN(!open_file(file_path,"w",&p_file),false);
		int hr= ::fputs(content.c_str(),p_file);
		if(0!=hr)
		{
			const size_t buffer_count=1024;
			char buffer[buffer_count];
			strerror_s(buffer,buffer_count,errno);
			::fprintf(stderr,"Error: write file failure, error message \"%s\", file path \"%s\"\n",buffer,file_path.c_str());
		}
		assert(0==hr);
		ON_TRUE_RETURN(!close_file(p_file),false);
		return 0==hr;
	}
	std::string generate_unique_file_name(std::string file_name,const std::string&file_extension)
	{
		assert(file_name.length()>0&&file_extension.length()>0&&file_extension.find(".")==0);
		if(!command_line_namespace::option_do_not_search_for_absolute_file_path.is_turned_on()&&file_name.length()>1&&file_name[1]!=':')
		{
			char current_directory[_MAX_PATH*10];
			GetCurrentDirectoryA(_MAX_PATH*10,current_directory);
			file_name=std::string(current_directory)+std::string("\\")+file_name;
		}
		const size_t buffer_size=1024;
		char version_number[buffer_size];
		int i_version_number=0;
		std::string unique_file_name;
		do
		{
			::sprintf_s(version_number,buffer_size,".%d",i_version_number);
			unique_file_name=file_name+version_number+file_extension;
			i_version_number++;
		}
		while(file_exists(unique_file_name));
		return unique_file_name;
	}
	std::string get_latest_orig_file(std::string file_name)
	{
		assert(file_name.length()>0);
		if(!command_line_namespace::option_do_not_search_for_absolute_file_path.is_turned_on()&&file_name.length()>1&&file_name[1]!=':')
		{
			char current_directory[_MAX_PATH*10];
			GetCurrentDirectoryA(_MAX_PATH*10,current_directory);
			file_name=std::string(current_directory)+std::string("\\")+file_name;
		}
		const size_t buffer_size=1024;
		char version_number[buffer_size];
		int i_version_number=0;
		std::string unique_file_name,latest_orig_file;
		do
		{
			latest_orig_file=unique_file_name;
			::sprintf_s(version_number,buffer_size,".%d",i_version_number);
			unique_file_name=file_name+version_number+".orig";
			i_version_number++;
		}
		while(file_exists(unique_file_name));
		return latest_orig_file;
	}
	void search_files_in_directory(std::string file_matching_pattern,std::list<std::string>&files)
	{
		if(!command_line_namespace::option_do_not_search_for_absolute_file_path.is_turned_on()&&file_matching_pattern.length()>1&&file_matching_pattern[1]!=':')
		{
			char current_directory[_MAX_PATH*10];
			GetCurrentDirectoryA(_MAX_PATH*10,current_directory);
			file_matching_pattern=std::string(current_directory)+std::string("\\")+file_matching_pattern;
		}
		char path[_MAX_PATH*10];
		char drive[_MAX_DRIVE*10];
		char dir[_MAX_DIR*10];
		char file_name[_MAX_FNAME*10];
		char ext[_MAX_EXT*10];
		::_splitpath_s(file_matching_pattern.c_str(),drive,_MAX_DRIVE*10,dir,_MAX_DIR*10,file_name,_MAX_FNAME*10,ext,_MAX_EXT*10);
		_finddata_t find_data;
		intptr_t handle=_findfirst((char*)file_matching_pattern.c_str(),&find_data);
		if(-1!=handle)
		{
			do
			{
				if(find_data.attrib&_A_SUBDIR)
				{
					continue;
				}
				::_makepath_s(path,drive,dir,find_data.name,0);
				files.push_back(path);
			}
			while(::_findnext(handle,&find_data)==0);
			::_findclose(handle);
		}
	}
	void search_files_in_directory_subdirectory(std::string file_matching_pattern,std::list<std::string>&files)
	{
		if(!command_line_namespace::option_do_not_search_for_absolute_file_path.is_turned_on()&&file_matching_pattern.length()>1&&file_matching_pattern[1]!=':')
		{
			char current_directory[_MAX_PATH*10];
			GetCurrentDirectoryA(_MAX_PATH*10,current_directory);
			file_matching_pattern=std::string(current_directory)+std::string("\\")+file_matching_pattern;
		}
		char path[_MAX_PATH*10];
		char drive[_MAX_DRIVE*10];
		char dir[_MAX_DIR*10];
		char file_name[_MAX_FNAME*10];
		char ext[_MAX_EXT*10];
		::_splitpath_s(file_matching_pattern.c_str(),drive,dir,file_name,ext);
		_finddata_t find_data;
		intptr_t handle=_findfirst((char*)file_matching_pattern.c_str(),&find_data);
		if(-1!=handle)
		{
			do
			{
				if(find_data.attrib&_A_SUBDIR)
				{
					continue;
				}
				::_makepath_s(path,drive,dir,find_data.name,0);
				files.push_back(path);
			}
			while(::_findnext(handle,&find_data)==0);
			::_findclose(handle);
		}
		::_makepath_s(path,drive,dir,"*",0);
		handle=_findfirst(path,&find_data);
		if(-1!=handle)
		{
			do
			{
				if(!(find_data.attrib&_A_SUBDIR))
				{
					continue;
				}
				std::string subdirectory=find_data.name;
				if(subdirectory=="."||subdirectory=="..")
				{
					continue;
				}
				::_makepath_s(path,drive,dir,subdirectory.c_str(),0);
				std::string subdirectory_file_matching_pattern=path;
				subdirectory_file_matching_pattern+="\\";
				subdirectory_file_matching_pattern+=file_name;
				subdirectory_file_matching_pattern+=ext;
				search_files_in_directory_subdirectory(subdirectory_file_matching_pattern,files);
			}
			while(::_findnext(handle,&find_data)==0);
			::_findclose(handle);
		}
	}
	void search_files(int argc,char**argv,int index,std::list<std::string>&files)
	{
		assert(argc>0);
		for(;index<argc;index++)
		{
			if(command_line_namespace::option_search_subdirectories.is_turned_on())
			{
				search_files_in_directory_subdirectory(argv[index],files);
			}
			else
			{
				search_files_in_directory(argv[index],files);
			}
		}
	}
	std::string get_file_extension(const std::string&file_name)
	{
		assert(file_name.length()>0);
		size_t dot_position=file_name.rfind('.');
		std::string file_extension
		{
			""
		};
		if(std::string::npos!=dot_position)
		{
			file_extension=file_name.substr(dot_position);
		}
		return file_extension;
	}
	std::string strip_file_extension(const std::string&file_name)
	{
		assert(file_name.length()>0);
		size_t dot_position=file_name.rfind('.');
		if(std::string::npos!=dot_position)
		{
			return file_name.substr(0,dot_position-1);
		}
		return file_name;
	}
	std::string replace_file_extension(const std::string&file_name,const std::string&new_file_extension)
	{
		return strip_file_extension(file_name)+new_file_extension;
	}
	std::string get_orig_file_matching_pattern(const std::string&file_name)
	{
		return file_name+".*.orig";
	}
	void search_command_line_orig_files(int argc,char**argv,int index,std::list<std::string>&files)
	{
		assert(argc>0);
		assert(index> -1&&index<argc);
		for(;index<argc;index++)
		{
			if(command_line_namespace::option_search_subdirectories.is_turned_on())
			{
				search_files_in_directory_subdirectory(get_orig_file_matching_pattern(argv[index]),files);
			}
			else
			{
				search_files_in_directory(get_orig_file_matching_pattern(argv[index]),files);
			}
		}
	}
	size_t find_utf8_bom(const std::string&file_content)
	{
		static const char utf8_bom[4]="\xEF\xBB\xBF";
		return file_content.find(utf8_bom);
	}
	bool is_utf8_encoded_file(const std::string&file_path)
	{
		std::string file_content;
		read_file(file_path,file_content);
		size_t utf8_bom_position=find_utf8_bom(file_content);
		if(std::string::npos!=utf8_bom_position)
		{
#ifdef _WIN64
			fprintf(stdout,"Utf8 with BOM(EFBBBF@%I64u) encoded file, file path %s\n",utf8_bom_position,file_path.c_str());
#else
			fprintf(stdout,"Utf8 with BOM(EFBBBF@%d) encoded file, file path %s\n",utf8_bom_position,file_path.c_str());
#endif
		}
		else
		{
			fprintf(stdout,"Not utf8 encoded file, file path %s\n",file_path.c_str());
		}
		return std::string::npos!=utf8_bom_position;
	}
	bool is_utf8_encoded_file_content(const std::string&file_content)
	{
		size_t utf8_bom_position=find_utf8_bom(file_content);
		return std::string::npos!=utf8_bom_position;
	}
	size_t count_number_of_line_feeds(const std::string&source)
	{
		size_t number_of_line_feeds=0;
		for(size_t i=0;i<source.length();i++)
		{
			if(source[i]=='\n')
			{
				number_of_line_feeds++;
			}
		}
		return number_of_line_feeds;
	}
}
