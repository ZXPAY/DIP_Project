#include "Danny_Image_Processing.h"
double QuantizationMatrix[8][8] = {
  {16, 11, 10, 16, 24, 40, 51, 61},
  {12, 12, 14, 19, 26, 58, 60, 55},
  {14, 13, 16, 24, 40, 57, 69, 56},
  {14, 17, 22, 29, 51, 57, 80, 62},
  {18, 22, 37, 56, 68, 109, 103, 77},
  {24, 35, 55, 64, 81, 104, 113, 92},
  {49, 64, 78, 87, 103, 121, 120, 101},
  {72, 92, 95, 98, 112, 100, 103, 99}
};
double FilterMask[8][8] = {
  {1,1,1,0,0,0,0,0},
  {1,1,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
};

int8_t gaussianMask[3][3] = {
  // need to divide 16
  {1,2,1},
  {2,4,2},
  {1,2,1},
};

int8_t laplacianMask_1[3][3] = {
    {0,1,0},
    {1,-4,1},
    {0,1,0},
  };
int8_t laplacianMask_2[3][3] = {
    {1,1,1},
    {1,-8,1},
    {1,1,1},
  };
int8_t sobelMaskX[3][3] = {
    {-1,-2,-1},
    {0,0,0},
    {1,2,1},
  };
int8_t sobelMaskY[3][3] = {
    {-1,0,1},
    {-2,0,2},
    {-1,0,1},
  };

double *DCT_transform(uint8_t *originImage, uint16_t imgW, uint16_t imgH){
  double *img = (double *)malloc(imgW*imgH*sizeof(double));
  for(int u=0;u<imgW;u+=8){
    for(int v=0;v<imgH;v+=8){
      DCT_transformUnit(originImage, img, imgW, u, v);
    }
  }
  return img;
}

void DCT_transformUnit(uint8_t *originImage, double *img, uint16_t imgW, uint16_t startX, uint16_t startY){
  double tempData;
  double sum_tempData = 0;
  for(int u=0;u<8;u++){
    for(int v=0;v<8;v++){
      sum_tempData = 0;
      for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){
          tempData = ((double)(originImage[(x+startX)*imgW+y+startY]))-128;
          sum_tempData += (double)((tempData)*cos((double)((2*x+1)*(u)*pi)/16.0)*cos((double)(((2*(y)+1)*v*pi))/16.0));
        }
      }
      double c_u = 1.0, c_v = 1.0;
      if(u == 0) c_u = 1/(sqrt(2));
      if(v == 0) c_v = 1/(sqrt(2));
      sum_tempData *= 0.25*c_u*c_v;
      img[(u+startX)*imgW+v+startY] = sum_tempData;
      // img[(u+startX)*imgW+v+startY] = sum_tempData/QuantizationMatrix[u][v]*FilterMask[u][v];
    }
  }
}

void Quantization_transform(double *img, uint16_t imgW, uint16_t imgH){
  for(int u=0;u<imgW;u+=8){
    for(int v=0;v<imgH;v+=8){
      for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){
          img[(u+x)*imgW+v+y] = round(img[(u+x)*imgW+v+y]/QuantizationMatrix[x][y]);
          // img[(u+x)*imgW+v+y] = img[(u+x)*imgW+v+y]/QuantizationMatrix[x][y];
        }
      }
    }
  }
}

// ********************************************************************** //
// ********************************************************************** //
double *iDCT_transform(double *img, uint16_t imgW, uint16_t imgH){
  double *iMat = (double *)malloc(imgW*imgH*sizeof(double));
  for(int x=0;x<imgW;x+=8){
    for(int y=0;y<imgH;y+=8){
      iDCT_transformUnit(img, iMat, imgW, x, y);
    }
  }
  return iMat;
}

void iQuantization_transform(double *img, uint16_t imgW, uint16_t imgH){
  for(int x=0;x<imgW;x+=8){
    for(int y=0;y<imgH;y+=8){
        for(int u=0;u<8;u++){
          for(int v=0;v<8;v++){
            img[(u+x)*imgW+v+y] *= QuantizationMatrix[u][v];
          }
      }
    }
  }
}

