/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_MAXPOOLINGLAYER_H
#define C_MAXPOOLINGLAYER_H

#ifdef _MSC_VER // 适配Visual C++ 编译器
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // 适配GCC
    #include <cblas.h>
#endif // __GNUC__

#include <cstdlib>
#include <ctime>

/************************************************************************
*
*						 Max采样层
*
************************************************************************/
#include "cLayer.h"

struct sPos
{
	int x,y;
	sPos(){
		x=y=0;
	}
	sPos(int _x,int _y){
		x=_x;
		y=_y;
	}
};

class cMaxPoolingLayer:public cLayer{
public:
	cMaxPoolingLayer(int inputNum,int outputNum,int batchSize,int inputR,int inputC,int poolR,int poolC);
	~cMaxPoolingLayer();
	cMaxPoolingLayer(const char* filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);

protected:
	//禁止默认构造和拷贝构造
	cMaxPoolingLayer();
	cMaxPoolingLayer& operator=(cMaxPoolingLayer&);

	//输入矩阵大小
	int inputR,inputC;
	//池化矩阵大小
	int poolR,poolC;
	//输出矩阵大小
	int outputR,outputC;

	//记录最大的值的位置
	sPos *maxPosition;
	//output

};

#endif
