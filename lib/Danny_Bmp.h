#ifndef DANNY_BMP_H
#define DANNY_BMP_H
#include <iostream>
#include <fstream>

#define SHOW_BMP_HEADER_INFORMATION true
#define BMP_HEADER_SIZE 54      // bytes

#pragma pack(1)
typedef struct BMP_HEADER{
  uint16_t Identifier;
  uint32_t FileSize;
  uint32_t Reserve;
  uint32_t BitmapDataOffset;
  uint32_t BitmapHeaderSize;
  uint32_t Width;
  uint32_t Height;
  uint16_t Planes;
  uint16_t BitsPerPixel;
  uint32_t Compression;
  uint32_t BitmapDataSize;
  uint32_t H_Rosolution;
  uint32_t U_Rosolution;
  uint32_t UsedColorsSize;
  uint32_t ImportantColors;
  uint8_t* Plalette;
}bmpheader;

void readBmpImageHeader(char* fileName, bmpheader *bmpStruct, bool showInfo);  // given default show info false
void readBmpImageData(char* fileName, bmpheader *bmpStruct, uint8_t* img_data);
void writeBmpImage(char* fileName, bmpheader *bmpStruct, uint8_t* img_data);
uint32_t getInfo(uint8_t *info, uint8_t start, uint8_t stop);
void showBmpHeaderInformation(bmpheader *bmpStruct);
void setGrayLevelHistogramData(bmpheader* bmpStruct, uint8_t* img_array, uint32_t* GrayLevelData);
void saveGrayLevelHistogramData(char* fileName, uint32_t* grayLevelData);

#endif