void iDCT_transformUnit(double *img, double *inv_img, uint16_t imgW, uint16_t startX, uint16_t startY){
  double sum_tempData = 0;
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      sum_tempData = 0;
      for(int u=0;u<8;u++){
        for(int v=0;v<8;v++){
          double c_u = 1.0, c_v = 1.0;
          if(u == 0) c_u = 1/(sqrt(2));
          if(v == 0) c_v = 1/(sqrt(2));
          sum_tempData += c_u*c_v*img[(u+startX)*imgW+v+startY]*cos((double)((2.0*x+1)*u*pi/16))*cos((double)((2.0*y+1)*v*pi/16));
        }
      }
      sum_tempData *= 0.25;
      inv_img[(x+startX)*imgW+y+startY] = sum_tempData + 128;
    }
  }
}

void filterMask_transform(double *img, uint16_t imgW, uint16_t imgH){
  for(int u=0;u<imgW;u+=8){
    for(int v=0;v<imgH;v+=8){
      for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){
          img[(u+x)*imgW+v+y] *= FilterMask[x][y];
        }
      }
    }
  }
}

double *imgUint82Double(uint8_t *img, uint32_t imgSize){
  double *new_img = (double*)malloc(imgSize * sizeof(double));
  for(uint32_t s=0;s<imgSize;s++){
    new_img[s] = (double)img[s];
  }
  return new_img;
}

uint8_t *imgDouble2Uint8(double *img, uint32_t imgSize){
  uint8_t *new_img = (uint8_t *)malloc(imgSize);
  for(uint32_t s=0;s<imgSize;s++){
     new_img[s] = (uint8_t)round(img[s]);
  }
  return new_img;
}

void imgOperation(double *img, uint32_t imgSize, uint16_t operation, double value){
  uint32_t i;
  switch (operation) {
    case IMAGE_PLUS:
      for(i=0;i<imgSize;i++){
          img[i] += value;
      }
    break;
    case IMAGE_MINUS:
      for(i=0;i<imgSize;i++){
          img[i] -= value;
      }
    break;
    case IMAGE_MULTIFY:
      for(i=0;i<imgSize;i++){
          img[i] *= value;
      }
    break;
    case IMAGE_DIVIDE:
      for(i=0;i<imgSize;i++){
          img[i] /= value;
      }
    break;
    case IMAGE_ABS:
      for(i=0;i<imgSize;i++){
          img[i] = abs(img[i]);
      }
    break;
    default:
      printf("Warning !!! No image operator found !!!\n");
    break;
  }
}

double *twoImgOperation(double *img1, double *img2, uint32_t imgSize, uint16_t operation){
  double *imgOperated = (double *)malloc(imgSize*sizeof(double));
  uint32_t i;
  switch (operation) {
    case IMAGE_PLUS:
      for(i=0;i<imgSize;i++){
          imgOperated[i] = img1[i] + img2[i];
      }
    break;
    case IMAGE_MINUS:
      for(i=0;i<imgSize;i++){
          imgOperated[i] = img1[i] - img2[i];
      }
    break;
    case IMAGE_MULTIFY:
      for(i=0;i<imgSize;i++){
          imgOperated[i] = img1[i] * img2[i];
      }
    break;
    case IMAGE_DIVIDE:
      for(i=0;i<imgSize;i++){
          imgOperated[i] = img1[i] / img2[i];
      }
    break;
    default:
      printf("Warning !!! No image operator found !!!\n");
    break;
  }
  return imgOperated;
}

double *imgDownSampling(uint8_t *originImage, uint16_t imgW, uint16_t imgH){
  double *dimg = (double *)malloc((uint32_t)(imgW*imgH/4*sizeof(double)));
  uint32_t newC = 0;
  for(uint16_t w=0;w<imgW;w++){
    for(uint16_t h=0;h<imgH;h++){
      if((h&1)&&(w&1)){
        dimg[newC] = originImage[w*imgW+h];
        newC++;
      }

    }
  }
  // printf("nn->%lu\n", newC);
  return dimg;
}

