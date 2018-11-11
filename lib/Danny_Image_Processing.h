#ifndef DANNY_IMAGE_PROCESSING_H
#define  DANNY_IMAGE_PROCESSING_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "Danny_Bmp.h"

#ifndef pi
#define pi (3.14159265358979323846264338327950288)
#endif

#define IMAGE_PLUS 1
#define IMAGE_MINUS 2
#define IMAGE_MULTIFY 3
#define IMAGE_DIVIDE 4

double *DCT_transform(uint8_t *originImage, uint16_t imgW, uint16_t imgH);
void DCT_transformUnit(uint8_t *originImage, double *img, uint16_t imgW, uint16_t startX, uint16_t startY);
void Quantization_transform(double *img, uint16_t imgW, uint16_t imgH);
double *iDCT_transform(double *img, uint16_t imgW, uint16_t imgH);
void iDCT_transformUnit(double *img, double *inv_img, uint16_t imgW, uint16_t startX, uint16_t startY);
void iQuantization_transform(double *img, uint16_t imgW, uint16_t imgH);
uint8_t *imgDouble2Uint8(double *img, uint32_t imgSize);
double *imgUint82Double(uint8_t *img, uint32_t imgSize);

void imgOperation(double *img, uint32_t imgSize, uint16_t operation, double value);

double *imgDownSampling(uint8_t *originImage, uint16_t imgW, uint16_t imgH);
double *imgDownSamplingMean(uint8_t *originImage, uint16_t imgW, uint16_t imgH);
void filterMask_transform(double *img, uint16_t imgW, uint16_t imgH);

double getImageMSE(uint8_t *originImage, double *transferedImage, uint32_t imgSize);
double getImagePSNR(uint8_t *originImage, double *transferedImage, uint32_t imgSize);

#endif
