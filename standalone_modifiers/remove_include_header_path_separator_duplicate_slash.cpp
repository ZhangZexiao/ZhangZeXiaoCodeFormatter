#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_remove_include_header_path_separator_duplicate_slash.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, new DecoratingModifer_remove_empty_token(new Modifier_remove_include_header_path_separator_duplicate_slash()));
}