double *imgDownSamplingMean(uint8_t *originImage, uint16_t imgW, uint16_t imgH){
  double *dimg = (double *)malloc((uint32_t)(imgW*imgH/4*sizeof(double)));
  double meanValue = 0;;
  for(uint16_t w=0;w<imgW;w+=2){
    for(uint16_t h=0;h<imgH;h+=2){
      meanValue = 0;
      for(int i=0;i<2;i++){
          for(int j=0;j<2;j++){
            meanValue += (double) originImage[(w+i)*imgW+h+j];
          }
      }
      dimg[(w/2)*imgW/2+h/2] = meanValue/4;
      // printf("%d\n", (w/2)*imgW+h/2);
    }
  }
  return dimg;
}

double getImageMSE(uint8_t *originImage, double *transferedImage, uint32_t imgSize){
  double sumError = 0;
  double *rawImage = (double *)imgUint82Double(originImage, imgSize);
  setDataNormalization(rawImage, imgSize);
  double *newImage = (double *)malloc(imgSize*sizeof(double));
  for(uint32_t i=0;i<imgSize;i++) newImage[i] = transferedImage[i];
  setDataNormalization(newImage, imgSize);

  for(uint32_t i=0;i<imgSize;i++){
    sumError += pow((((double)rawImage[i]) - newImage[i]), 2.0);
    // printf("%.2f\n", (((double)rawImage[i]) - newImage[i]));
  }

  free(rawImage);
  free(newImage);
  return sumError/((double)imgSize);
}

double getImagePSNR(uint8_t *originImage, double *transferedImage, uint32_t imgSize){
  double imgMSE = getImageMSE(originImage, transferedImage, imgSize);
  return 10 * (log10(255.0*255.0/imgMSE));
}


double *sortedMatrix(double *mat, uint32_t imgSize, bool descentOrNot){
  double *sortedMat = (double *)malloc(imgSize*sizeof(double));
  for(uint16_t i=0;i<imgSize;i++){sortedMat[i] = mat[i];}
  for(uint16_t i=0;i<imgSize;i++){
    for(uint16_t j=i+1;j<imgSize;j++){
      if(descentOrNot){
        if(sortedMat[i] < sortedMat[j]){
          swap(&sortedMat[i], &sortedMat[j]);
        }
      }
      else{
        if(sortedMat[i] > sortedMat[j]){
          swap(&sortedMat[i], &sortedMat[j]);
        }
      }
    }
  }
  return sortedMat;
}

double *getPaddingImage(double *img, uint16_t imgW, uint16_t imgH, uint16_t maskSide){
  if(maskSide % 2 == 0) {printf("Warning !!! MaskSide is odd !!!\n");}
  uint16_t paddingNum = (maskSide-1)/2;
  #ifdef SHOW_PADDING_DEBUG
    printf("padding %d, %d\n", paddingNum, (imgW+2*paddingNum)*(imgH+2*paddingNum));
  #endif
  double *paddingImg = (double *)malloc((imgW+2*paddingNum)*(imgH+2*paddingNum)*sizeof(double));
  for(uint16_t i=0;i<imgW+2*paddingNum;i++){
    for(uint16_t j=0;j<imgH+2*paddingNum;j++){
      bool paddingLogic = false;
      if(!paddingLogic){
        for(uint16_t l=0;l<paddingNum;l++){
            paddingLogic |=  (i==l)|| (j==l) || ((i==(imgW+paddingNum+l)) || (j==(imgH+paddingNum+l)));
            if(paddingLogic){
              break;
            }
        }
      }
      if(paddingLogic){
        paddingImg[i+(imgW+2*paddingNum)*j] = 0;
      }
      else{
        paddingImg[i+(imgW+2*paddingNum)*j] = img[(i-paddingNum)+imgW*(j-paddingNum)];
      }
      // printf("%d,%.0f\t", i*(imgH+2)+j,paddingImg[i*(imgH+2)+j]);
    }
    // printf("\n");
  }
  return paddingImg;
}

