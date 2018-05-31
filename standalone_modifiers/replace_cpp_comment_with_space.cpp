#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_replace_cpp_comment_with_space.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, DecoratingModifer_remove_empty_token(new Modifier_replace_cpp_comment_with_space()));
}