#ifndef C_SQUARELOSSLAYER_H
#define C_SQUARELOSSLAYER_H

/************************************************************************
*
*							平方差损失层
*						( cost(i)=1/2*(hΘ - y)^2 )
*************************************************************************/

#include "cLayer.h"
#include "ctime"

//损失层：
//	- 反向传播用于对correct答案计算误差
//	- 正向传播不应该被执行
class cSquareLossLayer:public cLayer{
public:
	cSquareLossLayer(int inputNum,int batchSize);
	~cSquareLossLayer();
	cSquareLossLayer(const char *filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *NetworkCorrect,double *preLayerOutput);
	void saveLayer(const char *filePath);

private:

};

#endif
