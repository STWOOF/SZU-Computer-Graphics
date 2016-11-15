#include "Angel.h"
#include "mat.h"
#include "vec.h"

#include <vector>
#include <string>
#include <fstream>

#pragma comment(lib, "glew32.lib")

// ������Ƭ�еĶ�������
typedef struct vIndex {
    unsigned int a, b, c;
    vIndex(int ia, int ib, int ic) : a(ia), b(ib), c(ic) {}
} vec3i;

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

int axis;

const int TRANSFORM_SCALE = 0;
const int TRANSFORM_ROTATE = 1;
const int TRANSFORM_TRANSLATE = 2;
const int ANIMATION_START = 3;
const int ANIMATION_STOP = 4;
const int ANIMATION_AXIS = 5;

const double DELTA_DELTA = 0.1;    // Delta�ı仯��
const double DEFAULT_DELTA = 0.3;    // Ĭ�ϵ�Deltaֵ

double scaleDelta = DEFAULT_DELTA;
double rotateDelta = DEFAULT_DELTA;
double translateDelta = DEFAULT_DELTA;

vec3 scaleTheta(1.0, 1.0, 1.0);    // ���ſ��Ʊ���
vec3 rotateTheta(0.0, 0.0, 0.0);    // ��ת���Ʊ���
vec3 translateTheta(0.0, 0.0, 0.0);    // ƽ�ƿ��Ʊ���

GLint matrixLocation;
int currentTransform = TRANSFORM_TRANSLATE;    // ���õ�ǰ�任
int mainWindow;
const int NUM_VERTICES = 2904;

std::string filename;
std::vector<vec3i> faces;

int nVertices = 0;
int nFaces = 0;
int nEdges = 0;

// ��λ������ĸ�����
vec3 vertices[NUM_VERTICES];

void read_off(const std::string filename)
{
	if (filename.empty())
		return;

	std::ifstream fin;
	fin.open(filename);
	std::string offchar;

	fin >> offchar; // ��ȡ��һ�е�OFF
	fin >> nVertices >> nFaces >> nEdges; // ��ȡ������Ƭ������

	// �洢����
	for (int i = 0; i < nVertices; i++)
	{
		double x, y, z;
		fin >> x >> y >> z;
		vertices[i] = vec3(x, y, z);
	}
	for (int i = 0; i < nFaces; i++)
	{
		int n, x, y, z;
		fin >> n >> x >> y >> z;
		faces.push_back(vec3i(x, y, z));
	}

	fin.close();
}

void init()
{

	// ���������������
	GLuint vao[1];
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	// ��������ʼ�����㻺�����
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * sizeof(vec3), vertices, GL_STATIC_DRAW);

    // ��������ʼ�����������������
    GLuint vertexIndexBuffer;
    glGenBuffers(1, &vertexIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(vec3i), faces.data(), GL_STATIC_DRAW);

	// ��ȡ��ɫ����ʹ��
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// �Ӷ�����ɫ���г�ʼ�������λ��
	GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pLocation);
	glVertexAttribPointer(pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// ��þ���洢λ��
	matrixLocation = glGetUniformLocation(program, "matrix");

	// ��ɫ����
	glClearColor(0, 0, 0, 0);
}

void display()
{
	// ������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���ɱ任����
	mat4 m( 1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);

    // ����任����
	switch (currentTransform)
	{
	case TRANSFORM_SCALE:
		m += Scale(scaleTheta);
		break;
	case TRANSFORM_ROTATE:
		m *= RotateX(rotateTheta.x) * RotateY(rotateTheta.y) * RotateZ(rotateTheta.z);
		break;
	case TRANSFORM_TRANSLATE:
		m += Translate(translateTheta);
		break;
	}

	// ��ָ��λ��matrixLocation�д���任����m
	glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, m);
    // �����������еĸ���������
    glDrawElements(GL_TRIANGLES, int(faces.size() * 3), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glutSwapBuffers();
}

// ͨ��Deltaֵ����Theta
void updateTheta(int axis, int sign) {
	switch (currentTransform) {
	case TRANSFORM_SCALE:
		scaleTheta[axis] += sign * scaleDelta; 
		break;
	case TRANSFORM_ROTATE: 
		rotateTheta[axis] += sign * rotateDelta; 
		break;
	case TRANSFORM_TRANSLATE: 
		translateTheta[axis] += sign * translateDelta;
		break;
	}
}

// ��ԭTheta��Delta
void resetTheta()
{
	scaleTheta = vec3(1.0, 1.0, 1.0);
	rotateTheta = vec3(0.0, 0.0, 0.0);
	translateTheta = vec3(0.0, 0.0, 0.0);
	scaleDelta = DEFAULT_DELTA;
	rotateDelta = DEFAULT_DELTA;
	translateDelta = DEFAULT_DELTA;
}

// ���±仯Deltaֵ
void updateDelta(int sign)
{
	switch (currentTransform) {
	case TRANSFORM_SCALE:
		scaleDelta += sign * DELTA_DELTA;
		break;
	case TRANSFORM_ROTATE:  
		rotateDelta += sign * DELTA_DELTA; 
		break;
	case TRANSFORM_TRANSLATE: 
		translateDelta += sign * DELTA_DELTA;
		break;
	}
}

