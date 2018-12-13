#include "Danny_Bmp.h"

uint8_t* imageBmpRead(char *fileName, bmpheader *bmp){
  FILE* f_r = fopen(fileName, "rb");
  fread(bmp, sizeof(uint8_t), (BMP_HEADRER_SIZE+BMP_PLATTE_SIZE), f_r);
  setMatrixData(bmp->BitmapDataSize, 4, getMatrixData(bmp->Width, sizeof(bmp->Width))*getMatrixData(bmp->Height, sizeof(bmp->Height)));
  uint8_t *img_data = (uint8_t *) malloc(getMatrixData(bmp->BitmapDataSize, 4)*(sizeof(uint8_t)));
  fread(img_data, sizeof(uint8_t), getMatrixData(bmp->BitmapDataSize, 4), f_r);
  fclose(f_r);
  return img_data;
}

void imageBmpWrite(char *fileName, bmpheader *bmp, uint8_t* img_data){
  for(int i=0;i<256;i++){
      bmp->Plalette[i*4+0] = i;
      bmp->Plalette[i*4+1] = i;
      bmp->Plalette[i*4+2] = i;
      bmp->Plalette[i*4+3] = 0;
  }
  setMatrixData(bmp->BitsPerPixel, 4, 8);
  setMatrixData(bmp->UsedColorsSize, 4, 256);
  setMatrixData(bmp->BitmapDataOffset, 4, 1078);   // 1024 + 54

  FILE* f_w = fopen((char *)fileName, "wb");
  if(f_w == 0) printf("Error open !!!\n");
  uint32_t fileSize = getMatrixData(bmp->BitmapDataSize, sizeof(bmp->BitmapDataSize));
  fwrite(bmp, sizeof(uint8_t), (BMP_HEADRER_SIZE+BMP_PLATTE_SIZE), f_w);
  fwrite(img_data, sizeof(uint8_t), fileSize, f_w);
  fclose(f_w);
}

uint8_t** imageBmpReadBGR(char *fileName, bmpheader *bmp){
  FILE* f_r = fopen((char *)fileName, "rb");
  if(f_r == 0) printf("Error open !!!\n");
  fread(bmp, sizeof(uint8_t), (BMP_HEADRER_SIZE), f_r);
  setMatrixData(bmp->BitmapDataSize, 4, getMatrixData(bmp->Width, sizeof(bmp->Width))*getMatrixData(bmp->Height, sizeof(bmp->Height)));
  uint8_t *img_data = (uint8_t *) malloc(getMatrixData(bmp->BitmapDataSize, 4)*3*(sizeof(uint8_t)));
  fread(img_data, sizeof(uint8_t), getMatrixData(bmp->BitmapDataSize, 4)*3, f_r);
  fclose(f_r);

  uint8_t *r = (uint8_t *)malloc(getMatrixData(bmp->BitmapDataSize, 4)*sizeof(uint8_t));
  uint8_t *g = (uint8_t *)malloc(getMatrixData(bmp->BitmapDataSize, 4)*sizeof(uint8_t));
  uint8_t *b = (uint8_t *)malloc(getMatrixData(bmp->BitmapDataSize, 4)*sizeof(uint8_t));

  uint16_t rawImgW = getMatrixData(bmp->Width, sizeof(bmp->Width));
  uint16_t rawImgH = getMatrixData(bmp->Height, sizeof(bmp->Height));
  for(uint16_t w=0;w<rawImgW;w++){
    for(uint16_t h=0;h<rawImgH;h++){
      b[w+h*rawImgW] = img_data[3*(w+h*rawImgW)+0];
      g[w+h*rawImgW] = img_data[3*(w+h*rawImgW)+1];
      r[w+h*rawImgW] = img_data[3*(w+h*rawImgW)+2];
    }
  }
  uint8_t **imgBGR = (uint8_t **)malloc(3);
  imgBGR[0] = b;
  imgBGR[1] = g;
  imgBGR[2] = r;
  return imgBGR;
}

void imageBmpWriteBGR(char *fileName, bmpheader *bmp, uint8_t** img_data){
  setMatrixData(bmp->BitsPerPixel, 4, 24);
  setMatrixData(bmp->UsedColorsSize, 4, 0);
  setMatrixData(bmp->BitmapDataOffset, 4, 54);
  uint8_t* temp = (uint8_t *)malloc(getMatrixData(bmp->BitmapDataSize, 4)*3*sizeof(uint8_t));
  uint8_t* b = img_data[0];
  uint8_t* g = img_data[1];
  uint8_t* r = img_data[2];
  uint16_t imgW = getMatrixData(bmp->Width, 4);
  uint16_t imgH = getMatrixData(bmp->Height, 4);
  for(uint16_t w=0;w<imgW;w++){
    for(uint16_t h=0;h<imgH;h++){
      temp[3*(w+h*imgW)+0] = b[w+h*imgW];
      temp[3*(w+h*imgW)+1] = g[w+h*imgW];
      temp[3*(w+h*imgW)+2] = r[w+h*imgW];
    }
  }
  FILE* f_w = fopen((char *)fileName, "wb");
  if(f_w == 0) printf("Error open !!!\n");
  fwrite(bmp, sizeof(uint8_t), (BMP_HEADRER_SIZE), f_w);
  fwrite(temp, sizeof(uint8_t), imgW*imgH*3, f_w);
  fclose(f_w);
  free(temp);
}

