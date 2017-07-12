/*
 *        Computer Graphics Course - Shenzhen University
 *                 Mid-term Assignment - Tetris
 *             Programmed by ALEKO LAU - 2016.10.24
 * ============================================================
 *
 * - ��ʵ�ֹ������£�
 * -  1) �������̸�
 * -  2) ������/��/�¼����Ʒ�����ƶ����ϼ���ת����
 * -  3) ������ɷ��鲢���ϲ�ͬ����ɫ
 * -  4) ����֮�����ײ���
 * -  5) ������Զ������ƶ�
 * -  6) ���̸���ÿһ�������֮���Զ����� (clearrow)
 * -  7) ����ϵͳ�������ٶ���������Ӷ��ӿ졣
 * -  8) ��ȡ�ⲿ�ļ���¼������߷�
 * -  9) ���̿�ݼ�����������Ϸ
 * - 10) ������������������Ч
 *
 */

#include "include/Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int starttime ;			// ���Ʒ��������ƶ�ʱ��
int rotation = 0;		// ���Ƶ�ǰ�����еķ�����ת
int random;             // ������ɷ������
int score;				// ��ǰ��Ϸ����
int bestscore;			// ��Ϸ��߷�
vec2 tile[4];			// ��ʾ��ǰ�����еķ���
int xsize = 400;		// ���ڴ�С��������Ҫ�䶯���ڴ�С����
int ysize = 720;
int FALL_TIME = 1000;
bool halt = false;		// ��Ϸֹͣ����
// ��ά�����ʾ���п��ܳ��ֵķ���ͷ���
vec2 allRotationsOshape[1][4] =
							  {{vec2(0, 0), vec2(-1,0), vec2(0,-1), vec2(-1,-1)}};  //   "O"
vec2 allRotationsIshape[2][4] =
							  {{vec2(0, 0), vec2(1, 0), vec2(-2, 0),vec2(-1, 0)},	//   "I"
							   {vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(0, -2)}};  
vec2 allRotationsSshape[2][4] =
							  {{vec2(0, 0), vec2(1, 0), vec2(0,-1), vec2(-1,-1)},	//   "S"
							   {vec2(0, 1), vec2(0, 0), vec2(1,-1), vec2(1,  0)}};  
vec2 allRotationsZshape[2][4] =
							  {{vec2(0, 0), vec2(-1,0), vec2(0,-1), vec2(1, -1)},	//   "Z"
							   {vec2(1, 1), vec2(0, 0), vec2(0,-1), vec2(1,  0)}};  
vec2 allRotationsLshape[4][4] =
							  {{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},	//   "L"
							   {vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1)},   
							   {vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   
							   {vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}};
