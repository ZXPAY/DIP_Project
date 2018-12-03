#ifndef DANNY_IMAGE_PROCESSING_H
#define  DANNY_IMAGE_PROCESSING_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include "Danny_Bmp.h"

#ifndef pi
#define pi (3.14159265358979323846264338327950288)
#endif

/*
    --------> y ,w
    |
    |
    |
    |
    ˇ
    x, h
*/

// #define SHOW_MEDIAN_FILTER_DEBUG
// #define SHOW_PADDING_DEBUG
// #define SHOW_MASK_FILTER_DEBUG

#define IMAGE_PLUS 1
#define IMAGE_MINUS 2
#define IMAGE_MULTIFY 3
#define IMAGE_DIVIDE 4
#define IMAGE_ABS 5

// DCT (Discrete Cosine Transform)
// DCT (Discrete Cosine Transform) unit, do one time (8X8)
// Quantization, filtermast, one of JPEG format step
// i means inverse
double *DCT_transform(uint8_t *originImage, uint16_t imgW, uint16_t imgH);
void DCT_transformUnit(uint8_t *originImage, double *img, uint16_t imgW, uint16_t startX, uint16_t startY);
void Quantization_transform(double *img, uint16_t imgW, uint16_t imgH);
double *iDCT_transform(double *img, uint16_t imgW, uint16_t imgH);
void iDCT_transformUnit(double *img, double *inv_img, uint16_t imgW, uint16_t startX, uint16_t startY);
void iQuantization_transform(double *img, uint16_t imgW, uint16_t imgH);
void filterMask_transform(double *img, uint16_t imgW, uint16_t imgH);

// image data type (double to uint8) or (uint8 to double)
uint8_t *imgDouble2Uint8(double *img, uint32_t imgSize);
double *imgUint82Double(uint8_t *img, uint32_t imgSize);

// Do all image pixel +-*/ by the same value
void imgOperation(double *img, uint32_t imgSize, uint16_t operation, double value);
double *twoImgOperation(double *img1, double *img2, uint32_t imgSize, uint16_t operation);

// Image Down Sampling, by odd number pixel
double *imgDownSampling(uint8_t *originImage, uint16_t imgW, uint16_t imgH);
double *imgDownSamplingMean(uint8_t *originImage, uint16_t imgW, uint16_t imgH);



// Calculate two image MSE (Mean Square Error)
double getImageMSE(uint8_t *originImage, double *transferedImage, uint32_t imgSize);

// Calculate two image PSNR (Peak of Signal and Noise Ratio)
double getImagePSNR(uint8_t *originImage, double *transferedImage, uint32_t imgSize);

// Use to sort the matrix value
double *sortedMatrix(double *mat, uint32_t imgSize, bool descentOrNot);

// return image padding, return size is (imgW+2, imgH+2)
// All zeros around the image matrix
double *getPaddingImage(double *img, uint16_t imgW, uint16_t imgH, uint16_t maskSide);

// return median value, median mask size is (maskSide X maskSide), square_side is image width
double getMedianNumber(double *mat, uint16_t posx, uint16_t posy, uint16_t maskSide, uint16_t square_side);

// return image that do median filter
double* medianFilter(double *img, uint16_t imgW, uint16_t imgH, uint16_t maskSide);

// return image that do mask filter in spatial domain
double* maskFilter(double *img, uint16_t imgW, uint16_t imgH, int8_t *mask, uint8_t maskSide, uint8_t stride);

// Morphologic method, No padding
double* dilation(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH);
double* erosion(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH);
double* opening(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH);
double* closing(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH);

// Call by address, exchange two value
void swap(double *value1, double *value2);

// BGR Processing
uint8_t** BGR2OneSave(double *imgB, double *imgG, double *imgR, uint32_t imgSize);
double* BGR2Gray(double *imgB, double *imgG, double *imgR, uint32_t imgSize);

#endif
