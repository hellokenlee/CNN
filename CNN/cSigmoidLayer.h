/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_SIGMOIDLAYER_H
#define C_SIGMOIDLAYER_H

#include <cstdio>
#include <cmath>
#include <ctime>

/************************************************************************
*
*						 sigmoid激活函数层
*
*************************************************************************/

#include "cLayer.h"

//f(z)=1/(1+e^(-z))
//其中 z 是向量
//sigmoid 层的输入数目一定等于输出数目
class cSigmoidLayer:public cLayer{
public:
	cSigmoidLayer(int inputNum,int outputNum,int batchSize=1);
	~cSigmoidLayer();
	cSigmoidLayer(const char *filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);

protected:
	//禁止默认构造和拷贝
	cSigmoidLayer();
	cSigmoidLayer& operator=(const cSigmoidLayer&);
};

#endif
