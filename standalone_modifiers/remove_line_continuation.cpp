#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_remove_line_continuation.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, new DecoratingModifer_remove_empty_token(new Modifier_remove_line_continuation()));
}