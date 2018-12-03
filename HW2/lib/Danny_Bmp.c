#include "Danny_Bmp.h"

uint8_t* imageBmpRead(char *fileName, bmpheader *bmp){
  FILE* f_r = fopen(fileName, "rb");
  fread(bmp, sizeof(uint8_t), (BMP_HEADRER_SIZE+BMP_PLATTE_SIZE), f_r);
  uint32_t fileSize = getMatrixData(bmp->FileSize, sizeof(bmp->FileSize));
  setMatrixData(bmp->BitmapDataSize, 4, getMatrixData(bmp->Width, sizeof(bmp->Width))*getMatrixData(bmp->Height, sizeof(bmp->Height)));
  uint8_t *img_data = (uint8_t *) malloc(fileSize*(sizeof(uint8_t)));
  fread(img_data, sizeof(uint8_t), fileSize, f_r);
  fclose(f_r);
  return img_data;
}

void imageBmpWrite(char *fileName, bmpheader *bmp, uint8_t* img_data){
  FILE* f_w = fopen((char *)fileName, "wb");
  if(f_w == 0) printf("Error open !!!\n");
  uint32_t fileSize = getMatrixData(bmp->BitmapDataSize, sizeof(bmp->BitmapDataSize));
  fwrite(bmp, sizeof(uint8_t), (BMP_HEADRER_SIZE+BMP_PLATTE_SIZE), f_w);
  fwrite(img_data, sizeof(uint8_t), fileSize, f_w);
  fclose(f_w);
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

void setGrayLevelHistogramData(bmpheader* bmp, uint8_t* img_data, uint32_t* GrayLevelData){
  uint16_t widthSize = getMatrixData(bmp->Width, sizeof(bmp->Width));
  uint16_t HeightSize = getMatrixData(bmp->Height, sizeof(bmp->Height));
  for(int i=0;i<256;i++){
    *(GrayLevelData+i) = 0;
  }
  for(int w=0;w<widthSize;w++){
    for(int h=0;h<HeightSize;h++){
      (*(GrayLevelData+*(img_data+h+w*(widthSize))))++;
    }
  }
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
