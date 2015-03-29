#include "../include/bmpio.h"

bool InputBmp(int &width, int &height, PixelColor *buffer, const char *file_name){
  //to be constructed...
  return 0;
}

bool OutputBmp(int width, int height, PixelColor *buffer, const char *file_name){
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
  FILE *bmp_file=fopen(file_name, "wb");
  if (!bmp_file){
    fclose(bmp_file);
    return 0;
  }
  if (!fwrite(&fh, sizeof fh, 1, bmp_file)){
    return 0;
  }
  if (!fwrite(&ih, sizeof ih, 1, bmp_file)){
    return 0;
  }
  if (!fwrite(buffer, size, 1, bmp_file)){
    return 0;
  }
  fclose(bmp_file);
  return 1;
}