double getMedianNumber(double *mat, uint16_t posx, uint16_t posy, uint16_t maskSide, uint16_t square_side){
  double *matNew = (double *)malloc(maskSide*maskSide*sizeof(double));
  uint16_t cnt = 0;
  for(uint16_t w=0;w<maskSide;w++){
    for(uint16_t h=0;h<maskSide;h++){
      matNew[cnt] = mat[(w+posx)+(square_side)*(posy+h)];
      #ifdef SHOW_MEDIAN_FILTER_DEBUG
      if((w+posx)+(square_side)*(posy+h) < 1000)
        printf("%d,%.0f\n",(w+posx)+(square_side)*(posy+h), mat[(w+posx)+(square_side)*(posy+h)]);
      #endif
      cnt++;
    }
    #ifdef SHOW_MEDIAN_FILTER_DEBUG
      printf("\n");
    #endif
  }
  #ifdef SHOW_MEDIAN_FILTER_DEBUG
    printf("Sorted\n");
  #endif
  double *sortedMat = sortedMatrix(matNew, maskSide*maskSide, true);
  #ifdef SHOW_MEDIAN_FILTER_DEBUG
    for(uint16_t i=0;i<maskSide*maskSide;i++) printf("%.0f,",sortedMat[i]);
    printf("\n");
  #endif
  double returnValue = sortedMat[(maskSide*maskSide+1)/2-1];
  free(matNew);
  free(sortedMat);
  return returnValue;
}

double* medianFilter(double *img, uint16_t imgW, uint16_t imgH, uint16_t maskSide){
  double *medianImg = (double *)malloc(imgW*imgH*sizeof(double));
  // Do padding
  double *paddingImg = getPaddingImage(img, imgW, imgH, maskSide);
  uint16_t paddingNum = (maskSide-1)/2;
  #ifdef SHOW_PADDING_DEBUG
    for(uint16_t i=0;i<imgW+2*paddingNum;i++){
      for(uint16_t j=0;j<imgH+2*paddingNum;j++){
        printf("%.0f\t", paddingImg[i+(imgW+2*paddingNum)*j]);
      }
      printf("\n");
    }
  #endif

  for(uint16_t w=0;w<imgW;w++){
    for(uint16_t h=0;h<imgH;h++){
      medianImg[w+imgW*h] = getMedianNumber(paddingImg, w, h, maskSide, imgW+2*paddingNum);
      #ifdef SHOW_MEDIAN_FILTER_DEBUG
        printf("Median : %.2f\n========\n", medianImg[w+imgW*h]);
      #endif
    }
  }
  free(paddingImg);
  return medianImg;
}

double* maskFilter(double *img, uint16_t imgW, uint16_t imgH, int8_t *mask, uint8_t maskSide, uint8_t stride){
  double *maskImg = (double *)malloc(imgW*imgH*sizeof(double));

  // Padding Size => (imgW+2, imgH+2)
  double *paddingImg = getPaddingImage(img, imgW, imgH, maskSide);
  uint16_t paddingNum = (maskSide-1)/2;
  for(uint16_t w=0;w<imgW;w+=stride){
    for(uint16_t h=0;h<imgH;h+=stride){
      maskImg[h*imgW+w] = 0;
      for(uint8_t i=0;i<maskSide;i++){
        for(uint8_t j=0;j<maskSide;j++){
          maskImg[h*imgW+w] += paddingImg[(w+i)+(imgW+2*paddingNum)*(h+j)] * (double)mask[i*maskSide+j];
          #ifdef SHOW_MASK_FILTER_DEBUG
            printf("%.0f\t", paddingImg[(w+i)+(imgW+2*paddingNum)*(h+j)]);
          #endif
        }
        #ifdef SHOW_MASK_FILTER_DEBUG
          printf("\n");
        #endif
      }
      #ifdef SHOW_MASK_FILTER_DEBUG
        printf("Mask Value : %.2f", maskImg[h*imgW+w]);
        printf("\n===============\n");
      #endif
    }
    // printf("\n");
  }
  return maskImg;
}

