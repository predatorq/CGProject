#pragma once
#include"myglwidget.h"
class JumpCubeClass
{
public:
	JumpCubeClass() {}
	JumpCubeClass(GLfloat* Color);
	~JumpCubeClass() {}

	GLfloat GetColor(int index);


	GLfloat color[3];
	int posX;
	int posZ;
	bool nextP; //true Ϊx�᷽��falseΪz�᷽��
	void ranPOS(int x, int z);
};

