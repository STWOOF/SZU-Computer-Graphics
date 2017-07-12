#define _CRT_SECURE_NO_WARNINGS
#include "Angel.h"
#include "bmp_loader.h"
using namespace std;

static unsigned int getint(FILE *fp){ // ��ȡ4bit����
  int c, c1, c2, c3;
  c = getc(fp);  
  c1 = getc(fp); 
  c2 = getc(fp);  
  c3 = getc(fp);
  return ((unsigned int) c) +  (((unsigned int) c1) << 8) + (((unsigned int) c2) << 16) + (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp) // ��ȡ2bit����
{
  int c, c1;
  c = getc(fp);  
  c1 = getc(fp);
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
} 

// ����bmpͼƬ
int ImageLoad(char *filename, Image *image) {
  FILE *file;
  int size; // ͼƬ��С
  unsigned int planes;
  unsigned int bpp; // ÿ����24bits
  char temp;
  
  // �����ļ�
  if ((file = fopen(filename, "rb")) == NULL) {
    cout << "File Not Found : " << filename << endl;
    return 0;
  }
  
  // ����ͼƬ���ص�����
  fseek(file, 18, SEEK_CUR);
  image->sizeX = getint (file);
  image->sizeY = getint (file);
  size = image->sizeX * image->sizeY * 3;
  planes = getshort(file);
  bpp = getshort(file);

  // ��ȡͼƬ����
  fseek(file, 24, SEEK_CUR);
  image->data = (char *) malloc(size);
  if (image->data == NULL) {
    cout << "Error allocating memory for color-corrected image data" << endl;
    return 0; 
  }
  if (fread(image->data, size, 1, file) != 1) {
    cout << "Error reading image data from " << filename << endl;
    return 0;
  }
  for (int i = 0; i < size; i += 3) { // ��ԭ�е�bgr��ɫתΪrgb��ʽ
    temp = image->data[i];
    image->data[i] = image->data[i+2];
    image->data[i+2] = temp;
  }
  return 1;
}

// ��ȡͼƬ���󶨵�OpenGL
void LoadGLTextures(char *filename,GLuint *texture_num) { 
  Image image;
  if (!ImageLoad(filename, &image)) {
    exit(1);
  }          

  // ��������
  glGenTextures(3, texture_num);
  glBindTexture(GL_TEXTURE_2D, *texture_num);
  
  // �������
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
  
  // ������  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, image.sizeX, image.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);

}


