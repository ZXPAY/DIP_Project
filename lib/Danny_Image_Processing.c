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
  }

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
