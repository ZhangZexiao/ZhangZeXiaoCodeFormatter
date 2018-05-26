#include "..\zzxcf\BaseMain.h"
#include "..\zzxcf\Modifier_replace_cluster_of_blank_with_space.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, Modifier_replace_cluster_of_blank_with_space());
}