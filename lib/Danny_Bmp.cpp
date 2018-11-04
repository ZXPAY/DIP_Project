#include "Danny_Bmp.h"
#include <iostream>
using namespace std;


void readBmpImageHeader(char* fileName, bmpheader *bmpStruct, bool showInfo){
  FILE* f_r = fopen(fileName, "rb");
  uint8_t info[BMP_HEADER_SIZE];
  fread(info, sizeof(uint8_t), BMP_HEADER_SIZE, f_r);

  uint32_t width = getInfo(info, 19, 22);
  uint32_t height = getInfo(info, 23, 26);
  uint32_t size = 512*512;

  bmpStruct->Identifier = getInfo(info, 1, 2);
  bmpStruct->FileSize = getInfo(info, 3, 6);
  bmpStruct->Reserve = getInfo(info, 7, 10);
  bmpStruct->BitmapDataOffset = getInfo(info, 11, 14);
  bmpStruct->BitmapHeaderSize = getInfo(info, 15, 18);
  bmpStruct->Width = 512;
  bmpStruct->Height = 512;
  bmpStruct->Planes = getInfo(info, 27, 28);
  bmpStruct->BitsPerPixel = getInfo(info, 29, 30);
  bmpStruct->Compression = getInfo(info, 31, 34);
  bmpStruct->BitmapDataSize = getInfo(info, 35, 38);
  bmpStruct->H_Rosolution = getInfo(info, 39, 42);
  bmpStruct->U_Rosolution = getInfo(info, 43, 46);
  bmpStruct->UsedColorsSize = getInfo(info, 47, 50);
  bmpStruct->ImportantColors = getInfo(info, 51, 54);

  if(showInfo){
    showBmpHeaderInformation(bmpStruct);
  }
  // 調色盤資料
  uint32_t usedColorSize = getInfo(info, 47, 50);
  uint8_t plalette[usedColorSize*4];
  fread(plalette, sizeof(uint8_t), 1024, f_r);

  uint8_t image_data[512*512];
  fread(image_data, sizeof(uint8_t), size, f_r);
}

void readBmpImageData(char* fileName, bmpheader *bmpStruct, uint8_t* img_data){
  uint8_t info[BMP_HEADER_SIZE];
  FILE* f_r = fopen(fileName, "rb");
  fread(info, sizeof(uint8_t), BMP_HEADER_SIZE, f_r);
  bmpStruct->Plalette = (uint8_t*) malloc(1024);
  fread(bmpStruct->Plalette, sizeof(uint8_t), 1024, f_r);
  cout<<"WWW" <<endl;
  // Read Data
  fread(img_data, sizeof(uint8_t), 512*512, f_r);
  fclose(f_r);
  cout<<"GGG" <<endl;
}

void writeBmpImage(char* fileName, bmpheader *bmpStruct, uint8_t* img_data){
  FILE* f_w = fopen(fileName, "wb");
  fwrite(&bmpStruct->Identifier, sizeof(uint8_t), 2, f_w);
  fwrite(&bmpStruct->FileSize, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->Reserve, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->BitmapDataOffset, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->BitmapHeaderSize, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->Width, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->Height, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->Planes, sizeof(uint8_t), 2, f_w);
  fwrite(&bmpStruct->BitsPerPixel, sizeof(uint8_t), 2, f_w);
  fwrite(&bmpStruct->Compression, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->BitmapDataSize, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->H_Rosolution, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->U_Rosolution, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->UsedColorsSize, sizeof(uint8_t), 4, f_w);
  fwrite(&bmpStruct->ImportantColors, sizeof(uint8_t), 4, f_w);
  fwrite(bmpStruct->Plalette, sizeof(uint8_t), 1024, f_w);
  fwrite(img_data, sizeof(uint8_t), 512*512, f_w);
  fclose(f_w);
}

// Calculate Bitmap File information
// start <= Header information number <=stop
uint32_t getInfo(uint8_t *info, uint8_t start, uint8_t stop){
  uint32_t returnValue = 0;
  uint8_t shiftTmp = 0;
  for(int n=start-1;n<stop;n++){
    returnValue += (*(info+n) << shiftTmp);
    shiftTmp += 8;
  }
  return returnValue;
}

// show the BMP Header Structure Content
void showBmpHeaderInformation(bmpheader *bmpStruct){
  if(bmpStruct->Width*bmpStruct->Height == bmpStruct->BitmapDataSize) {cout << "Gray image." << endl;}
  else {cout << "RBG Image." << endl;}
  cout << "============== Bitmap File Header ==============" << endl;
  cout << "Identifier : " << bmpStruct->Identifier << endl;
  cout << "File Size : " << bmpStruct->FileSize << " Bytes" << endl;
  cout << "Reserve : " << bmpStruct->Reserve << endl;
  cout << "Bitmap Data Offset : " << bmpStruct->BitmapDataOffset << endl;
  cout << endl;
  cout << "============== Bitmap Info Header ==============" << endl;
  cout << "Bitmap Header Size : " << bmpStruct->BitmapHeaderSize<< endl;
  cout << "Width : " << bmpStruct->Width << endl;
  cout << "Height : " << bmpStruct->Height << endl;
  cout << "Planes : " << bmpStruct->Planes << endl;
  cout << "Bits Per Pixel : " << bmpStruct->BitsPerPixel << endl;
  cout << "Compression : " << bmpStruct->Compression << endl;
  cout << "Bitmap Data Size : " << bmpStruct->BitmapDataSize << endl;
  cout << "H-Rosolution : " << bmpStruct->H_Rosolution << endl;
  cout << "U-Rosolution : " << bmpStruct->U_Rosolution << endl;
  cout << "Used Colors Size : " << bmpStruct->UsedColorsSize << endl;
  cout << "Important Colors: " << bmpStruct->ImportantColors << endl;
}

void setGrayLevelHistogramData(bmpheader* bmpStruct, uint8_t* img_data, uint32_t* GrayLevelData){
  for(int i=0;i<256;i++){
    *(GrayLevelData+i) = 0;
  }
  for(int w=0;w<bmpStruct->Width;w++){
    for(int h=0;h<bmpStruct->Height;h++){
      (*(GrayLevelData+*(img_data+h+w*bmpStruct->Width)))++;
    }
  }
}

void saveGrayLevelHistogramData(char* fileName, uint32_t* grayLevelData){
  fstream file;
  file.open(fileName, ios::out);
  if(file.fail())
    cout << "File can not open !!!" << endl;
  else{
    // write data to .txt
    // 8 bits
    for(int n=0;n<256;n++){
      file << n << "," << *(grayLevelData+n) << ",\r\n";
    }
  }
  cout << fileName << " Build file succeed !!!" << endl;
  file.close();
}
