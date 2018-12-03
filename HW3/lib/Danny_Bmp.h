#ifndef DANNY_BMP_H
#define DANNY_BMP_H
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "Danny_Image_Processing.h"

#define BMP_HEADRER_SIZE 54
#define BMP_PLATTE_SIZE 256*4

#pragma pack(1)
typedef struct __attribute__((packed)) BMP_HEADER{
  uint8_t Identifier[2];
  uint8_t FileSize[4];
  uint8_t Reserve[4];
  uint8_t BitmapDataOffset[4];
  uint8_t BitmapHeaderSize[4];
  uint8_t Width[4];
  uint8_t Height[4];
  uint8_t Planes[2];
  uint8_t BitsPerPixel[2];
  uint8_t Compression[4];
  uint8_t BitmapDataSize[4];
  uint8_t H_Rosolution[4];
  uint8_t U_Rosolution[4];
  uint8_t UsedColorsSize[4];
  uint8_t ImportantColors[4];
  uint8_t Plalette[BMP_PLATTE_SIZE];
}bmpheader;

uint8_t* imageBmpRead(char *fileName, bmpheader *bmp);
void imageBmpWrite(char *fileName, bmpheader *bmp, uint8_t* img_data);
uint8_t** imageBmpReadBGR(char *fileName, bmpheader *bmp);
void imageBmpWriteBGR(char *fileName, bmpheader *bmp, uint8_t** img_data);
uint32_t getMatrixData(uint8_t* mat, uint8_t mat_size);
void setMatrixData(uint8_t *mat, uint8_t mat_size, uint32_t data);
void showBmpHeaderInformation(bmpheader *bmp);
void setGrayLevelHistogramData(bmpheader* bmp, uint8_t* img_array, uint32_t* GrayLevelData);

// Normalization
double getMinimum(double *mptr, uint32_t dataSize);
double getMaximum(double *mptr, uint32_t dataSize);
void setDataNormalization(double *ptr, uint32_t dataSize);
void testNormalization();

#endif
