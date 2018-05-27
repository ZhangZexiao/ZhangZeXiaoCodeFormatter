#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_remove_duplicate_line_feed.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, Modifier_remove_duplicate_line_feed());
}