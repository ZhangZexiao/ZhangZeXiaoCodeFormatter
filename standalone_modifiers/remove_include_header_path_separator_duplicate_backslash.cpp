#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_remove_include_header_path_separator_duplicate_backslash.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, DecoratingModifer_remove_empty_token(new Modifier_remove_include_header_path_separator_duplicate_backslash()));
}