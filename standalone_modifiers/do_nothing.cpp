#include "..\common\BaseMain.h"
int main(int argc, char**argv)
{
	BaseMain::Main(argc, argv, new DummyModifier());
}