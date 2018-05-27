#include "..\common\TokenSequence.h"
#ifdef ZZXCF_TOKENIZER_EXPORTS
#define ZZXCF_TOKENIZER_API __declspec(dllexport)
#else
#define ZZXCF_TOKENIZER_API __declspec(dllimport)
#endif
ZZXCF_TOKENIZER_API void tokenize(const TokenSequence::value_type&, TokenSequence&);
