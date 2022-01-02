#pragma once
#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include "JumpCubeClass.h"
#include "JumpAbleClass.h"
#include "objUtils.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>


using namespace std;


class JumpCubeClass;
class JumpAbleClass;
class Loader;

class ObjLoader {
public:
	ObjLoader(string filename);//���캯��
	void Draw();//���ƺ���
private:
	vector<vector<GLfloat>>vSets;//��Ŷ���(x,y,z)����
	vector<vector<GLint>>fSets;//������������������
};

class MyGLWidget : public QOpenGLWidget{
	Q_OBJECT

public:
	MyGLWidget(QWidget* parent = Q_NULLPTR);
	void setList();
	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	bool onJump = false;
	int cubeCount = 0;
	float offsetX = 0;
	float offsetZ = 0;
	float ss = 0.0;
	GLuint m_box;
	GLuint s_box;
	vector<JumpCubeClass> cubeList;
	vector<JumpCubeClass>::iterator iter;
	JumpAbleClass* jumper;
	string filePath;
	vector<Vertex> obj;

};



#endif // MYGLWIDGET_H