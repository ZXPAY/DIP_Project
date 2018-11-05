#include <stdlib.h>
#include <stdio.h>
#include "lib/Danny_Bmp.h"
#include <math.h>
#include <inttypes.h>
#include "testDCT.h"
#include "lib/Danny_Image_Processing.h"

/*
  u
  ^
  |
  |
  |
  |------------> v

*/

// #define SHOW_BMP_HEADER_INFORATION
// #define RUN_TEST
#define RUN_MAIN_1
#define RUN_MAIN_2
#define GENERATE_DCT_IMG

#define IMG_NAME "lena512.bmp"
#define pi (3.14159265358979323846264338327950288)

int main(){
  bmpheader bmp;
  uint8_t *raw_img = imageBmpRead((char *)IMG_NAME, &bmp);
  uint32_t imgSize = getMatrixData(bmp.BitmapDataSize, sizeof(bmp.BitmapDataSize));
  uint16_t imgWidth = getMatrixData(bmp.Width, sizeof(bmp.Width));
  uint16_t imgHeight = getMatrixData(bmp.Height, sizeof(bmp.Height));
  #ifdef SHOW_BMP_HEADER_INFORATION
    printf("========== Raw image information ==========\n");
    showBmpHeaderInformation(&bmp);
    printf("img size is %d\n", imgSize);
    printf("img width is %d\n", imgWidth);
    printf("img height is %d\n", imgHeight);
    printf("\n\n");
  #endif
// #################################################################### //
// #################################################################### //
#ifdef RUN_TEST
 printf("========== RUN TEST CODE ==========\n");
 testDCT_Transform();
#endif
// #################################################################### //
// #################################################################### //
#ifdef RUN_MAIN_1
  printf("\n========== RUN MAIN_1 CODE ==========\n");
  double *img = DCT_transform(raw_img, imgWidth, imgHeight);
  Quantization_transform(img, imgWidth, imgHeight);

#ifdef GENERATE_DCT_IMG
  printf("========== Save image *.bmp ==========\n");
  // printf("DCT : %.2f, %.2f\n", getMaximum(img, imgSize), getMinimum(img, imgSize));
  imageBmpWrite((char *)"HW2-1-1_DCT.bmp", &bmp, imgDouble2Uint8(img, imgSize));
#endif

// #### iQuantization #### //
  iQuantization_transform(img, imgWidth, imgHeight);
  double *inv_img = iDCT_transform(img, imgWidth, imgHeight);
  // setDataNormalization(img, imgSize);
#ifdef GENERATE_DCT_IMG
  // printf("iDCT : %.2f, %.2f\n", getMaximum(inv_img, imgSize), getMinimum(inv_img, imgSize));
  imageBmpWrite((char *)"HW2-1-2_iDCT.bmp", &bmp, imgDouble2Uint8(inv_img, imgSize));
#endif

  printf("2-1-2 MSE  : %.30f\n", getImageMSE(raw_img, inv_img, imgSize));
  printf("2-1-2 PSNR : %.2f dB\n", getImagePSNR(raw_img, inv_img, imgSize));

// #### Image subsract #### //
  double *img_substract = (double *)malloc(imgSize*sizeof(double));
  for(uint32_t i=0;i<imgSize;i++){
    img_substract[i] =  inv_img[i] - (double)raw_img[i];
  }
  printf("img_substract %.15f, %.15f\n", getMaximum(img_substract, imgSize), getMinimum(img_substract, imgSize));
  setDataNormalization(img_substract, imgSize);
  printf("2-1-3 MSE  : %.2f\n", getImageMSE(raw_img, img_substract, imgSize));
  printf("2-1-3 PSNR : %.2f dB\n", getImagePSNR(raw_img, img_substract, imgSize));
#ifdef GENERATE_DCT_IMG
  imageBmpWrite((char *)"HW2-1-3_substract.bmp", &bmp, imgDouble2Uint8(img_substract, imgSize));
#endif

  free(img);
  free(inv_img);
  free(img_substract);
#endif

// #################################################################### //
// #################################################################### //
#ifdef RUN_MAIN_2
  printf("\n========== RUN MAIN_2 CODE ==========\n");
  double *imgDown = imgDownSampling(raw_img, imgWidth, imgHeight);
  setMatrixData(bmp.Width, 4, 256);
  setMatrixData(bmp.Height, 4, 256);
  setMatrixData(bmp.FileSize, 4, 256*256+56+256*4);
  setMatrixData(bmp.BitmapDataSize, 4, 256*256);
  imgSize = getMatrixData(bmp.BitmapDataSize, 4);
  imgWidth = getMatrixData(bmp.Width, 4);
  imgHeight = getMatrixData(bmp.Height, 4);
  #ifdef SHOW_BMP_HEADER_INFORATION
    printf("========== DownSampling image information ==========\n");
    showBmpHeaderInformation(&bmp);
    printf("img size is %d\n", imgSize);
    printf("img width is %d\n", imgWidth);
    printf("img height is %d\n", imgHeight);
    printf("\n\n");
  #endif

  uint8_t *raw_imgd = imgDouble2Uint8(imgDown, imgSize);

  double *imgd = DCT_transform(raw_imgd, imgWidth, imgHeight);
  filterMask_transform(imgd, imgWidth, imgHeight);
  double *inv_imgd = iDCT_transform(imgd, imgWidth, imgHeight);

  printf("2-1-3 MSE  : %.2f\n", getImageMSE(raw_imgd, inv_imgd, imgSize));
  printf("2-1-3 PSNR : %.2f dB\n", getImagePSNR(raw_imgd, inv_imgd, imgSize));

  double *imgd_substract = (double *)malloc(imgSize*sizeof(double));
  for(uint32_t i=0;i<imgSize;i++){
    imgd_substract[i] = inv_imgd[i] - imgDown[i];
  }

  printf("imgd_substract %.15f, %.15f\n", getMaximum(imgd_substract, imgSize), getMinimum(imgd_substract, imgSize));
  printf("2-1-4 MSE  : %.2f\n", getImageMSE(raw_imgd, imgd_substract, imgSize));
  printf("2-1-4 PSNR : %.2f dB\n", getImagePSNR(raw_imgd, imgd_substract, imgSize));

#ifdef GENERATE_DCT_IMG
  printf("========== Save image *.bmp ==========\n");
  imageBmpWrite((char *)"HW2-2-1_imgDown.bmp", &bmp, imgDouble2Uint8(imgDown, imgSize));
  imageBmpWrite((char *)"HW2-2-2_FilterMask.bmp", &bmp, imgDouble2Uint8(imgd, imgSize));
  imageBmpWrite((char *)"HW2-2-3_FilterMask_IDCT.bmp", &bmp, imgDouble2Uint8(inv_imgd, imgSize));
  setDataNormalization(imgd_substract, imgSize);
  imageBmpWrite((char *)"HW2-2-4_FilterMask_Substract.bmp", &bmp, imgDouble2Uint8(imgd_substract, imgSize));
#endif
  free(imgDown);
  free(imgd);
  free(inv_imgd);
#endif
  printf("========== END ==========\n");
  return 0;
}
