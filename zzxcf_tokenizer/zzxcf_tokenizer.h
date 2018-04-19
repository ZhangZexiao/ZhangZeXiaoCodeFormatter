#include <string>
#include <list>
#include <cassert>
#define ON_TRUE_RETURN(condition,rc) {if(condition)return rc;}
#ifdef ZZXCF_TOKENIZER_EXPORTS
#define ZZXCF_TOKENIZER_API __declspec(dllexport)
#else
#define ZZXCF_TOKENIZER_API __declspec(dllimport)
#endif
ZZXCF_TOKENIZER_API void tokenize(const std::string&s,std::list<std::string>&phrase);
