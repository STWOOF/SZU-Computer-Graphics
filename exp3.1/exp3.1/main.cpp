/*
*        Computer Graphics Course - Shenzhen University
*      Week 6 - Camera Position and Control Skeleton Code
* ============================================================
*
* - ����������ǲο����룬����Ҫ����ο���ҵ˵��������˳������ɡ�
* - ��������OpenGL�����������������У���ο���һ��ʵ��γ�����ĵ���
*/

#include "include/Angel.h"
#include "include/TriMesh.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>

using namespace std;

GLuint programID;
GLuint vertexArrayID;
GLuint vertexBufferID;
GLuint vertexIndexBuffer;

GLuint vPositionID;
GLuint modelViewMatrixID;

vec4 eye = vec4(0.0, 0.0, 0.0);
vec4 at = vec4(6.0, 4.0, 5.0);
vec4 up = -vec4(1.0, 1.0, 1.0);

TriMesh* mesh = new TriMesh();

namespace Camera
{
    mat4 modelMatrix;
    mat4 viewMatrix;

	mat4 LookAt( const vec4& eye, const vec4& at, const vec4& up )
	{
		// TODO �밴��ʵ������ݲ�ȫ����۲����ļ���
		vec4 vpn = at - eye;
		vec4 n = normalize(vpn);
		vec4 u = normalize(up * n);
		vec4 v = normalize(n * u);
		mat4 res = mat4 (
			(u[1], u[2], u[3], 0),
			(v[1], v[2], v[3], 0),
			(n[1], n[2], n[3], 0),
			(0, 0, 0, 1)
		);
		return res * Translate(-eye);
	}
}

//////////////////////////////////////////////////////////////////////////
// OpenGL ��ʼ��

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// ����shader���һ�ȡ������λ��
	programID = InitShader("vshader.glsl", "fshader.glsl");
	vPositionID = glGetAttribLocation(programID, "vPosition");
	modelViewMatrixID = glGetUniformLocation(programID, "modelViewMatrix");

	// ���ⲿ��ȡ��άģ���ļ�
	mesh->read_off("cow.off");

	vector<vec3f> vs = mesh->v();
	vector<vec3i> fs = mesh->f();

	// ����VAO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// ����VBO�����󶨶�������
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(vec3f), vs.data(), GL_STATIC_DRAW);

	// ����VBO�����󶨶�������
	glGenBuffers(1, &vertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, fs.size() * sizeof(vec3i), fs.data(), GL_STATIC_DRAW);

	// OpenGL��Ӧ״̬����
	glEnable(GL_LIGHTING);
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

	// TODO �����������
	
	Camera::modelMatrix = mat4(1.0);
	Camera::viewMatrix = LookAt(eye, at, up);
	
	mat4 modelViewMatrix = Camera::viewMatrix * Camera::modelMatrix;
	glUniformMatrix4fv(modelViewMatrixID, 1, GL_TRUE, &modelViewMatrix[0][0]);

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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);

	glDrawElements(
		GL_TRIANGLES,
		int(mesh->f().size() * 3),
		GL_UNSIGNED_INT,
		(void*)0
	);

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
	return;
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧ����

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033:	// ESC�� �� 'q' ���˳���Ϸ
		exit(EXIT_SUCCESS);
		break;
	case 'q':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		eye[1]++;
		break;
	case 'w':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		eye[2]++;
		break;
	case 'e':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		eye[0]++;
		break;
	case 'r':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		at[1]++;
		break;
	case 't':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		at[2]++;
		break;
	case 'y':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		at[0]++;
		break;
	case 'u':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		up[1]++;
		break;
	case 'i':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		up[2]++;
		break;
	case 'o':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		up[0]++;
		break;
	case 'a':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		eye[1]--;
		break;
	case 's':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		eye[2]--;
		break;
	case 'd':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		eye[0]--;
		break;
	case 'f':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		at[1]--;
		break;
	case 'g':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		at[2]--;
		break;
	case 'h':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		at[0]--;
		break;
	case 'j':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		up[1]--;
		break;
	case 'k':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		up[2]--;
		break;
	case 'l':
		cout << eye << endl;
		cout << at << endl;
		cout << up << endl;
		up[0]--;
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

	if (mesh) {
		delete mesh;
		mesh = NULL;
	}
}

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