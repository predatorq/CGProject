#include "JumpCubeClass.h"
#include<ctime>

void JumpCubeClass::ranPOS(int x, int z) {
	srand((unsigned)time(NULL));
	int xOrz = rand() % 2;
	if (xOrz == 0) {
		nextP = true;//x�᷽��
		posZ = z;
		posX = x + rand() % 4 + 3;

	}
	else {
		nextP = false;//z�᷽��
		posX = x;
		posZ = z + rand() % 4 + 3;
	}
}