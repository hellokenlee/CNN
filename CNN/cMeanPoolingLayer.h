/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_MEANPOOLINGLAYER_H
#define C_MEANPOOLINGLAYER_H

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
*						 平均取样层
*
*************************************************************************/
#include "cLayer.h"
#include "mMath.h"
class cMeanPoolingLayer:public cLayer{
public:
	cMeanPoolingLayer(int inputNum,int outputNum,int batchSize,int inputR,int inputC,int poolR,int poolC);
	~cMeanPoolingLayer();
	cMeanPoolingLayer(const char* filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);


private:
	//禁止默认构造和拷贝
	cMeanPoolingLayer();
	cMeanPoolingLayer& operator=(cMeanPoolingLayer &);

	//输入矩阵(特征图)大小
	int inputR,inputC;
	//池化核大小
	int poolR,poolC;
	//输出矩阵大小
	int outputR,outputC;
	//error[batchSize][inputNum][inputR][inputC]
	//output[batchSize][outputNum][outputR][outputC]

	//scale=1/(pC*pR)
	double scale;
};

#endif
