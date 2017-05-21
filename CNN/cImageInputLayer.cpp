#include "cImageInputLayer.h"

cImageInputLayer::cImageInputLayer(int inputNum,int NetworkOutputNum,int batchSize)
    :cLayer(inputNum, inputNum, batchSize){
    layerType = "MinistInputLayer";
	this->inputSize = inputNum;
	this->outputSize = inputNum;

    output=new double[outputNum*batchSize];
//	answer=new int[batchSize];
}
cImageInputLayer::~cImageInputLayer(){
    delete []output;
//    delete []answer;
}

cImageInputLayer::cImageInputLayer(const char *filePath)
    :cLayer(0, 0, 0){
    ;
}
void cImageInputLayer::saveLayer(const char *filePath){
    ;
}
void cImageInputLayer::fPropagation(double *NetworkCorrect){
    ;
}


void cImageInputLayer::getDataFromImage(const char* filename){
    //图片格式
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //图片数据
    BYTE *bits(0);
    //图片指针
    FIBITMAP *image(0);
    //检查格式
    fif = FreeImage_GetFileType(filename, 0);
    if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filename);
    if(fif == FIF_UNKNOWN) {
        printf("[cImageInputLayer]Error: unknow image type!\n");
        return;
    }
    //载入图片
    if(FreeImage_FIFSupportsReading(fif))
        image = FreeImage_Load(fif, filename);
    if(!image) {
        printf("[cImageInputLayer]Error: cannot load image\n");
        return;
    }

    FIBITMAP *imagePtr = image;
    //检查是否PNG图片，是转换成32位
    if(fif == FIF_PNG) {
        imagePtr = FreeImage_ConvertTo32Bits(image);
    }
    //垂直翻转图片
    FreeImage_FlipVertical(imagePtr);
    //获取数据
    bits = FreeImage_GetBits(imagePtr);
    int width = FreeImage_GetWidth(imagePtr);
    int height = FreeImage_GetHeight(imagePtr);
    //
    if(width * height != inputNum){
        printf("[cImageInputLayer]Error: image size incorrect!\n");
        return;
    }
    //float res[height * width];
    for(int i = 0; i < width; ++i){
        for(int j = 0; j < height; ++j){
            unsigned char r = bits[(i * width * 4) + (j * 4) + 0];
            unsigned char g = bits[(i * width * 4) + (j * 4) + 1];
            unsigned char b = bits[(i * width * 4) + (j * 4) + 2];
            unsigned char a = bits[(i * width * 4) + (j * 4) + 3];
            output[j * width + i] = ((double(r) + double(g) + double(b)) / 3.0 / 255.0 * ((double)a / 255.0));
        }
    }
    FreeImage_Unload(image);
}
void cImageInputLayer::bPropagation(double *nextLayerError,double *preLayerOutput){
	printf("[cImageInputLayer]Error: bPropagation should not be used!\n");
	system("PAUSE");
	exit(-1);
}
