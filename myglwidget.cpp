#include "MyGLWidget.h"
//#include "SOIL.h"

GLuint loadBMP_custom(const char* imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char* data;
	// Open the file
	FILE* file = fopen(imagepath, "rb");
	if (!file)
	{
		printf("Image could not be openedn");
		return 0;
	}
	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP filen");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP filen");
		return 0;
	}
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return textureID;
}

void Background(GLuint texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	const int iw = 10;
	const int ih = 10;
	glPushMatrix();
	glTranslatef(-iw / 2, -ih / 2, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2i(0, 0);
	glTexCoord2d(1.0, 0.0); glVertex2i(iw, 0);
	glTexCoord2d(1.0, 1.0); glVertex2i(iw, ih);
	glTexCoord2d(0.0, 1.0); glVertex2i(0, ih);
	glEnd();
	glPopMatrix();
	return;
}


/*###################################################
##  ??????: MyGLWidget
##  ?????????????????????????????????
##  ???????????????
##		parent ????????????
#####################################################*/
MyGLWidget::MyGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	jumper = new JumpAbleClass();
	jumper->color[0] = 0.5;
	jumper->color[1] = 0.5;
	jumper->color[2] = 0.5;
	//texture = LoadTexture("2.jpg", 512, 512);
	ifstream ifile("chess.obj");
	obj = LoadOBJ(ifile);
	int count = 0;
	GLfloat color[3];
	srand((unsigned)time(NULL));
	for (int i = 0; i < 4; i++) { //?????????????????????
		for (int i = 0; i < 3; i++) {
			color[i] = (float)rand()/255.0;
			int temp = (int)color[i];
			color[i] -= temp;
			if (color[i] > 0.8)
				color[i] -= 0.1;
			if (color[i] < 0.2)
				color[i] += 0.1;
		}
		JumpCubeClass temp(color);
		temp.posX = count + 4;
		temp.posZ = 0;
		count += 4;
		temp.nextP = true;
		cubeList.insert(cubeList.end(), temp);
	}
}

/*###################################################
##  ??????: initializeGL
##  ????????????????????????????????????
##  ??????????????????
#####################################################*/
void MyGLWidget::initializeGL(){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //????????????
    glShadeModel(GL_SMOOTH);              //??????????????????

    glClearDepth(1.0);                                 //??????????????????
    glEnable(GL_DEPTH_TEST);                           //??????????????????
    glDepthFunc(GL_LEQUAL);                            //???????????????????????????
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //?????????????????????????????????

    GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};   //???????????????
    GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};   //???????????????
    GLfloat LightPosition[] = {2.0f, 3.0f, -6.0f, 1.0f}; //????????????
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);      //???????????????
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);      //???????????????
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);    //??????????????????
    glEnable(GL_LIGHT1);                                 //??????????????????
    glEnable(GL_LIGHTING);                               //??????????????????
    setList();
}


