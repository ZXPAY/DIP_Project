#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "lib/Danny_Bmp.h"

#define LENA_ORIGINAL_NAME "lena512.bmp"
#define LENA_FIRST_TRANSFORMATION_NAME "lenaFirstTransformation.bmp"
#define LENA_SECOND_TRANSFORMATION_NAME "lenaSecondTransformation.bmp"
#define LENA_FIRST_RESORATION_NAME "lenaFirstRestoration.bmp"
#define LENA_SECOND_RESORATION_NAME "lenaSecondRestoration.bmp"

#define LENA_GRAY_LEVEL_ORIGINAL_NAME "lena512GrayLevel.txt"
#define LENA_GRAY_LEVEL_FIRST_TRANSFORMATION_NAME "lenaFirstTransformationGrayLevel.txt"
#define LENA_GRAY_LEVEL_SECOND_TRANSFORMATION_NAME "lenaSecondTransformationGrayLevel.txt"
#define LENA_GRAY_LEVEL_FIRST_RESTORATION_NAME "lenaFirstRestorationGrayLevel.txt"
#define LENA_GRAY_LEVEL_SECOND_RESTORATION_NAME "lenaSecondRestorationGrayLevel.txt"

/*
  project:DIP (Digital Image Processing) Homework
  1. Read .bmp file and show the header
  2. Calculate Gray-Level Histogram
  3. Power-Law Transmission
  4. Get Original Image
*/

using namespace std;