// �ֶ��������̺���
void manualKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		updateTheta(X_AXIS, 1);
		break;
	case 'a':
		updateTheta(X_AXIS, -1);
		break;
	case 'w':
		updateTheta(Y_AXIS, 1);
		break;
	case 's':
		updateTheta(Y_AXIS, -1);
		break;
	case 'e':
		updateTheta(Z_AXIS, 1);
		break;
	case 'd':
		updateTheta(Z_AXIS, -1);
		break;
	case 'r':
		updateDelta(1);
		break;
	case 'f':
		updateDelta(-1);
		break;
	case 't':
		resetTheta();
		break;
	case 033:
		// Esc����
		exit(EXIT_SUCCESS);
		break;
	}
	glutPostWindowRedisplay(mainWindow);
}

// �Զ��������̺���
void autoKeyboard(unsigned char key, int x, int y)
{
	// ������ת���Լ��仯����
	switch (key)
	{
	case 'x':
		axis = X_AXIS;
		break;
	case 'y':
		axis = Y_AXIS;
		break;
	case 'z':
		axis = Z_AXIS;
		break;
	case 'r':
		updateDelta(1);
		break;
	case 'f':
		updateDelta(-1);
		break;
	}
	glutPostWindowRedisplay(mainWindow);
}

// �Զ�������ת����
void idleFunction()
{
	// �����ı���ת��
	glutKeyboardFunc(autoKeyboard);
	// ����updateTheta��ת
	updateTheta(axis, 1);
	// ����������ػ�
	glutPostWindowRedisplay(mainWindow);
}

// ������
void mainWindowMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// ������������ָ����û�������¼�����ʱ������idleFunction
		currentTransform = TRANSFORM_ROTATE;
		glutIdleFunc(idleFunction);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// ��������Ҽ���������ã��ָ��ֶ�����ָ��
		glutIdleFunc(NULL);
		glutKeyboardFunc(manualKeyboard);
	}
}

// �˵�����
void menuEvents(int menuChoice)
{
	switch (menuChoice)
	{
		// ���ò�������
	case TRANSFORM_SCALE: case TRANSFORM_ROTATE: case TRANSFORM_TRANSLATE:
		currentTransform = menuChoice;
		break;
		// ����idleFunction���ƶ�����ʼ
	case ANIMATION_START:
		currentTransform = TRANSFORM_ROTATE;
		glutIdleFunc(idleFunction);
		break;
		// ���ƶ���ֹͣ���ָ��ֶ�����ָ��
	case ANIMATION_STOP:
		glutIdleFunc(NULL);
		glutKeyboardFunc(manualKeyboard);
		break;
		// ������ת��
	case ANIMATION_AXIS + X_AXIS: case ANIMATION_AXIS + Y_AXIS: case ANIMATION_AXIS + Z_AXIS:
		axis = menuChoice - ANIMATION_AXIS;
	default:
		break;
	}
}

// �˵�ѡ���趨
void setupMenu()
{
	int sub = glutCreateMenu(menuEvents);
	glutAddMenuEntry("X", ANIMATION_AXIS + X_AXIS);
	glutAddMenuEntry("Y", ANIMATION_AXIS + Y_AXIS);
	glutAddMenuEntry("Z", ANIMATION_AXIS + Z_AXIS);
	glutCreateMenu(menuEvents);
	glutAddMenuEntry("Scale", TRANSFORM_SCALE);
	glutAddMenuEntry("Rotate", TRANSFORM_ROTATE);
	glutAddMenuEntry("Translate", TRANSFORM_TRANSLATE);
	glutAddMenuEntry("Start Animation", ANIMATION_START);
	glutAddMenuEntry("Stop Animation", ANIMATION_STOP);
	glutAddSubMenu("Rotation Axis", sub);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void printHelp() {
	printf("%s\n\n", "3D Transfomations");
	printf("Keyboard options:\n");
	printf("	r: Increase delta of currently selected transform\n");
	printf("	f: Decrease delta of currently selected transform\n");
	printf(" - While not in animation\n");
	printf("	q: Increase x\n");
	printf("	a: Decrease x\n");
	printf("	w: Increase y\n");
	printf("	s: Decrease y\n");
	printf("	e: Increase z\n");
	printf("	d: Decrease z\n");
	printf("	t: Reset all transformations and deltas\n");
	printf("	Esc: Exit Program\n");
	printf(" - While in animation\n");
	printf("	x: Set X Axis as rotation axis\n");
	printf("	y: Set Y Axis as rotation axis\n");
	printf("	z: Set Z Axis as rotation axis\n");
	printf("Mouse options:\n");
	printf("	Left Button: Start Animation\n");
	printf("	Right Button: Stop Animation\n");
	printf("	Middle Button: Open Menu\n");
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    // ����֧��˫�ػ������Ȳ���
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	mainWindow = glutCreateWindow("2015160196_������_ʵ��2");

	glewExperimental = GL_TRUE;
	glewInit();
	
	// ����OFF
	read_off("cow.off");

	init();
	setupMenu();
	glutDisplayFunc(display);
	// ���ü�����
	glutMouseFunc(mainWindowMouse);
	glutKeyboardFunc(manualKeyboard);
	// ���������Ϣ
	printHelp();
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}
