/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_FULLYCONNECTEDLAYER_H
#define C_FULLYCONNECTEDLAYER_H

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
*						 全连接层
*
*************************************************************************/
#include "cLayer.h"


class cFullyConnectedLayer:public cLayer{
public:
	cFullyConnectedLayer(int inputNum,int outputNum,int batchSize,double learningRate);
	~cFullyConnectedLayer();
	cFullyConnectedLayer(const char* filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);

	void randomInit();
	double getLeariningRate();
protected:
	//禁止默认构造和拷贝
	cFullyConnectedLayer();
	cFullyConnectedLayer& operator=(cFullyConnectedLayer &);

	//学习率
	double learningRate;
	//权重 [inputNum][outputNum]
	double *weights;
	//累计残差 用于梯度下降
	//[inputNum][outputNum]
	double *De;
	//常数项 [batchSize][outputNum]
	double *bias;
	//常数项的梯度下降
	double *Be;
};
#endif
