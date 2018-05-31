#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_remove_line_continuation.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, Modifier_remove_line_continuation());
}