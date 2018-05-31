#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_convert_include_header_path_separator_from_backslash_to_slash.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, DecoratingModifer_remove_empty_token(new Modifier_convert_include_header_path_separator_from_backslash_to_slash()));
}