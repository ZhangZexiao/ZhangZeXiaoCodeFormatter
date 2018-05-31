#include "..\common\BaseMain.h"
#include "..\zzxcf\Modifier_replace_cluster_of_blank_with_space.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, new DecoratingModifer_remove_empty_token(new Modifier_replace_cluster_of_blank_with_space()));
}