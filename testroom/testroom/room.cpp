#include "Angel.h"
#include "TriMesh.h"
#include <vector>
//#include "bmp_loader.h"

#pragma comment(lib, "glew32.lib")
using namespace std;

GLuint programID;
GLuint vertexArrayID;
GLuint vertexBufferID;
GLuint vertexNormalID;
GLuint vertexIndexBuffer;

GLuint vPositionID;
GLuint vNormalID;
GLuint modelViewMatrixID;
GLuint projMatrixID;
GLuint rotationMatrixID;

GLuint lightPosID;
GLuint shadowID;

TriMesh* mesh = new TriMesh();

vec3 lightPos = vec3(0.5, 2.0, 1.0);
float rotationAngle = -25.0;

// �۲��λ��
vec3 eye(0.7, 0, 0.3);
vec3 at(-100, 0, 0);
vec3 up(0, 0, 1);

GLuint texture[3]; // ��������

namespace Camera
{
	mat4 modelMatrix(1.0);
	mat4 viewMatrix(1.0);
	mat4 projMatrix = Ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
}

void init(void)
{
	programID = InitShader("vshader.glsl", "fshader.glsl");

	// �Ӷ�����ɫ����ƬԪ��ɫ���л�ȡ������λ��
	vPositionID = glGetAttribLocation(programID, "vPosition");
	vNormalID = glGetAttribLocation(programID, "vNormal");
	modelViewMatrixID = glGetUniformLocation(programID, "modelViewMatrix");
	projMatrixID = glGetUniformLocation(programID, "projMatrix");
	rotationMatrixID = glGetUniformLocation(programID, "rotationMatrix");
	lightPosID = glGetUniformLocation(programID, "lightPos");
	shadowID = glGetUniformLocation(programID, "isShadow");

	// ��ȡ�ⲿ��άģ��
	mesh->read_off("sphere.off");

	vector<vec3f> vs = mesh->v();
	vector<vec3i> fs = mesh->f();
	vector<vec3f> ns;

	// ����ƫ������������Ӱ�ƶ���ģ��y��С�ĵ�
	vec3f shift = vs[0];
	for (int i = 0; i < vs.size(); ++i)
		if (vs[i].y < shift.y) shift = vs[i];

	// ������ģ����ÿ������ķ����������洢��ns������
	for (int i = 0; i < vs.size(); ++i)
	{
		vs[i].y -= shift.y;
		ns.push_back(vs[i] - vec3(0.0, -shift.y, 0.0));
	}

	// ����VAO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// ����VBO�����󶨶�������
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vs.size() * sizeof(vec3f), vs.data(), GL_STATIC_DRAW);

	// ����VBO�����󶨷���������
	glGenBuffers(1, &vertexNormalID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalID);
	glBufferData(GL_ARRAY_BUFFER, ns.size() * sizeof(vec3f), ns.data(), GL_STATIC_DRAW);

	// ����VBO�����󶨶�������
	glGenBuffers(1, &vertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, fs.size() * sizeof(vec3i), fs.data(), GL_STATIC_DRAW);

	// ��������
	//LoadGLTextures("Data/giger1.bmp", &texture[0]);
	//LoadGLTextures("Data/floor.bmp", &texture[1]);
	//LoadGLTextures("Data/sky3.bmp", &texture[2]);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // ��ɫ����
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

// ������
void drawRoom()
{
	glBindTexture(GL_TEXTURE_2D, texture[0]); // ��ǽ�ڵ�����
	glBegin(GL_QUADS);
	// ǰ��ǽ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	// ���ǽ��
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);

	// �ұ�ǽ��
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);

	// ���ǽ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]); // �󶨵��������
	glBegin(GL_QUADS);
	// ����      
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();
}

// �����
void drawSky()
{
	glBindTexture(GL_TEXTURE_2D, texture[2]); // ����յ�����

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5f, 0.5f); glVertex3f(0.0f, 0.0f, 50.0f); // ����ԭ��

	glTexCoord2f(1.0f, 1.0f); glVertex3f(-150.0f, -150.0f, -50.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(150.0f, -150.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(150.0f, 150.0f, -50.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-150.0f, 150.0f, -50.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-150.0f, -150.0f, -50.0f);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programID);
	glEnableVertexAttribArray(vPositionID);
	// ��������۲���󣬲����붥����ɫ��
	mat4 modelViewMatrix = Camera::viewMatrix * Camera::modelMatrix;

	glUniformMatrix4fv(modelViewMatrixID, 1, GL_TRUE, &modelViewMatrix[0][0]);
	glUniformMatrix4fv(projMatrixID, 1, GL_TRUE, &Camera::projMatrix[0][0]);
	glUniform1i(shadowID, 0); // shader�������Ƿ�Ϊ��Ӱѡ��ֱ���ɫ

	glDrawElements(GL_TRIANGLES, int(mesh->f().size() * 3), GL_UNSIGNED_INT, (void*)0);

	// ������ӰͶӰ���󣬲����붥����ɫ��
	float lx = lightPos.x;
	float ly = lightPos.y;
	float lz = lightPos.z;

	mat4 shadowProjMatrix(
		-ly, 0.0, 0.0, 0.0,
		lx, 0.0, lz, 1.0,
		0.0, 0.0, -ly, 0.0,
		0.0, 0.0, 0.0, -ly);

	shadowProjMatrix = shadowProjMatrix * modelViewMatrix;
	glUniformMatrix4fv(modelViewMatrixID, 1, GL_TRUE, &shadowProjMatrix[0][0]);
	glUniform1i(shadowID, 1);

	// ������ת����
	mat4 rotationMatrix = RotateX(rotationAngle);
	glUniformMatrix4fv(rotationMatrixID, 1, GL_TRUE, &rotationMatrix[0][0]);

	// �����λ�ô��붥����ɫ��
	glUniform3fv(lightPosID, 1, lightPos);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(
		vPositionID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(vNormalID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalID);
	glVertexAttribPointer(
		vNormalID,
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
	//glLoadIdentity(); // Each time clean the matrix
	//gluLookAt(eye.x, eye.y, eye.z,
	//	at.x, at.y, at.z,
	//	up.x, up.y, up.z); // Translate the world to the current position
	//drawRoom();
	//drawSky();
	glutSwapBuffers(); // Double Buffer  
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 0.001, 400.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q': case 'Q': case 033: exit(0); break;
	}
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: eye.y -= 0.1; break;
	case GLUT_KEY_RIGHT: eye.y += 0.1; break;
	case GLUT_KEY_UP: eye.x -= 0.1; break;
	case GLUT_KEY_DOWN: eye.x += 0.1; break;
	case GLUT_KEY_PAGE_UP: eye.z += 0.1; break;
	case GLUT_KEY_PAGE_DOWN: eye.z -= 0.1; break;
	default: return; // ����������ʱ�������ػ�
	}
	glutPostRedisplay();
}

void clean()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vertexArrayID);

	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("room");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
	clean();
	return 0;
}