/*###################################################
##  ??????: resizeGL
##  ??????????????????????????????????????????????????????view??????
##  ???????????????
##      w ?????????????????????
##		h ?????????????????????
#####################################################*/
void MyGLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspect;
	glViewport(0, 0, w, h);

	if (w >= h) {
		aspect = w / h;
		glOrtho(-10 * aspect, 10 * aspect, -10, 10, -10, 100);
	}
	else {
		aspect = h / w;
		glOrtho(-10, 10, -10 * aspect, 10 * aspect, -10, 10);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*###################################################
##  ??????: setList
##  ????????????????????????????????????????????????????????????????????????????????????
##  ??????????????????
#####################################################*/
void MyGLWidget::setList() {
	m_box = glGenLists(2);
	glNewList(m_box, GL_COMPILE);
	glBegin(GL_QUADS);

	glVertex3f(1.0f, 1.0f, -1.0f);                  //??????(??????)  
	glVertex3f(-1.0f, 1.0f, -1.0f);                 //??????(??????)  
	glVertex3f(-1.0f, 1.0f, 1.0f);                  //??????(??????)  
	glVertex3f(1.0f, 1.0f, 1.0f);                   //??????(??????)  
	glNormal3f(1.0f, 1.0f, -1.0f);                  //??????(??????)  
	glNormal3f(-1.0f, 1.0f, -1.0f);                 //??????(??????)  
	glNormal3f(-1.0f, 1.0f, 1.0f);                  //??????(??????)  
	glNormal3f(1.0f, 1.0f, 1.0f);                   //??????(??????) 


	glVertex3f(1.0f, -1.0f, 1.0f);                  //??????(??????)  
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //??????(??????)  
	glVertex3f(-1.0f, -1.0f, -1.0f);                //??????(??????)  
	glVertex3f(1.0f, -1.0f, -1.0f);                 //??????(??????)  


	glVertex3f(1.0f, 1.0f, 1.0f);                   //??????(??????)  
	glVertex3f(-1.0f, 1.0f, 1.0f);                  //??????(??????)  
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //??????(??????)  
	glVertex3f(1.0f, -1.0f, 1.0f);                  //??????(??????)  
	glNormal3f(1.0f, 1.0f, 1.0f);                   //??????(??????)  
	glNormal3f(-1.0f, 1.0f, 1.0f);                  //??????(??????)  
	glNormal3f(-1.0f, -1.0f, 1.0f);                 //??????(??????)  
	glNormal3f(1.0f, -1.0f, 1.0f);                  //??????(??????)

	glVertex3f(1.0f, -1.0f, -1.0f);                 //??????(??????)  
	glVertex3f(-1.0f, -1.0f, -1.0f);                //??????(??????)  
	glVertex3f(-1.0f, 1.0f, -1.0f);                 //??????(??????)  
	glVertex3f(1.0f, 1.0f, -1.0f);                  //??????(??????)  


	glVertex3f(-1.0f, 1.0f, 1.0f);                  //??????(??????)  
	glVertex3f(-1.0f, 1.0f, -1.0f);                 //??????(??????)  
	glVertex3f(-1.0f, -1.0f, -1.0f);                //??????(??????)  
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //??????(??????)  


	glVertex3f(1.0f, 1.0f, -1.0f);                  //??????(??????)  
	glVertex3f(1.0f, 1.0f, 1.0f);                   //??????(??????)  
	glVertex3f(1.0f, -1.0f, 1.0f);                  //??????(??????)  
	glVertex3f(1.0f, -1.0f, -1.0f);                 //??????(??????)  
	glNormal3f(1.0f, 1.0f, -1.0f);                  //??????(??????)  
	glNormal3f(1.0f, 1.0f, 1.0f);                   //??????(??????)  
	glNormal3f(1.0f, -1.0f, 1.0f);                  //??????(??????)  
	glNormal3f(1.0f, -1.0f, -1.0f);                 //??????(??????)  
	glEnd();
	glEndList();

	s_box = m_box + 1;
	glNewList(s_box, GL_COMPILE);
	glScalef(0.3, 0.3, 0.3);
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < this->obj.size(); i++) {
		glVertex3f(obj[i].point[0], obj[i].point[1], obj[i].point[2]);
		glNormal3f(obj[i].normal[0], obj[i].normal[1], obj[i].normal[2]);
	}
	glEnd();
	glEndList();

}

