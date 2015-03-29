#ifndef BMPIO_H_
#define BMPIO_H_

#include "basic_types.h"
#include <cstdio>
#include <cstdlib>

#pragma pack(push)
#pragma pack(1)

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

#pragma pack(pop)

bool InputBmp(int &width, int &height, PixelColor *buffer, const char *file_name);

bool OutputBmp(int width, int height, PixelColor *buffer, const char *file_name);

#endif
