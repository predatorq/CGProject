#pragma once
#include"myglwidget.h"
class JumpCubeClass
{
public:
	JumpCubeClass() {}
	~JumpCubeClass() {}

	int posX;
	int posZ;
	bool nextP; //true Ϊx�᷽��falseΪz�᷽��
	void ranPOS(int x, int z);
};

