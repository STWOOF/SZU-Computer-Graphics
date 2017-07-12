/*
*        Computer Graphics Course - Shenzhen University
*       Week 7 - Shadow Projection Matrix Skeleton Code
* ============================================================
*
* - ����������ǲο����룬����Ҫ����ο���ҵ˵��������˳������ɡ�
* - ��������OpenGL�����������������У���ο���һ��ʵ��γ�����ĵ���
*/

#include "Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>

using namespace std;

GLuint programID;
GLuint vertexArrayID;
GLuint vertexBufferID;
GLuint vertexIndexBuffer;

GLuint vPositionID;
GLuint rotationMatrixID;
GLuint modelViewMatrixID;
GLuint projMatrixID;
GLuint fColorID;

// ����ӽǲ���
float l = -2.0, r = 2.0;    // ���Ҳü�ƽ��
float b = -2.0, t = 2.0;    // ���²ü�ƽ��
float n = -2.0, f = 2.0;    // Զ���ü�ƽ��
float rotationAngle = -5.0; // ��ת�Ƕ�

vec4 red(1.0, 0.0, 0.0, 1.0);
vec4 black(0.0, 0.0, 0.0, 1.0);

float lightPos[3] = {-0.5, 2.0, 0.5};

//////////////////////////////////////////////////////////////////////////
// �����������

namespace Camera
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;

	mat4 ortho( const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar )
	{
		// TODO �밴��ʵ��ν��ⲹȫ����ͶӰ����ļ���
		return mat4(1.0);
	}

	mat4 perspective( const GLfloat fovy, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar)
	{
		// TODO �밴��ʵ��ν��ⲹȫ͸��ͶӰ����ļ���
		return mat4(1.0);
	}

	mat4 lookAt(const vec4& eye, const vec4& at, const vec4& up)
	{
		// TODO �밴��ʵ������ݲ�ȫ����۲����ļ���
		return mat4(1.0);
	}
}

//////////////////////////////////////////////////////////////////////////
// OpenGL ��ʼ��

void init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	vec3 points[3];
	points[0] = vec3( -0.5, 0.5,  0.5);
	points[1] = vec3(  0.5, 0.5,  0.5);
	points[2] = vec3(  0.0, 0.75, 0.0);

	programID = InitShader("vshader.glsl", "fshader.glsl");

	// �Ӷ�����ɫ���л�ȡ��Ӧ������λ��
	vPositionID = glGetAttribLocation(programID, "vPosition");

	rotationMatrixID = glGetUniformLocation(programID, "rotationMatrix");
	modelViewMatrixID = glGetUniformLocation(programID, "modelViewMatrix");
	projMatrixID = glGetUniformLocation(programID, "projMatrix");
	fColorID = glGetUniformLocation(programID, "fColor");

	// ����VAO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// ����VBO�����󶨶�������
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// OpenGL��Ӧ״̬����
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}

//////////////////////////////////////////////////////////////////////////
// ��Ⱦ

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	// TODO ����ģ-�ӱ任����ͶӰ����
	// ����Ϊ�����ص㲻����ͶӰ�任�����Խ�ͶӰ��������Ϊ����ͶӰ���ɣ�
	//
	Camera::modelMatrix = mat4(1.0);
	Camera::viewMatrix = mat4(1.0);
	Camera::projMatrix = Ortho(-2, 2, -2, 2, -2, 2);

	// Ϊ����۲�ͶӰ�����ó�����ת����
	mat4 rotationMatrix = RotateX(rotationAngle);
	glUniformMatrix4fv(rotationMatrixID, 1, GL_TRUE, &rotationMatrix[0][0]);

	glEnableVertexAttribArray(vPositionID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
		vPositionID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	// TODO ��������ͶӰ�����»���ԭʼ�������Σ��ú�ɫ��ʾ��
	mat4 modelViewMatrix = Camera::viewMatrix * Camera::modelMatrix;
	mat4 projMatrix = Camera::projMatrix;

	glUniformMatrix4fv(modelViewMatrixID, 1, GL_TRUE, modelViewMatrix);
	glUniformMatrix4fv(projMatrixID, 1, GL_TRUE, projMatrix);

	glUniform4fv(fColorID, 1, red);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// TODO ������ӰͶӰ���󣬻���ͶӰ֮��������Σ��ú�ɫ��ʾ��
	float lx = lightPos[0];
	float ly = lightPos[1];
	float lz = lightPos[2];

	mat4 shadowProjMatrix(-ly, 0.0, 0.0, 0.0,
		lx, 0.0, lz, 1.0,
		0.0, 0.0, -ly, 0.0,
		0.0, 0.0, 0.0, -ly);

	glUniformMatrix4fv(modelViewMatrixID, 1, GL_TRUE, &shadowProjMatrix[0][0]);

	glUniform4fv(fColorID, 1, black);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(vPositionID);
	glUseProgram(0);

	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// �������ô���

void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// �����Ӧ����

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		lightPos[0] = (x - 250) / 10;
		lightPos[1] = (250 - y) / 10;
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧ����

void keyboard(unsigned char key, int x, int y)
{
	switch(key) 
	{
	case 033:
		exit(EXIT_SUCCESS);
		break;
	case 'q':
		exit (EXIT_SUCCESS);
		break;
	case 'x':		// 'x' ��ʹ�ó�����ת-1.0��
		rotationAngle -= 1.0;
		break;
	case 'X':		// 'X' ��ʹ�ó�����ת+1.0��
		rotationAngle += 1.0;
		break;
	}
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////

void idle(void)
{
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////

void clean()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vertexArrayID);
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OpenGL-Tutorial");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	clean();

	return 0;
}