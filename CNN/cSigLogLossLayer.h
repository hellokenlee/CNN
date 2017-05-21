#ifndef C_SIGLOGLOSSLAYER_H
#define C_SIGLOGLOSSLAYER_H

#include "cLayer.h"

/************************************************************************
*
*						 sigmoid损失函数层
*
*************************************************************************/

class cSigLogLossLayer:public cLayer{
public:
	cSigLogLossLayer(int inputNum,int outputNum,int batchSize);
	~cSigLogLossLayer();
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);
protected:
	//正确答案
	double *correct;
	//output: 网络的输出
	//error: 残差
};
#endif
