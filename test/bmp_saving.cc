#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

#pragma pack(push)
#pragma pack(1)

typedef unsigned char BYTE;//8 bits
typedef unsigned short WORD;//16 bits
typedef unsigned int DWORD;//32 bits

typedef struct {
  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} myBMPFILEHEADER;

typedef struct{
  DWORD      biSize;
  DWORD       biWidth;
  DWORD       biHeight;
  WORD       biPlanes;
  WORD       biBitCount;
  DWORD      biCompression;
  DWORD      biSizeImage;
  DWORD       biXPelsPerMeter;
  DWORD       biYPelsPerMeter;
  DWORD      biClrUsed;
  DWORD      biClrImportant;
} myBMPINFOHEADER;

bool OutputBmp(int width, int height, const char * file_name){
  int size=width*height*3;

  myBMPFILEHEADER fh;
  fh.bfType=0x4D42;
  fh.bfSize=sizeof(myBMPFILEHEADER)+sizeof(myBMPINFOHEADER)+size;
  fh.bfReserved1=fh.bfReserved2=0;
  fh.bfOffBits=fh.bfSize-size;
  
  myBMPINFOHEADER ih;
  ih.biSize=sizeof(myBMPINFOHEADER);
  ih.biWidth=width;
  ih.biHeight=height;
  ih.biPlanes=1;
  ih.biBitCount=24;
  ih.biCompression=0;
  ih.biSizeImage=0;
  ih.biXPelsPerMeter=0;
  ih.biYPelsPerMeter=0;
  ih.biClrUsed=0;
  ih.biClrImportant=0;

  BYTE *data=new BYTE[size+10];

  for(int i=0; i<size; i++)
    data[i]=static_cast<BYTE>(rand() & ((1<<8)-1));
  
  FILE *bmp_file=fopen(file_name, "wb");
  fwrite(&fh, sizeof fh, 1, bmp_file);
  fwrite(&ih, sizeof ih, 1, bmp_file);
  fwrite(data, size, 1, bmp_file);
  fclose(bmp_file);

  return 1;
}

#pragma(pop)


typedef struct {
  WORD    bfType;
  DWORD   bfSize;
  WORD    bfReserved1;
  WORD    bfReserved2;
  DWORD   bfOffBits;
} anotherBMPFILEHEADER;

int main(){

  printf("%d %d %d\n",sizeof(unsigned short), sizeof(unsigned long), sizeof(unsigned int));
  printf("%d %d\n", sizeof(myBMPINFOHEADER), sizeof(myBMPFILEHEADER));
  printf("%d\n", sizeof(anotherBMPFILEHEADER));
  
  OutputBmp(1000,1000,"test.bmp");

  return 0;
}
