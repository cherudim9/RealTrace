#ifndef _TGA_INPUT_H
#define _TGA_INPUT_H

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "basic_geometry.h"

class TGAFILE{
 public:
  unsigned char imageTypeCode;
  short int imageWidth;
  short int imageHeight;
  unsigned char bitCount;
  unsigned char *imageData;

  PointT GetColor(int i, int j){
    int y=(j*imageWidth+i)*bitCount/8;
    return PointT(imageData[y+2]/256.0, imageData[y+1]/256.0, imageData[y]/256.0);
  }
};

bool LoadTGAFile(char *filename, TGAFILE *tgaFile);

#endif