// Calculate Bitmap File information
uint32_t getMatrixData(uint8_t *mat, uint8_t mat_size){
  uint8_t shiftTmp = 0;
  uint32_t returnValue = 0;
  for(int i=0;i<mat_size;i++){
    returnValue += (*(mat+i)) << shiftTmp;
    shiftTmp += 8;

  }
  return returnValue;
}

void setMatrixData(uint8_t *mat, uint8_t mat_size, uint32_t data){
  for(int i=0;i<mat_size;i++){
    mat[i] = ((data&(0xff<<(8*i)))>>(8*i));
  }
}

// show the BMP Header Structure Content
void showBmpHeaderInformation(bmpheader *bmp){
  printf("Identifier is : %d\n", getMatrixData(bmp->Identifier, sizeof(bmp->Identifier)));
  printf("FileSize is : %d\n", getMatrixData(bmp->FileSize, sizeof(bmp->FileSize)));
  printf("Reserve is : %d\n", getMatrixData(bmp->Reserve, sizeof(bmp->Reserve)));
  printf("BitmapDataOffset is : %d\n", getMatrixData(bmp->BitmapDataOffset, sizeof(bmp->BitmapDataOffset)));
  printf("BitmapHeaderSize is : %d\n", getMatrixData(bmp->BitmapHeaderSize, sizeof(bmp->BitmapHeaderSize)));
  printf("Width is : %d\n", getMatrixData(bmp->Width, sizeof(bmp->Width)));
  printf("Height is : %d\n", getMatrixData(bmp->Height, sizeof(bmp->Height)));
  printf("Planes is : %d\n", getMatrixData(bmp->Planes, sizeof(bmp->Planes)));
  printf("BitsPerPixel is : %d\n", getMatrixData(bmp->BitsPerPixel, sizeof(bmp->BitsPerPixel)));
  printf("Compression is : %d\n", getMatrixData(bmp->Compression, sizeof(bmp->Compression)));
  printf("BitmapDataSize is : %d\n", getMatrixData(bmp->BitmapDataSize, sizeof(bmp->BitmapDataSize)));
  printf("H_Rosolution is : %d\n", getMatrixData(bmp->H_Rosolution, sizeof(bmp->H_Rosolution)));
  printf("U_Rosolution is : %d\n", getMatrixData(bmp->U_Rosolution, sizeof(bmp->U_Rosolution)));
  printf("UsedColorsSize is : %d\n", getMatrixData(bmp->UsedColorsSize, sizeof(bmp->UsedColorsSize)));
  printf("ImportantColors is : %d\n", getMatrixData(bmp->ImportantColors, sizeof(bmp->ImportantColors)));
}

uint32_t *getGrayLevelHistogramData(double* img_data, uint16_t imgW, uint16_t imgH){
  uint32_t *grayLevelData = (uint32_t *)calloc(256, sizeof(uint32_t));
  for(int w=0;w<imgW;w++){
    for(int h=0;h<imgH;h++){
      grayLevelData[(uint8_t)img_data[w+h*(imgW)]]++;
    }
  }
  return grayLevelData;
}

// ===================================================== //
double getMinimum(double *mptr, uint32_t dataSize){
  double minData = *mptr;
  for(uint32_t n=0;n<dataSize;n++){
    if(*(mptr+n) < minData){
      minData = *(mptr+n);
    }
  }
  return minData;
}

double getMaximum(double *mptr, uint32_t dataSize){
  double maxData = *mptr;
  for(uint32_t n=0;n<dataSize;n++){
    if(*(mptr+n) > maxData){
      maxData = *(mptr+n);
    }
  }
  return maxData;
}

void setDataNormalization(double *ptr, uint32_t dataSize){
  double minData = getMinimum(ptr, dataSize);
  double maxData = getMaximum(ptr, dataSize);
  for(uint32_t n=0;n<dataSize;n++){
    ptr[n] = ((ptr[n] - minData)/(maxData - minData))*255.0;
  }
}

void testNormalization(){
  double testMat[6] = {1, 0.2, 6, 8, 3.2, -5};
  double ans[6] = {0.462, 0.4, 0.846, 1.0, 0.631, 0};
  double *ptr = &testMat[0];
  setDataNormalization(ptr, 6);
  for(int i=0;i<6;i++){
    printf("test : %.3f,  ans : %.3f\n", ptr[i], ans[i]);
  }
}

bool isValueInMatrix(uint8_t *mat, double value, uint32_t dataSize){
  bool isInMatrix = false;
  for(uint8_t i=0;i<=dataSize;i++){
    if(mat[i]==(uint8_t)value){
      isInMatrix = true;
    }
  }
  return isInMatrix;
}

uint8_t *getDifferentGray(double *grayImg, uint32_t dataSize){
  uint8_t cnt = 0;
  uint8_t *haveNum = (uint8_t *) calloc(255, sizeof(uint8_t));
  for(uint8_t p=0;p<=255;p++){
    if(grayImg[p]!=0){
      // if(!isValueInMatrix(haveNum, grayImg[p], cnt+1)){
      //   haveNum[cnt] = grayImg[p];
      // }
      printf("EFEF\n");
    }
  }
  return haveNum;
}
