#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "lib/Danny_Bmp.h"
#include "lib/Danny_Image_Processing.h"

#define IMAGE_NAME_HW3_1 "volleyball.bmp"
#define IMAGE_NAME_HW3_2 "magicblocks.bmp"

extern int8_t sobelMaskX[3][3];
extern int8_t sobelMaskY[3][3];
extern int8_t gaussianMask[3][3];

uint8_t mMask1[3][3] = {
  {0,1,0},
  {1,1,1},
  {0,1,0},
};
uint8_t mMask2[3][3] = {
  {1,1,1},
  {1,1,1},
  {1,1,1},
};

// #define RUN_TEST
#define RUN_MAIN_1
// #define RUN_MAIN_2
#define MAIN_GENERATE_IMAGE
// #define RUN_TEST_CONNECTED

int main(){

  bmpheader bmp;
  #ifdef RUN_MAIN_1
    uint8_t **raw_img = imageBmpReadBGR((char *)IMAGE_NAME_HW3_1, &bmp);
  #endif
  #ifdef RUN_MAIN_2
    uint8_t **raw_img = imageBmpReadBGR((char *)IMAGE_NAME_HW3_2, &bmp);
  #endif
  showBmpHeaderInformation(&bmp);
  uint16_t imgW = getMatrixData(bmp.Width, 4);
  uint16_t imgH = getMatrixData(bmp.Height, 4);
  uint32_t imgSize = imgW*imgH;
  double *imgB = imgUint82Double(raw_img[0], imgSize);
  double *imgG = imgUint82Double(raw_img[1], imgSize);
  double *imgR = imgUint82Double(raw_img[2], imgSize);
  free(raw_img);

#ifdef RUN_TEST
  printf("\n===== Test =====\n");
  uint8_t **saveImg = BGR2OneSave(imgB, imgG, imgR, imgSize);
  imageBmpWriteBGR((char *)"testSave.bmp", &bmp, saveImg);
  double *testGrayImg = BGR2Gray(imgB, imgG, imgR, imgSize);
  showBmpHeaderInformation(&bmp);
  imageBmpWrite((char *)"testGray.bmp", &bmp, imgDouble2Uint8(testGrayImg, imgSize));
  double *imgG_SobleX = maskFilter(testGrayImg, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgG_SobleY = maskFilter(testGrayImg, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgG_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgG_SobleY, imgSize, IMAGE_ABS, 0);
  imageBmpWrite((char *)"ximgG.bmp", &bmp, imgDouble2Uint8(testGrayImg, imgSize));
  imageBmpWrite((char *)"ximgG_SobleX.bmp", &bmp, imgDouble2Uint8(imgG_SobleX, imgSize));
  imageBmpWrite((char *)"ximgG_SobleY.bmp", &bmp, imgDouble2Uint8(imgG_SobleY, imgSize));
  double *sumImgG = twoImgOperation(imgG_SobleX, imgG_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgG, imgSize, IMAGE_ABS, 0);
  sumImgG = medianFilter(sumImgG, imgW, imgH, 3);
  imageBmpWrite((char *)"ximgG_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgG, imgSize));

#endif


#ifdef RUN_MAIN_1
  printf("\n===== Main 1 =====\n");
  // double *grayImg = BGR2Gray(imgB, imgG, imgR, imgSize);

  // for(uint32_t i=0;i<imgSize;i++) imgB[i] = (imgB[i] < 50)*255.0;
  double *imgB_SobleX = maskFilter(imgB, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgB_SobleY = maskFilter(imgB, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgB_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgB_SobleY, imgSize, IMAGE_ABS, 0);
  double *sumImgB = twoImgOperation(imgB_SobleX, imgB_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgB, imgSize, IMAGE_ABS, 0);
  sumImgB = medianFilter(sumImgB, imgW, imgH, 3);
  #ifdef MAIN_GENERATE_IMAGE
    imageBmpWrite((char *)"imgB.bmp", &bmp, imgDouble2Uint8(imgB, imgSize));
    imageBmpWrite((char *)"imgB_SobleX.bmp", &bmp, imgDouble2Uint8(imgB_SobleX, imgSize));
    imageBmpWrite((char *)"imgB_SobleY.bmp", &bmp, imgDouble2Uint8(imgB_SobleY, imgSize));
    imageBmpWrite((char *)"imgB_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgB, imgSize));
  #endif
  free(imgB);
  free(imgB_SobleX);
  free(imgB_SobleY);

  double *imgG_SobleX = maskFilter(imgG, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgG_SobleY = maskFilter(imgG, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgG_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgG_SobleY, imgSize, IMAGE_ABS, 0);
  double *sumImgG = twoImgOperation(imgG_SobleX, imgG_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgG, imgSize, IMAGE_ABS, 0);
  sumImgG = medianFilter(sumImgG, imgW, imgH, 3);
  #ifdef MAIN_GENERATE_IMAGE
    imageBmpWrite((char *)"imgG.bmp", &bmp, imgDouble2Uint8(imgG, imgSize));
    imageBmpWrite((char *)"imgG_SobleX.bmp", &bmp, imgDouble2Uint8(imgG_SobleX, imgSize));
    imageBmpWrite((char *)"imgG_SobleY.bmp", &bmp, imgDouble2Uint8(imgG_SobleY, imgSize));
    imageBmpWrite((char *)"imgG_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgG, imgSize));
  #endif
  free(imgG);
  free(imgG_SobleX);
  free(imgG_SobleY);

  double *imgR_SobleX = maskFilter(imgR, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgR_SobleY = maskFilter(imgR, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgR_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgR_SobleY, imgSize, IMAGE_ABS, 0);
  double *sumImgR = twoImgOperation(imgR_SobleX, imgR_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgR, imgSize, IMAGE_ABS, 0);
  sumImgR = medianFilter(sumImgR, imgW, imgH, 3);
  #ifdef MAIN_GENERATE_IMAGE
    imageBmpWrite((char *)"imgR.bmp", &bmp, imgDouble2Uint8(imgR, imgSize));
    imageBmpWrite((char *)"imgR_SobleX.bmp", &bmp, imgDouble2Uint8(imgR_SobleX, imgSize));
    imageBmpWrite((char *)"imgR_SobleY.bmp", &bmp, imgDouble2Uint8(imgR_SobleY, imgSize));
    imageBmpWrite((char *)"imgR_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgR, imgSize));
  #endif
  free(imgR);
  free(imgR_SobleX);
  free(imgR_SobleY);

  double *allImg = twoImgOperation(sumImgB, sumImgG, imgSize, IMAGE_PLUS);
  allImg = twoImgOperation(allImg, sumImgR, imgSize, IMAGE_PLUS);
  imgOperation(allImg, imgSize, IMAGE_ABS, 0);
  free(sumImgB);
  free(sumImgG);
  free(sumImgR);

  //imageBmpWrite((char *)"allImg.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  for(uint32_t i=0;i<imgSize;i++) allImg[i] = (allImg[i] > 100)*255.0;
  imageBmpWrite((char *)"preHW3-1.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  allImg = medianFilter(allImg, imgW, imgH, 3);
  allImg = medianFilter(allImg, imgW, imgH, 5);
  allImg = medianFilter(allImg, imgW, imgH, 7);
  allImg = maskFilter(allImg, imgW, imgH, &gaussianMask[0][0], 3, 1);
  imgOperation(allImg, imgSize, IMAGE_DIVIDE, 16.0);
  allImg = erosion(allImg, imgW, imgH, &mMask1[0][0], 3, 3);
  allImg = dilation(allImg, imgW, imgH, &mMask2[0][0], 3, 3);
  allImg = erosion(allImg, imgW, imgH, &mMask1[0][0], 3, 3);
  allImg = erosion(allImg, imgW, imgH, &mMask1[0][0], 3, 3);

  imageBmpWrite((char *)"HW3-1-1.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  printf("Image Max value is %.0f\n", getMaximum(allImg, imgSize));
  printf("Image Min value is %.0f\n", getMinimum(allImg, imgSize));

  for(uint32_t i=0;i<imgSize;i++){
    allImg[i] = 255 - allImg[i];
  }
  // imageBmpWrite((char *)"test.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  double *marker = connectedComponentLabling(allImg, imgW, imgH, 0);
  // imageBmpWrite((char *)"mark.bmp", &bmp, imgDouble2Uint8(marker, imgSize));

  // uint32_t *gray = getGrayLevelHistogramData(marker, imgW, imgH);
  printf("marker Max value is %.0f\n", getMaximum(marker, imgSize));
  printf("marker Min value is %.0f\n", getMinimum(marker, imgSize));
  // for(uint16_t i=0;i<256;i++){
  //   printf("%d, %d\n", i, gray[i]);
  // }
  // imageBmpWrite((char *)"test2.bmp", &bmp, imgDouble2Uint8(marker, imgSize));
  // free(gray);
  free(allImg);

  double *ansB = (double *)calloc(imgSize, sizeof(double));
  double *ansG = (double *)calloc(imgSize, sizeof(double));
  double *ansR = (double *)calloc(imgSize, sizeof(double));
  uint16_t b_cnt = 0;
  uint16_t g_cnt = 0;
  uint16_t r_cnt = 1;
  uint16_t maxData = (uint16_t)getMaximum(marker, imgSize);
  for(uint8_t p=1;p<=maxData;p++){
    for(uint32_t m=0;m<imgSize;m++){
      if(marker[m] == p){
        ansB[m] = 61.0*b_cnt;
        ansG[m] = 61.0*g_cnt;
        ansR[m] = 61.0*r_cnt;
      }
    }
    if(r_cnt != 4){
      r_cnt++;
    }
    else if(b_cnt != 4){
      b_cnt++;
      r_cnt = 0;
    }
    else if(g_cnt != 4){
      g_cnt++;
      r_cnt = 0;
      b_cnt = 0;
    }
    // printf("%d,%d,%d,%d\n", r_cnt, b_cnt, g_cnt, p);
  }

  free(marker);
  uint8_t **final = BGR2OneSave(ansB, ansG, ansR, imgSize);
  free(ansB);
  free(ansG);
  free(ansR);
  imageBmpWriteBGR((char *)"HW3-1-2.bmp", &bmp, final);

#endif

// ######################################################### //
// ######################################################### //

#ifdef RUN_MAIN_2
  printf("\n===== Main 2 =====\n");

  double *imgB_SobleX = maskFilter(imgB, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgB_SobleY = maskFilter(imgB, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgB_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgB_SobleY, imgSize, IMAGE_ABS, 0);
  double *sumImgB = twoImgOperation(imgB_SobleX, imgB_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgB, imgSize, IMAGE_ABS, 0);
  sumImgB = medianFilter(sumImgB, imgW, imgH, 3);
  #ifdef MAIN_GENERATE_IMAGE
    imageBmpWrite((char *)"imgB.bmp", &bmp, imgDouble2Uint8(imgB, imgSize));
    imageBmpWrite((char *)"imgB_SobleX.bmp", &bmp, imgDouble2Uint8(imgB_SobleX, imgSize));
    imageBmpWrite((char *)"imgB_SobleY.bmp", &bmp, imgDouble2Uint8(imgB_SobleY, imgSize));
    imageBmpWrite((char *)"imgB_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgB, imgSize));
  #endif
  free(imgB);
  free(imgB_SobleX);
  free(imgB_SobleY);

  double *imgG_SobleX = maskFilter(imgG, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgG_SobleY = maskFilter(imgG, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgG_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgG_SobleY, imgSize, IMAGE_ABS, 0);
  double *sumImgG = twoImgOperation(imgG_SobleX, imgG_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgG, imgSize, IMAGE_ABS, 0);
  sumImgG = medianFilter(sumImgG, imgW, imgH, 3);
  #ifdef MAIN_GENERATE_IMAGE
    imageBmpWrite((char *)"imgG.bmp", &bmp, imgDouble2Uint8(imgG, imgSize));
    imageBmpWrite((char *)"imgG_SobleX.bmp", &bmp, imgDouble2Uint8(imgG_SobleX, imgSize));
    imageBmpWrite((char *)"imgG_SobleY.bmp", &bmp, imgDouble2Uint8(imgG_SobleY, imgSize));
    imageBmpWrite((char *)"imgG_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgG, imgSize));
  #endif
  free(imgG);
  free(imgG_SobleX);
  free(imgG_SobleY);

  double *imgR_SobleX = maskFilter(imgR, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgR_SobleY = maskFilter(imgR, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgR_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgR_SobleY, imgSize, IMAGE_ABS, 0);
  double *sumImgR = twoImgOperation(imgR_SobleX, imgR_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgR, imgSize, IMAGE_ABS, 0);
  sumImgR = medianFilter(sumImgR, imgW, imgH, 3);
  #ifdef MAIN_GENERATE_IMAGE
    imageBmpWrite((char *)"imgR.bmp", &bmp, imgDouble2Uint8(imgR, imgSize));
    imageBmpWrite((char *)"imgR_SobleX.bmp", &bmp, imgDouble2Uint8(imgR_SobleX, imgSize));
    imageBmpWrite((char *)"imgR_SobleY.bmp", &bmp, imgDouble2Uint8(imgR_SobleY, imgSize));
    imageBmpWrite((char *)"imgR_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgR, imgSize));
  #endif
  free(imgR);
  free(imgR_SobleX);
  free(imgR_SobleY);

  double *allImg = twoImgOperation(sumImgB, sumImgG, imgSize, IMAGE_PLUS);
  allImg = twoImgOperation(allImg, sumImgR, imgSize, IMAGE_PLUS);
  imgOperation(allImg, imgSize, IMAGE_ABS, 0);
  free(sumImgB);
  free(sumImgG);
  free(sumImgR);

  //imageBmpWrite((char *)"allImg.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  for(uint32_t i=0;i<imgSize;i++) allImg[i] = (allImg[i] > 100)*255.0;
  imageBmpWrite((char *)"preHW3-2.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  allImg = medianFilter(allImg, imgW, imgH, 7);
  allImg = medianFilter(allImg, imgW, imgH, 5);
  allImg = medianFilter(allImg, imgW, imgH, 3);
  allImg = medianFilter(allImg, imgW, imgH, 3);
  allImg = maskFilter(allImg, imgW, imgH, &gaussianMask[0][0], 3, 1);
  imgOperation(allImg, imgSize, IMAGE_DIVIDE, 16.0);
  allImg = erosion(allImg, imgW, imgH, &mMask2[0][0], 3, 3);
  allImg = dilation(allImg, imgW, imgH, &mMask1[0][0], 3, 3);
  allImg = erosion(allImg, imgW, imgH, &mMask2[0][0], 3, 3);

  imageBmpWrite((char *)"HW3-2-1.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  printf("Image Max value is %.0f\n", getMaximum(allImg, imgSize));
  printf("Image Min value is %.0f\n", getMinimum(allImg, imgSize));

  for(uint32_t i=0;i<imgSize;i++){
    allImg[i] = 255 - allImg[i];
  }

  // imageBmpWrite((char *)"test.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  double *marker = connectedComponentLabling(allImg, imgW, imgH, 0);
  // imageBmpWrite((char *)"mark.bmp", &bmp, imgDouble2Uint8(marker, imgSize));

  // uint32_t *gray = getGrayLevelHistogramData(marker, imgW, imgH);
  printf("marker Max value is %.0f\n", getMaximum(marker, imgSize));
  printf("marker Min value is %.0f\n", getMinimum(marker, imgSize));
  // for(uint16_t i=0;i<256;i++){
  //   printf("%d, %d\n", i, gray[i]);
  // }
  // imageBmpWrite((char *)"test2.bmp", &bmp, imgDouble2Uint8(marker, imgSize));
  // free(gray);
  free(allImg);

  double *ansB = (double *)calloc(imgSize, sizeof(double));
  double *ansG = (double *)calloc(imgSize, sizeof(double));
  double *ansR = (double *)calloc(imgSize, sizeof(double));
  uint16_t b_cnt = 0;
  uint16_t g_cnt = 0;
  uint16_t r_cnt = 1;
  uint16_t maxData = (uint16_t)getMaximum(marker, imgSize);
  for(uint8_t p=1;p<=maxData;p++){
    for(uint32_t m=0;m<imgSize;m++){
      if(marker[m] == p){
        ansB[m] = 61.0*b_cnt;
        ansG[m] = 61.0*g_cnt;
        ansR[m] = 61.0*r_cnt;
      }
    }
    if(r_cnt != 4){
      r_cnt++;
    }
    else if(b_cnt != 4){
      b_cnt++;
      r_cnt = 0;
    }
    else if(g_cnt != 4){
      g_cnt++;
      r_cnt = 0;
      b_cnt = 0;
    }
    // printf("%d,%d,%d,%d\n", r_cnt, b_cnt, g_cnt, p);
  }

  free(marker);
  uint8_t **final = BGR2OneSave(ansB, ansG, ansR, imgSize);
  free(ansB);
  free(ansG);
  free(ansR);
  imageBmpWriteBGR((char *)"HW3-2-2.bmp", &bmp, final);

#endif


// ######################################################### //
// ######################################################### //


#ifdef RUN_TEST_CONNECTED

double testMat[8][8] = {
  {0,0,0,0,0,0,0,0},
  {0,0,1,0,0,1,0,0},
  {0,0,1,1,0,1,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,1,0,0,0,1,0},
  {0,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};
double *ptr = &testMat[0][0];

imgW = 8;
imgH = 8;
imgSize = 64;

double *markertest = connectedComponentLabling(ptr, 8, 8, 0);
printf("efefef\n");
double a = 0;
for(uint32_t i=0;i<imgSize;i++){
  if(markertest[i]>a){
    a = markertest[i];
  }
  printf("%.0f\n", markertest[i]);
}
printf("a is %.0f\n", a);

for(uint16_t i=0;i<8;i++){
  for(uint16_t j=0;j<8;j++){
    printf("%.0f\t", markertest[i*8+j]);
  }
  printf("\n");
}



#endif
  printf("\n===== END =====\n");
  return 0;
}
