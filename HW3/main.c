#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "lib/Danny_Bmp.h"
#include "lib/Danny_Image_Processing.h"

#define IMAGE_NAME "volleyball.bmp"

extern int8_t sobelMaskX[3][3];
extern int8_t sobelMaskY[3][3];
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

#define RUN_TEST
// #define RUN_MAIN

int main(){

  bmpheader bmp;
  uint8_t **raw_img = imageBmpReadBGR((char *)IMAGE_NAME, &bmp);
  showBmpHeaderInformation(&bmp);
  uint16_t imgW = getMatrixData(bmp.Width, 4);
  uint16_t imgH = getMatrixData(bmp.Height, 4);
  uint32_t imgSize = imgW*imgH;
  double *imgB = imgUint82Double(raw_img[0], imgSize);
  double *imgG = imgUint82Double(raw_img[1], imgSize);
  double *imgR = imgUint82Double(raw_img[2], imgSize);

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


#ifdef RUN_MAIN
  printf("\n===== Main =====\n");
  // double *grayImg = BGR2Gray(imgB, imgG, imgR, imgSize);

  // for(uint32_t i=0;i<imgSize;i++) imgB[i] = (imgB[i] < 50)*255.0;
  double *imgB_SobleX = maskFilter(imgB, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgB_SobleY = maskFilter(imgB, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgB_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgB_SobleY, imgSize, IMAGE_ABS, 0);
  imageBmpWrite((char *)"imgB.bmp", &bmp, imgDouble2Uint8(imgB, imgSize));
  imageBmpWrite((char *)"imgB_SobleX.bmp", &bmp, imgDouble2Uint8(imgB_SobleX, imgSize));
  imageBmpWrite((char *)"imgB_SobleY.bmp", &bmp, imgDouble2Uint8(imgB_SobleY, imgSize));
  double *sumImgB = twoImgOperation(imgB_SobleX, imgB_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgB, imgSize, IMAGE_ABS, 0);
  sumImgB = medianFilter(sumImgB, imgW, imgH, 3);
  imageBmpWrite((char *)"imgB_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgB, imgSize));

  double *imgG_SobleX = maskFilter(imgG, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgG_SobleY = maskFilter(imgG, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgG_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgG_SobleY, imgSize, IMAGE_ABS, 0);
  imageBmpWrite((char *)"imgG.bmp", &bmp, imgDouble2Uint8(imgG, imgSize));
  imageBmpWrite((char *)"imgG_SobleX.bmp", &bmp, imgDouble2Uint8(imgG_SobleX, imgSize));
  imageBmpWrite((char *)"imgG_SobleY.bmp", &bmp, imgDouble2Uint8(imgG_SobleY, imgSize));
  double *sumImgG = twoImgOperation(imgG_SobleX, imgG_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgG, imgSize, IMAGE_ABS, 0);
  sumImgG = medianFilter(sumImgG, imgW, imgH, 3);
  imageBmpWrite((char *)"imgG_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgG, imgSize));

  double *imgR_SobleX = maskFilter(imgR, imgW, imgH, &sobelMaskX[0][0], 3, 1);
  double *imgR_SobleY = maskFilter(imgR, imgW, imgH, &sobelMaskY[0][0], 3, 1);
  imgOperation(imgR_SobleX, imgSize, IMAGE_ABS, 0);
  imgOperation(imgR_SobleY, imgSize, IMAGE_ABS, 0);
  imageBmpWrite((char *)"imgR.bmp", &bmp, imgDouble2Uint8(imgR, imgSize));
  imageBmpWrite((char *)"imgR_SobleX.bmp", &bmp, imgDouble2Uint8(imgR_SobleX, imgSize));
  imageBmpWrite((char *)"imgR_SobleY.bmp", &bmp, imgDouble2Uint8(imgR_SobleY, imgSize));
  double *sumImgR = twoImgOperation(imgR_SobleX, imgR_SobleY, imgSize, IMAGE_PLUS);
  imgOperation(sumImgR, imgSize, IMAGE_ABS, 0);
  sumImgR = medianFilter(sumImgR, imgW, imgH, 3);
  imageBmpWrite((char *)"imgR_SobleXY.bmp", &bmp, imgDouble2Uint8(sumImgR, imgSize));



  double *allImg = twoImgOperation(sumImgB, sumImgG, imgSize, IMAGE_PLUS);
  allImg = twoImgOperation(allImg, sumImgR, imgSize, IMAGE_PLUS);
  imgOperation(allImg, imgSize, IMAGE_ABS, 0);

  imageBmpWrite((char *)"allImg.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  for(uint32_t i=0;i<imgSize;i++) allImg[i] = (allImg[i] > 100)*255.0;
  imageBmpWrite((char *)"allImg2.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));

  allImg = medianFilter(allImg, imgW, imgH, 3);
  allImg = medianFilter(allImg, imgW, imgH, 5);
  allImg = medianFilter(allImg, imgW, imgH, 7);
  allImg = dilation(allImg, imgW, imgH, &mMask2[0][0], 3, 3);
  allImg = erosion(allImg, imgW, imgH, &mMask1[0][0], 3, 3);
  allImg = erosion(allImg, imgW, imgH, &mMask1[0][0], 3, 3);

  imageBmpWrite((char *)"allImg3.bmp", &bmp, imgDouble2Uint8(allImg, imgSize));


#endif

  printf("\n===== END =====\n");
  return 0;
}