/*###################################################
##  ??????: paintGL
##  ???????????????update??????
##  ??????????????????
#####################################################*/
void MyGLWidget::paintGL() {
	glClearColor(0.85f, 1.0f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //??????????????????????????? 
	//glDisable(GL_DEPTH_TEST);
	//
	//glEnable(GL_TEXTURE_2D);
	//glLoadIdentity();
	//Background(this->texture);					//????????????

	glEnable(GL_DEPTH_TEST);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 5.0 };
	GLfloat light_position[] = { 1.0, 0.0, 1.0,0.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	float scale1f = ss / 15;
	if (scale1f > 0.5)
		scale1f = 0.5;




	for (iter = cubeList.begin(); iter != cubeList.end(); iter++) {

		GLfloat mat_ambient[] = { iter->GetColor(0), iter->GetColor(1), iter->GetColor(2), 1.0 };
		GLfloat mat_diffuse[] = { iter->GetColor(0), iter->GetColor(1), iter->GetColor(2), 1.0 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

		JumpCubeClass tCube = *iter;
		glLoadIdentity();
		gluLookAt(-1, 3, 5, 0.0, 0, 0, 0, 1, 0);
		glRotatef(30, 0.0f, 1.0f, 0.0f);
		
		glTranslatef(-offsetX, 0, -offsetZ);
		if (cubeList.at(this->cubeCount).posX == tCube.posX && cubeList.at(cubeCount).posZ == tCube.posZ) {
			glTranslatef(tCube.posX - 6, -scale1f, tCube.posZ);
			glScalef(1.0, 1.0 - scale1f, 1.0);
		}
		else {
			glTranslatef(tCube.posX - 6, 0., tCube.posZ);
			glScalef(1.0, 1.0, 1.0);
		}
		glCallList(m_box);
	}

	GLfloat mat_ambient[] = { jumper->color[0], jumper->color[1], jumper->color[2], 1.0 };
	GLfloat mat_diffuse[] = { jumper->color[0], jumper->color[1], jumper->color[2], 1.0 };


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);


	glLoadIdentity();
	gluLookAt(-1, 3, 5, 0.0, 0, 0, 0, 1, 0);
	glRotatef(30, 0.0f, 1.0f, 0.0f);
	glTranslatef(jumper->posX - 6, jumper->posY - 2*scale1f, jumper->posZ);
	glTranslatef(-offsetX, 0, -offsetZ);
	glScalef(1.0, 1.0 - scale1f, 1.0);

	glCallList(s_box);

}

/*###################################################
##  ??????: keyPreeeEvent
##  ????????????????????????????????????
##  ???????????????eventt?????????????????????????????????
#####################################################*/
void  MyGLWidget::keyPressEvent(QKeyEvent* eventt) {
	switch (eventt->key()) {
	case Qt::Key_K:
		ss += float(0.2);
		update();
		break;
	}
}

void sleep(unsigned int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/*###################################################
##  ??????: keyReleaseEvent
##  ???????????????????????????????????????????????????
##  ???????????????eventt???????????????????????????????????????????????????
#####################################################*/
void  MyGLWidget::keyReleaseEvent(QKeyEvent* eventt) {
	switch (eventt->key()) {
	case Qt::Key_K:
		if (onJump) {
			return;
		}
		if (!eventt->isAutoRepeat()) {
			onJump = true;
			float xOrZSpeed = ss / 10;
			ss = 0;
			if (xOrZSpeed > 0.5) {
				xOrZSpeed = 0.5f;
			}
			float ySpeed = float(1.2);
			float preMove = float(0.0);
			bool onOrDrop = false;//true means drop 

			do {// ??????????????????????????????
				if (cubeList.at(cubeCount + 1).nextP) {//x???
					jumper->posX += xOrZSpeed;//????????????
					preMove += xOrZSpeed;
					jumper->posY += ySpeed;
					update();
					ySpeed -= float(0.1);
					sleep(10);
				}
				else {
					jumper->posZ += xOrZSpeed;
					preMove += xOrZSpeed;
					jumper->posY += ySpeed;
					update();
					ySpeed -= float(0.1);
					sleep(10);
				}
			} while (jumper->posY >= 2.0f);

			if (cubeList.at(cubeCount + 1).nextP) {
				if ((jumper->posX - cubeList.at(cubeCount).posX) <= 1) {

					onOrDrop = false;
					score++;
				}
				else {
					if (((jumper->posX - cubeList.at(cubeCount + 1).posX) <= 1 && (jumper->posX - cubeList.at(cubeCount + 1).posX) > 0) || ((cubeList.at(cubeCount + 1).posX - jumper->posX) <= 1) && (cubeList.at(cubeCount + 1).posX - jumper->posX) > 0) {
						
						onOrDrop = false;
						score++;
						cubeCount++;
						JumpCubeClass last = cubeList.at(cubeList.size() - 1);
						JumpCubeClass newCube;
						newCube.ranPOS(last.posX, last.posZ);
						cubeList.insert(cubeList.end(), newCube);
					}
					else {
						onOrDrop = true;
						score--;
					}
				}
			}
			else {
				if ((jumper->posZ - cubeList.at(cubeCount).posZ) <= 1) {
					onOrDrop = false;
					score++;
				}
				else {
					if (((jumper->posZ - cubeList.at(cubeCount + 1).posZ) <= 1 && (jumper->posZ - cubeList.at(cubeCount + 1).posZ) > 0) || ((cubeList.at(cubeCount + 1).posZ - jumper->posZ) <= 1) && (cubeList.at(cubeCount + 1).posZ - jumper->posZ) > 0) {
						onOrDrop = false;
						score++;
						cubeCount++;
						JumpCubeClass last = cubeList.at(cubeList.size() - 1);
						JumpCubeClass newCube;
						newCube.ranPOS(last.posX, last.posZ);
						cubeList.insert(cubeList.end(), newCube);
					}
					else {
						onOrDrop = true;
						score--;
					}
				}
			}
			if (onOrDrop) {
				do {
					//?????????
					jumper->posY += ySpeed;
					update();
					ySpeed -= float(0.1);
					sleep(10);
				} while (jumper->posY >= 0.0f);
				sleep(1000);
				if (cubeList.at(cubeCount + 1).nextP) {

					jumper->posX = cubeList.at(cubeCount).posX;
					ss = 0.0;
					jumper->posY = 2;
					update();
					onOrDrop = false;
					onJump = false;
					break;
				}
				else {
					jumper->posZ = cubeList.at(cubeCount).posZ;
					ss = 0.0;
					jumper->posY = 2;
					update();
					onOrDrop = false;
					onJump = false;
					break;
				}
			}


			ss = 0.0;
			jumper->posY = 2;
			if (cubeList.at(cubeCount + 1).nextP) {//????????????
				float tempOffset = offsetX + preMove;
				while (offsetX <= tempOffset) {
					offsetX += float(0.1);
					update();
					sleep(1);
				}
			}
			else {
				float tempOffset = offsetZ + preMove;
				while (offsetZ <= tempOffset) {
					offsetZ += float(0.1);
					update();
					sleep(1);
				}
			}



			onJump = false;
		}

		break;
	}
}