int main(){
  bmpheader lenaBmpHeader;
  // write *.bmp name
  fstream file;
  file.open("bmpHeaderName.txt", ios::out);
  if(file.fail()){
    cout << "檔案開啟失敗 !!!" << endl;
  }
  else{
    file << LENA_ORIGINAL_NAME << "\r\n";
    file << LENA_FIRST_TRANSFORMATION_NAME << "\r\n";
    file << LENA_SECOND_TRANSFORMATION_NAME << "\r\n";
    file << LENA_FIRST_RESORATION_NAME << "\r\n";
    file << LENA_SECOND_RESORATION_NAME << "\r\n";
  }
  file.close();
  file.open("txtHeaderName.txt", ios::out);
  if(file.fail()){
    cout << "檔案開啟失敗 !!!" << endl;
  }
  else{
    file << LENA_GRAY_LEVEL_ORIGINAL_NAME << "\r\n";
    file << LENA_GRAY_LEVEL_FIRST_TRANSFORMATION_NAME << "\r\n";
    file << LENA_GRAY_LEVEL_SECOND_TRANSFORMATION_NAME << "\r\n";
    file << LENA_GRAY_LEVEL_FIRST_RESTORATION_NAME << "\r\n";
    file << LENA_GRAY_LEVEL_SECOND_RESTORATION_NAME << "\r\n";
  }
  file.close();
  cout << endl;
  /*
    first --> Read .bmp file and extract header file
  */
  cout << "============== Extract .bmp file ==============" << endl;
  char fileName[] = "lena512.bmp";
  readBmpImageHeader(fileName, &lenaBmpHeader, SHOW_BMP_HEADER_INFORMATION);
  uint8_t img_data[lenaBmpHeader.BitmapDataSize];
  readBmpImageData(fileName, &lenaBmpHeader, img_data);

  /*
    second --> Calculate Gray Level Histogram
  */
  cout << "============== Calculate Gray Level Histogram ==============" << endl;
  uint16_t imgWidth = lenaBmpHeader.Width;
  uint16_t imgHeight = lenaBmpHeader.Height;
  uint32_t grayLevelHistogramCounter[256];
  cout << "Image Data Size is : " << sizeof(img_data) << " Bytes" << endl;
  setGrayLevelHistogramData(&lenaBmpHeader, img_data, grayLevelHistogramCounter);
  cout << "============== Write Gray Level Data To .txt ==============" << endl;
  saveGrayLevelHistogramData((char*)LENA_GRAY_LEVEL_ORIGINAL_NAME, grayLevelHistogramCounter);

  /*
    third --> Power-Low Transformation
  */
  cout << endl;
  cout << "============== Power-Low Transformation ==============" << endl;
  double c = 1;
  double r = 0.5;
  cout << "============== First Transformation: c = 1, r = 0.5 ==============" << endl;
  c = 1;
  r = 0.5;
  uint8_t imgPowerLawFirstTransformation[lenaBmpHeader.BitmapDataSize];
  for(int w=0;w<imgWidth;w++){
    for(int h=0;h<imgHeight;h++){
      *(imgPowerLawFirstTransformation+w*imgWidth+h) = (uint8_t)(c*pow((*(img_data+w*imgWidth+h))/255.0, r)*255.0);
    }
  }
  writeBmpImage((char*)LENA_FIRST_TRANSFORMATION_NAME, &lenaBmpHeader, imgPowerLawFirstTransformation);
  setGrayLevelHistogramData(&lenaBmpHeader, imgPowerLawFirstTransformation, grayLevelHistogramCounter);
  cout << "============== Write Gray Level Data To .txt ==============" << endl;
  saveGrayLevelHistogramData((char*)LENA_GRAY_LEVEL_FIRST_TRANSFORMATION_NAME, grayLevelHistogramCounter);

  cout << "============== Second Transformation: c = 1, r = 4.0 ==============" << endl;
  c = 1;
  r = 4;
  uint8_t imgPowerLawSecondTransformation[lenaBmpHeader.BitmapDataSize];
  for(int w=0;w<imgWidth;w++){
    for(int h=0;h<imgHeight;h++){
      *(imgPowerLawSecondTransformation+w*imgWidth+h) = (uint8_t)(c*pow((*(img_data+w*imgWidth+h))/255.0, r)*255.0);
    }
  }
  writeBmpImage((char*)LENA_SECOND_TRANSFORMATION_NAME, &lenaBmpHeader, imgPowerLawSecondTransformation);
  setGrayLevelHistogramData(&lenaBmpHeader, imgPowerLawSecondTransformation, grayLevelHistogramCounter);
  cout << "============== Write Gray Level Data To .txt ==============" << endl;
  saveGrayLevelHistogramData((char*)LENA_GRAY_LEVEL_SECOND_TRANSFORMATION_NAME, grayLevelHistogramCounter);

  cout << endl;
  cout << "============== Power-Low Restoration ==============" << endl;
  cout << "============== First To Origin: c = 1, r = 2.0 ==============" << endl;
  c = 1;
  r = 2;
  uint8_t imgPowerLawFirstRestoration[lenaBmpHeader.BitmapDataSize];
  for(int w=0;w<imgWidth;w++){
    for(int h=0;h<imgHeight;h++){
      *(imgPowerLawFirstRestoration+w*imgWidth+h) = (uint8_t)(c*pow((*(imgPowerLawFirstTransformation+w*imgWidth+h))/255.0, r)*255.0);
    }
  }
  writeBmpImage((char*)LENA_FIRST_RESORATION_NAME, &lenaBmpHeader, imgPowerLawFirstRestoration);
  setGrayLevelHistogramData(&lenaBmpHeader, imgPowerLawFirstRestoration, grayLevelHistogramCounter);
  cout << "============== Write Gray Level Data To .txt ==============" << endl;
  saveGrayLevelHistogramData((char*)LENA_GRAY_LEVEL_FIRST_RESTORATION_NAME, grayLevelHistogramCounter);

  cout << "============== Second To Origin: c = 1, r = 0.25 ==============" << endl;
  c = 1;
  r = 0.25;
  uint8_t imgPowerLawSecondRestoration[lenaBmpHeader.BitmapDataSize];
  for(int w=0;w<imgWidth;w++){
    for(int h=0;h<imgHeight;h++){
      *(imgPowerLawSecondRestoration+w*imgWidth+h) = (uint8_t)(c*pow((*(imgPowerLawSecondTransformation+w*imgWidth+h))/255.0, r)*255.0);
    }
  }
  writeBmpImage((char*)LENA_SECOND_RESORATION_NAME, &lenaBmpHeader, imgPowerLawSecondRestoration);
  setGrayLevelHistogramData(&lenaBmpHeader, imgPowerLawSecondRestoration, grayLevelHistogramCounter);
  cout << "============== Write Gray Level Data To .txt ==============" << endl;
  saveGrayLevelHistogramData((char*)LENA_GRAY_LEVEL_SECOND_RESTORATION_NAME, grayLevelHistogramCounter);

  return 0;
}