double* dilation(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH){
  double *maskImg = (double *)malloc(imgW*imgH*sizeof(double));
  // copy
  for(uint16_t w=0;w<imgW;w++){
    for(uint16_t h=0;h<imgH;h++){
      if(img[w+h*imgW] == 1.0){
        maskImg[w+h*imgW] = 255;
      }
      else{
        maskImg[w+h*imgW] = img[w+h*imgW];
      }
    }
  }
  uint8_t maskSum = 0;

  for(uint16_t w=0;w<imgW-maskW+1;w++){
    for(uint16_t h=0;h<imgH-maskH+1;h++){
      maskSum = 0;
      for(uint8_t i=0;i<maskW;i++){
        for(uint8_t j=0;j<maskH;j++){
          if((((uint8_t)img[w+i+(h+j)*imgW]) & 1 & ((uint8_t)mask[i+maskH*j])) == true){
            maskSum++;
          }
          #ifdef SHOW_DILATION_DEBUG
            printf("%d&%d,%d\t", (uint8_t)img[w+i+(h+j)*imgW], (uint8_t)mask[i+maskH*j]
                  ,(((uint8_t)img[w+i+(h+j)*imgW]) & ((uint8_t)mask[i+maskH*j])));
          #endif
        }
        #ifdef SHOW_DILATION_DEBUG
          printf("\n");
        #endif
      }
      if(maskSum!=0){
        maskImg[h*imgW+w] = 255.0;
      }
      else{
        maskImg[h*imgW+w] = 0.0;
      }
      #ifdef SHOW_DILATION_DEBUG
        printf("->%.0f\n=============\n", maskImg[h*imgW+w]);
      #endif
    }
  }
  return maskImg;
}

double* erosion(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH){
  double *maskImg = (double *)malloc(imgW*imgH*sizeof(double));
  uint8_t maskMax = 0;
  for(uint8_t i=0;i<maskW*maskH;i++){
    maskMax += mask[i]&1;
  }
  // copy
  for(uint16_t w=0;w<imgW;w++){
    for(uint16_t h=0;h<imgH;h++){
      if(img[w+h*imgW] == 1.0){
        maskImg[w+h*imgW] = 255;
      }
      else{
        maskImg[w+h*imgW] = img[w+h*imgW];
      }
    }
  }
  uint8_t maskSum = 0;

  for(uint16_t w=0;w<imgW-maskW+1;w++){
    for(uint16_t h=0;h<imgH-maskH+1;h++){
      maskSum = 0;
      for(uint8_t i=0;i<maskW;i++){
        for(uint8_t j=0;j<maskH;j++){
          if((((uint8_t)img[w+i+(h+j)*imgW]) & 1 & ((uint8_t)mask[i+maskH*j])) == true){
            maskSum++;
          }
          #ifdef SHOW_EROSION_DEBUG
            printf("%d&%d,%d\t", (uint8_t)img[w+i+(h+j)*imgW], (uint8_t)mask[i+maskH*j]
                  ,(((uint8_t)img[w+i+(h+j)*imgW]) & ((uint8_t)mask[i+maskH*j])));
          #endif
        }
        #ifdef SHOW_EROSION_DEBUG
          printf("\n");
        #endif
      }
      if(maskSum==maskMax){
        maskImg[h*imgW+w] = 255.0;
      }
      else{
        maskImg[h*imgW+w] = 0.0;
      }
      #ifdef SHOW_EROSION_DEBUG
        printf("->%.0f\n=============\n", maskImg[h*imgW+w]);
      #endif
    }
  }
  return maskImg;
}

double* opening(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH){
  double *erosionImg = erosion(img, imgW, imgH, mask, maskW, maskH);
  double *erosionDilationImg = dilation(erosionImg, imgW, imgH, mask, maskW, maskH);
  free(erosionImg);
  return erosionDilationImg;
}
double* closing(double *img, uint16_t imgW, uint16_t imgH, uint8_t *mask, uint8_t maskW, uint8_t maskH){
  double *dilationImg = dilation(img, imgW, imgH, mask, maskW, maskH);
  double *dilationErosionImg = erosion(dilationImg, imgW, imgH, mask, maskW, maskH);
  free(dilationImg);
  return dilationErosionImg;
}

