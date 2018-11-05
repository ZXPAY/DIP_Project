#ifndef TESTDCT_H
#define TESTDCT_H
#include "testDCT.h"
#include <stdlib.h>
#include <stdio.h>
#include "lib/Danny_Image_Processing.h"

void testDCT_Transform(){
  uint8_t testMatrix[8][8] = {
    {52,55,61,66,70,61,64,73},
    {63,59,66,90,109,85,69,72},
    {62,59,68,113,144,104,66,73},
    {63,58,71,122,154,105,70,69},
    {67,61,68,104,126,88,68,70},
    {79,65,60,70,77,68,58,75},
    {85,71,64,59,55,61,65,83},
    {87,79,69,68,65,76,78,94}
  };
  uint8_t *raw_img = &testMatrix[0][0];

  printf("\n\n====== Test Matrix ======\n");
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      printf("%d\t", testMatrix[i][j]);
    }
    printf("\n");
  }

  double *img = DCT_transform(raw_img, 8, 8);
  Quantization_transform(img, 8, 8);
  printf("\n\n====== DCT & Quantization ======\n");
  printTestMatrix(img);

  iQuantization_transform(img, 8, 8);
  double *inv_img = iDCT_transform(img, 8, 8);
  printf("\n\n====== iQuantization & iDCT ======\n");
  printTestMatrix(inv_img);

  double *imgd = DCT_transform(raw_img, 8, 8);
  filterMask_transform(imgd, 8, 8);
  printf("\n\n====== DCT & FilterMask ======\n");
  printTestMatrix(imgd);

}


void printTestMatrix(double *mat){
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      printf("%.2f\t", *(mat+i*8+j));
    }
    printf("\n");
  }
}

#endif
