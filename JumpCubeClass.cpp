#include "JumpCubeClass.h"
#include<ctime>

/*###################################################
##  ����: JumpCubeClass
##  ����������������ɫ��ʼ��������ɫ
##  ����������
##		*Color RGB������ɫ����������
#####################################################*/
JumpCubeClass::JumpCubeClass(GLfloat* Color){
	if (Color == nullptr){
		this->color[0] = 1.0;
		this->color[1] = 1.0;
		this->color[2] = 0.0;
	}else{
		for(int i=0;i < 3;i++){
			this->color[i] = Color[i];
		}
	}
}

/*###################################################
##  ����: GetColor
##  ��������������RGB��ɫ�ķ���
##  ����������
##		index ָ�����صķ�����R��G��B����һ��
#####################################################*/
GLfloat JumpCubeClass::GetColor(int index){
	return this->color[index];
}

/*###################################################
##  ����: ranPOS
##  ����������������ǰ�����λ�ã�������һ�������λ��
##  ����������
##		x ��ǰ������x���λ��
##		z ��ǰ������z���λ��
#####################################################*/
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
	for (int i = 0; i < 3; i++) {
		color[i] = (float)rand() / 255.0;
		int temp = (int)color[i];
		color[i] -= temp;
		if (color[i] > 0.8)
			color[i] -= 0.1;
		if (color[i] < 0.2)
			color[i] += 0.1;
	}
}