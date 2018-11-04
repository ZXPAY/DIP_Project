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
// #define RUN_MAIN_2
#define GENERATE_DCT_IMG

#define IMG_NAME "lena512.bmp"
#define pi (3.14159265358979323846264338327950288)

int main(){
  printf("========== RUN MAIN CODE ==========\n");
  bmpheader bmp;
  uint8_t *raw_img = imageBmpRead((char *)IMG_NAME, &bmp);
  uint32_t imgSize = getMatrixData(bmp.BitmapDataSize, sizeof(bmp.BitmapDataSize));
  uint16_t imgWidth = getMatrixData(bmp.Width, sizeof(bmp.Width));
  uint16_t imgHeight = getMatrixData(bmp.Height, sizeof(bmp.Height));
  #ifdef SHOW_BMP_HEADER_INFORATION
    showBmpHeaderInformation(&bmp);
    printf("img size is %d\n", imgSize);
    printf("img width is %d\n", imgWidth);
    printf("img height is %d\n", imgHeight);
  #endif
// #################################################################### //
// #################################################################### //
#ifdef RUN_TEST
 testDCT_Transform();
#endif
// #################################################################### //
// #################################################################### //
#ifdef RUN_MAIN_1
  double *img = DCT_transform(raw_img, imgWidth, imgHeight);
  double *inv_img = iDCT_transform(img, imgWidth, imgHeight);
  double *img_substract = (double *)malloc(imgSize*sizeof(double));

#ifdef GENERATE_DCT_IMG
  printf("========== Save bmp file ==========\n");
  printf("DCT : %.2f, %.2f\n", getMaximum(img, imgSize), getMinimum(img, imgSize));
  imageBmpWrite((char *)"HW2-1-1_DCT.bmp", &bmp, imgDouble2Uint8(img, imgSize));

  // setDataNormalization(img, imgSize);
  printf("iDCT : %.2f, %.2f\n", getMaximum(inv_img, imgSize), getMinimum(inv_img, imgSize));
  imageBmpWrite((char *)"HW2-1-2_iDCT.bmp", &bmp, imgDouble2Uint8(inv_img, imgSize));

  for(uint32_t i=0;i<imgSize;i++){
    img_substract[i] =  inv_img[i] - (double)raw_img[i];
  }
  printf("img_substract %.15f, %.15f\n", getMaximum(img_substract, imgSize), getMinimum(img_substract, imgSize));
  setDataNormalization(img_substract, imgSize);
  imageBmpWrite((char *)"HW2-1-3_substract.bmp", &bmp, imgDouble2Uint8(img_substract, imgSize));


#endif
  free(img);
  free(inv_img);
  free(img_substract);
#endif

// #################################################################### //
// #################################################################### //
#ifdef RUN_MAIN_2
  double *imgDown = imgDownSampling(raw_img, imgWidth, imgHeight);
  setMatrixData(bmp.Width, 4, 256);
  setMatrixData(bmp.Height, 4, 256);
  setMatrixData(bmp.FileSize, 4, 256*256+56+256*4);
  setMatrixData(bmp.BitmapDataSize, 4, 256*256);
  showBmpHeaderInformation(&bmp);

  double *imgd = DCT_transform(raw_img, imgWidth, imgHeight);

#ifdef GENERATE_DCT_IMG
  imageBmpWrite((char *)"HW2-2-1_imgDown.bmp", &bmp, imgDouble2Uint8(imgDown, getMatrixData(bmp.BitmapDataSize, 4)));
  imageBmpWrite((char *)"HW2-2-2_FilterMask.bmp", &bmp, imgDouble2Uint8(imgd, getMatrixData(bmp.BitmapDataSize, 4)));
#endif
  free(imgDown);
#endif
  printf("========== END ==========\n");
  return 0;
}