vec2 allRotationsJshape[4][4] = 
							  {{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(1, -1)},	//   "J"
							   {vec2(0, 1), vec2(1, 1), vec2(0,-1), vec2(0,  0)},   
							   {vec2(-1,1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   
							   {vec2(-1,-1),vec2(0, 1), vec2(0, 0), vec2(0, -1)}};
vec2 allRotationsTshape[4][4] = 
							  {{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(0, -1)},	//   "T"
							   {vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1,  0)},   
							   {vec2(0, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   
							   {vec2(-1,0), vec2(0, 1), vec2(0, 0), vec2(0, -1)}};


// ���ƴ��ڵ���ɫ����
vec4 orange  = vec4(1.0, 0.5, 0.0, 1.0);
vec4 cyan    = vec4(0.0, 1.0, 1.0, 1.0);
vec4 magenta = vec4(1.0, 0.0, 1.0, 1.0);
vec4 yellow  = vec4(1.0, 1.0, 0.0, 1.0);
vec4 red     = vec4(1.0, 0.0, 0.0, 1.0);
vec4 green   = vec4(0.0, 1.0, 0.0, 1.0);
vec4 blue    = vec4(0.0, 0.0, 1.0, 1.0);
vec4 white   = vec4(1.0, 1.0, 1.0, 1.0);
vec4 gray    = vec4(0.4, 0.4, 0.4, 1.0);
vec4 black   = vec4(0.0, 0.0, 0.0, 1.0);

// ��ǰ�����λ�ã������̸�����½�Ϊԭ�������ϵ��
vec2 tilepos = vec2(5, 19);

// ���������ʾ���̸��ĳλ���Ƿ񱻷�����䣬��board[x][y] = true��ʾ(x,y)�����ӱ���䡣
// �������̸�����½�Ϊԭ�������ϵ��
bool board[10][20];


// �����̸�ĳЩλ�ñ��������֮�󣬼�¼��Щλ���ϱ�������ɫ
vec4 boardcolours[1200];

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

//////////////////////////////////////////////////////////////////////////
// ��ȡ��߷�����
void read_profile(const string filename)
{
	if (filename.empty())
		return;
	ifstream fin;
	fin.open(filename); // ���ļ�
		fin >> bestscore; // ��ȡ��߷�
	fin.close();
}

//////////////////////////////////////////////////////////////////////////
// д����߷�����
void write_profile(const string filename)
{
	if (filename.empty())
		return;
	ofstream fout (filename); // ��ȡ�ļ�
	if (fout.is_open())
	{
		fout << bestscore; // ������߷�
		fout.close();
	}
}

//////////////////////////////////////////////////////////////////////////
// �޸����̸���posλ�õ���ɫΪcolour�����Ҹ��¶�Ӧ��VBO
void changecellcolour(vec2 pos, vec4 colour)
{
	// ÿ�������Ǹ������Σ��������������Σ��ܹ�6�����㣬�����ض���λ�ø����ʵ�����ɫ
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6 * (10 * pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// ����ƫ���������ʵ���λ�ø�����ɫ
	int offset = 6 * sizeof(vec4) * (int)(10 * pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////
// ��ǰ�����ƶ�������תʱ������VBO
void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// ÿ����������ĸ�����
	for (int i = 0; i < 4; i++)
	{
		// ������ӵ�����ֵ
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		// ÿ�����Ӱ������������Σ�������6����������
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec4), 6 * sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ����Ϊ��һ���������ֵķ��顣����Ϸ��ʼ��ʱ�����������һ����ʼ�ķ��顣
void newtile()
{
	if (!halt) // ��Ϸ������ʱ
	{
		// ���·���������̸���������м�λ�ò�����Ĭ�ϵ���ת����
		tilepos = vec2(5, 19);
		rotation = 0;
		random = rand() % 7; // ������������Ʒ�������
		for (int i = 0; i < 4; i++)
		{
			switch (random)
			{
			case 0:
				tile[i] = allRotationsOshape[0][i];
				break;
			case 1:
				tile[i] = allRotationsIshape[0][i];
				break;
			case 2:
				tile[i] = allRotationsSshape[0][i];
				break;
			case 3:
				tile[i] = allRotationsZshape[0][i];
				break;
			case 4:
				tile[i] = allRotationsLshape[0][i];
				break;
			case 5:
				tile[i] = allRotationsJshape[0][i];
				break;
			case 6:
				tile[i] = allRotationsTshape[0][i];
				break;
			default:
				break;
			}
		}

		updatetile();

		// ���·��鸳����ɫ
		vec4 newcolours[24];
		for (int i = 0; i < 24; i++)
			switch (random)
			{
			case 0:
				newcolours[i] = orange;
				break;
			case 1:
				newcolours[i] = red;
				break;
			case 2:
				newcolours[i] = yellow;
				break;
			case 3:
				newcolours[i] = cyan;
				break;
			case 4:
				newcolours[i] = blue;
				break;
			case 5:
				newcolours[i] = green;
				break;
			case 6:
				newcolours[i] = magenta;
				break;
			default:
				break;
			}

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��Ϸ��OpenGL��ʼ��
void init()
{
	// ��ʼ�����̸񣬰���64���������꣨�ܹ�32���ߣ�������ÿ������һ����ɫֵ
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// ��ʼ������
	score = 0;

	// ������
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}

	// ˮƽ��
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// �������߸��ɻ�ɫ
	for (int i = 0; i < 64; i++)
		gridcolours[i] = gray;

	// ��ʼ�����̸񣬲���û�б����ĸ������óɺ�ɫ
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// ��ÿ�����ӣ���ʼ��6�����㣬��ʾ���������Σ�����һ�������θ���
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);

			boardpoints[6*(10*i + j)    ] = p1;
			boardpoints[6*(10*i + j) + 1] = p2;
			boardpoints[6*(10*i + j) + 2] = p3;
			boardpoints[6*(10*i + j) + 3] = p2;
			boardpoints[6*(10*i + j) + 4] = p3;
			boardpoints[6*(10*i + j) + 5] = p4;
		}

	// �����̸�����λ�õ�����������Ϊfalse��û�б���䣩
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// ������ɫ��
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// ���̸񶥵�
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// ���̸񶥵�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸񶥵���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ���̸�ÿ������
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// ���̸�ÿ�����Ӷ���λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸�ÿ�����Ӷ�����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ��ǰ����
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// ��ǰ���鶥��λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ��ǰ���鶥����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

	// ��Ϸ��ʼ��
	PlaySound("./Audio/opening.wav", NULL, SND_SYNC | SND_FILENAME); // ������Ч
	newtile(); // ��һ�鷽��
	printf("SCORE: %d", score); // ��ӡ��ʼ����
	starttime = glutGet(GLUT_ELAPSED_TIME);
}

//////////////////////////////////////////////////////////////////////////
// �����cellposλ�õĸ����Ƿ��������Ƿ������̸�ı߽緶Χ�ڡ�(��ײ)
bool checkvalid(vec2 cellpos)
{
	int cx = cellpos.x, cy = cellpos.y;
	// ���߽緶Χ
	if ((cellpos.x >= 0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y < 20))
	{ // ����Ƿ����
		if(board[cx][cy]) return false;
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// �����������㹻�ռ���������ת��ǰ����
void rotate()
{
	// ����õ���һ����ת����
	int nextrotation = (rotation + 1) % 4;

	// ��鵱ǰ��ת֮���λ�õ���Ч��
	if (checkvalid((allRotationsLshape[nextrotation][0]) + tilepos)
		&& checkvalid((allRotationsLshape[nextrotation][1]) + tilepos)
		&& checkvalid((allRotationsLshape[nextrotation][2]) + tilepos)
		&& checkvalid((allRotationsLshape[nextrotation][3]) + tilepos))
	{
		// ������ת������ǰ��������Ϊ��ת֮��ķ���
		rotation = nextrotation;
		for (int i = 0; i < 4; i++)
			switch (random) // case 0 ֻ��һ����״ case 1-3 ��������״
			{
			case 0:
				tile[i] = allRotationsOshape[0][i];
				break;
			case 1:
				tile[i] = allRotationsIshape[rotation % 2][i];
				break;
			case 2:
				tile[i] = allRotationsSshape[rotation % 2][i];
				break;
			case 3:
				tile[i] = allRotationsZshape[rotation % 2][i];
				break;
			case 4:
				tile[i] = allRotationsLshape[rotation][i];
				break;
			case 5:
				tile[i] = allRotationsJshape[rotation][i];
				break;
			case 6:
				tile[i] = allRotationsTshape[rotation][i];
				break;
			default:
				break;
			}

		updatetile();
	}
}

void heart()
{
	halt = true;
	for(int i = 0; i < 10; ++i) for(int j = 0; j < 20; ++j) changecellcolour(vec2(i, j), black);
	// L
	changecellcolour(vec2(0, 19), green);
	changecellcolour(vec2(0, 18), green);
	changecellcolour(vec2(0, 17), green);
	changecellcolour(vec2(0, 16), green);
	changecellcolour(vec2(1, 16), green);
	changecellcolour(vec2(2, 16), green);
	// C
	changecellcolour(vec2(4, 19), green);
	changecellcolour(vec2(4, 18), green);
	changecellcolour(vec2(4, 17), green);
	changecellcolour(vec2(4, 16), green);
	changecellcolour(vec2(5, 16), green);
	changecellcolour(vec2(5, 19), green);
	// Y
	changecellcolour(vec2(9, 19), green);
	changecellcolour(vec2(9, 18), green);
	changecellcolour(vec2(9, 17), green);
	changecellcolour(vec2(9, 16), green);
	changecellcolour(vec2(8, 16), green);
	changecellcolour(vec2(8, 18), green);
	changecellcolour(vec2(7, 16), green);
	changecellcolour(vec2(7, 18), green);
	changecellcolour(vec2(7, 19), green);

	// Heart
	changecellcolour(vec2(2, 14), red);
	changecellcolour(vec2(3, 14), red);
	changecellcolour(vec2(6, 14), red);
	changecellcolour(vec2(7, 14), red);
	changecellcolour(vec2(1, 13), red);
	changecellcolour(vec2(2, 13), red);
	changecellcolour(vec2(3, 13), red);
	changecellcolour(vec2(4, 13), red);
	changecellcolour(vec2(5, 13), red);
	changecellcolour(vec2(6, 13), red);
	changecellcolour(vec2(7, 13), red);
	changecellcolour(vec2(8, 13), red);
	changecellcolour(vec2(2, 12), red);
	changecellcolour(vec2(3, 12), red);
	changecellcolour(vec2(4, 12), red);
	changecellcolour(vec2(5, 12), red);
	changecellcolour(vec2(6, 12), red);
	changecellcolour(vec2(7, 12), red);
	changecellcolour(vec2(3, 11), red);
	changecellcolour(vec2(4, 11), red);
	changecellcolour(vec2(5, 11), red);
	changecellcolour(vec2(6, 11), red);
	changecellcolour(vec2(4, 10), red);
	changecellcolour(vec2(5, 10), red);

	// P
	changecellcolour(vec2(0, 8), blue);
	changecellcolour(vec2(0, 7), blue);
	changecellcolour(vec2(0, 6), blue);
	changecellcolour(vec2(0, 5), blue);
	changecellcolour(vec2(1, 8), blue);
	changecellcolour(vec2(1, 6), blue);
	changecellcolour(vec2(2, 8), blue);
	changecellcolour(vec2(2, 7), blue);
	changecellcolour(vec2(2, 6), blue);
	// J
	changecellcolour(vec2(5, 8), blue);
	changecellcolour(vec2(5, 6), blue);
	changecellcolour(vec2(5, 5), blue);
	changecellcolour(vec2(4, 5), blue);
	// Q
	changecellcolour(vec2(9, 8), blue);
	changecellcolour(vec2(9, 7), blue);
	changecellcolour(vec2(9, 6), blue);
	changecellcolour(vec2(9, 5), blue);
	changecellcolour(vec2(8, 8), blue);
	changecellcolour(vec2(8, 6), blue);
	changecellcolour(vec2(7, 8), blue);
	changecellcolour(vec2(7, 7), blue);
	changecellcolour(vec2(7, 6), blue);
}

//////////////////////////////////////////////////////////////////////////
// ���row�У��������ã��Ѷȵ�������Ч
void clearrow(int row)
{
	for (int k = row; k < 19; k++)
		for (int i = 0; i < 10; i++)
		{
			// ��������
			board[i][k] = board[i][k + 1];
			// �ı���ɫ
			changecellcolour(vec2(i, k), boardcolours[(int)(6 * (10 * (k + 1) + i))]);
		}
	// ��������
	score++;
	// ��ʾ����
	printf("\r");
	printf("SCORE: %d", score);
	// �߷��ж�
	bestscore = (bestscore < score) ? score : bestscore;
	// д�����ݵ� data.tetris
	write_profile("data.tetris");
	// I love you :)
	if (score == 15) heart();
	// �Ѷȵ���
	FALL_TIME = 1000 - sqrt(score) * 160; // FALL_TIME��һ���ݺ���
	// ��������
	PlaySound("./Audio/clear.wav", NULL, SND_SYNC | SND_FILENAME);
}



//////////////////////////////////////////////////////////////////////////
// ������̸���row����û�б�������������������
void checkfullrow(int row)
{
	int count = 0;
	for (int i = 0; i < 10; i++) // ���
		if (!checkvalid(vec2(i, row)))
			count++;	
	if (count == 10) // ���
	{
		clearrow(row);
		checkfullrow(row); // ���¼�鵱ǰ��
	}
}

//////////////////////////////////////////////////////////////////////////
// ������̸���line����û�б����������������Ϸ�����ж���
void checkfullline(int line)
{
	bool full = false;
		if (!checkvalid(vec2(line, 19)))
			if (!checkvalid(vec2(line, 18)))
				if (!checkvalid(vec2(line, 17)))
					full = true;
	if (full)
	{
		cout << endl << "GAME OVER! YOUR SCORE IS: " << score << ", BEST SCORE IS: " << bestscore << endl; 
		PlaySound("./Audio/gameover.wav", NULL, SND_SYNC | SND_FILENAME);
		halt = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ���飬���Ҹ������̸��Ӧλ�ö������ɫVBO
void settile()
{
	// ÿ������
	for (int i = 0; i < 4; i++)
	{
		// ��ȡ���������̸��ϵ�����
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// �����Ӷ�Ӧ�����̸��ϵ�λ������Ϊ���
		board[x][y] = true;
		// ������Ӧλ�õ���ɫ�޸�
		switch (random)
		{
		case 0:
			changecellcolour(vec2(x, y), orange);
			break;
		case 1:
			changecellcolour(vec2(x, y), red);
			break;
		case 2:
			changecellcolour(vec2(x, y), yellow);
			break;
		case 3:
			changecellcolour(vec2(x, y), cyan);
			break;
		case 4:
			changecellcolour(vec2(x, y), blue);
			break;
		case 5:
			changecellcolour(vec2(x, y), green);
			break;
		case 6:
			changecellcolour(vec2(x, y), magenta);
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < 20; i++)
		checkfullrow(i); // ���ú����Ƿ�����
	for (int i = 0; i < 10; i++)
		if(!halt) // ����Ϸ������
			checkfullline(i); // �����Ϸ�Ƿ����
}

//////////////////////////////////////////////////////////////////////////
// ����λ��(x,y)���ƶ����顣��Ч���ƶ�ֵΪ(-1,0)��(1,0)��(0,-1)���ֱ��Ӧ����
// �����º������ƶ�������ƶ��ɹ�������ֵΪtrue����֮Ϊfalse����
bool movetile(vec2 direction)
{
	// �����ƶ�֮��ķ����λ������
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// ����ƶ�֮�����Ч��
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			// ��Ч���ƶ��÷���
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			updatetile();

			return true;
		}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// ����������Ϸ
void restart()
{
	halt = false;
	init();
}

//////////////////////////////////////////////////////////////////////////
// ��Ϸ��Ⱦ����
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // �������̸� (10*20*2 = 400 ��������)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // ���Ƶ�ǰ���� (8 ��������)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // �������̸����

	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// �ڴ��ڱ������ʱ�򣬿������̸�Ĵ�С��ʹ֮���̶ֹ��ı�����
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧ�¼��е����ⰴ����Ӧ
void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:	// ���ϰ�����ת����
			rotate();
			break;
		case GLUT_KEY_DOWN: // ���°����ƶ�����
			if (!movetile(vec2(0, -1)))
			{
				settile();
				newtile();
			}
			break;
		case GLUT_KEY_LEFT:  // ���󰴼��ƶ�����
			movetile(vec2(-1, 0));
			break;
		case GLUT_KEY_RIGHT: // ���Ұ����ƶ�����
			movetile(vec2(1, 0));
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧʱ���е���ͨ������Ӧ
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 033: // ESC�� �� 'q' ���˳���Ϸ
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' ��������Ϸ
			restart();
			break;
	}
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////
// �Զ�����
void autofall(int x)
{
	if (!movetile(vec2(0, -1)))
	{
		settile();
		newtile();
	}
	glutPostRedisplay();
	glutTimerFunc(FALL_TIME, autofall, 1); // ��������ʵ���Զ�����
}

//////////////////////////////////////////////////////////////////////////
void idle(void)
{
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	srand(time(NULL));
	read_profile("data.tetris");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(520, 92);
	glutCreateWindow("Tetris");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(FALL_TIME, autofall, 1); // �����Զ�����
	glutIdleFunc(idle);
	
	glutMainLoop();
	return 0;
}