double *connectedComponentLabling(double *img, uint16_t imgW, uint16_t imgH, double backgroundValue){
  // Using four neighbors method to mark the area
  double *marker = (double *)calloc(imgW*imgH, sizeof(double)); // initial value is 0
  double sortedValue[8] = {0,0,0,0,0,0,0,0};
  uint16_t labelValue = 0;
  double *sortedMat;
  double value;
  uint32_t imgSize = imgW*imgH;

  for(uint16_t w=1;w<imgW-1;w++){
    for(uint16_t h=1;h<imgH-1;h++){
      if(img[w+h*imgW] != backgroundValue){
        sortedValue[0] = marker[w-1+(h-1)*imgW];
        sortedValue[1] = marker[w-1+(h)*imgW];
        sortedValue[2] = marker[w-1+(h+1)*imgW];
        sortedValue[3] = marker[w+(h-1)*imgW];
        sortedValue[4] = marker[w+(h+1)*imgW];
        sortedValue[5] = marker[w+1+(h-1)*imgW];
        sortedValue[6] = marker[w+1+(h)*imgW];
        sortedValue[7] = marker[w+1+(h+1)*imgW];
        sortedMat = sortedMatrix(&sortedValue[0], 8, false);
        double value = 0;
        for(int i=1;i<8;i++) if(sortedMat[i]!=0){value=sortedMat[i];break;}
        if(value != 0){
          marker[w+h*imgW] = value;
        }
        else{
          labelValue++;
          marker[w+h*imgW] = labelValue;
        }
        free(sortedMat);
      }
    }
  }

  for(uint16_t k=0;k<(300);k++){
    for(uint16_t w=1;w<imgW-1;w++){
      for(uint16_t h=1;h<imgH-1;h++){
        if(img[w+h*imgW] != backgroundValue){
          sortedValue[0] = marker[w-1+(h-1)*imgW];
          sortedValue[1] = marker[w-1+(h)*imgW];
          sortedValue[2] = marker[w-1+(h+1)*imgW];
          sortedValue[3] = marker[w+(h-1)*imgW];
          sortedValue[4] = marker[w+(h+1)*imgW];
          sortedValue[5] = marker[w+1+(h-1)*imgW];
          sortedValue[6] = marker[w+1+(h)*imgW];
          sortedValue[7] = marker[w+1+(h+1)*imgW];
          sortedMat = sortedMatrix(&sortedValue[0], 8, false);
          value = 0;
          for(int i=1;i<8;i++) if(sortedMat[i]!=0){value=sortedMat[i];break;}
          if(value != 0){
            marker[w+h*imgW] = value;
          }
          free(sortedMat);
        }
      }
    }
  }

  uint8_t *haveValue = (uint8_t *)calloc(256, sizeof(uint8_t));
    uint8_t cnt = 0;
    for(uint32_t i=0;i<imgSize;i++){
      if(marker[i] != 0){
        bool flag = false;
        for(uint8_t j=0;j<cnt+1;j++){
          if((uint8_t)marker[i] == haveValue[j]){
            flag = true;
          }
        }
        if(!flag){
          haveValue[cnt] = (uint8_t)marker[i];
          cnt++;
        }
        uint8_t cnt2 = 0;
        while((uint8_t)marker[i] != haveValue[cnt2]){
          cnt2++;
        }
        marker[i] = cnt2+1;
      }
  }

  free(haveValue);
  return marker;
}


void swap(double *value1, double *value2){
  double tempData = *value1;
  *value1 = *value2;
  *value2 = tempData;
}

// BGR Processing
uint8_t** BGR2OneSave(double *imgB, double *imgG, double *imgR, uint32_t imgSize){
  uint8_t **BGR2One = (uint8_t **)malloc(3);
  BGR2One[0] = imgDouble2Uint8(imgB, imgSize);
  BGR2One[1] = imgDouble2Uint8(imgG, imgSize);
  BGR2One[2] = imgDouble2Uint8(imgR, imgSize);
  return BGR2One;
}
double* BGR2Gray(double *imgB, double *imgG, double *imgR, uint32_t imgSize){
  double *grayImg = (double *)malloc(imgSize*sizeof(double));
  for(uint32_t i=0;i<imgSize;i++){
    grayImg[i] = (imgB[i]+imgG[i]+imgR[i])/3;
  }
  return grayImg;
}
