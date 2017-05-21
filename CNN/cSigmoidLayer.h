/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_SIGMOIDLAYER_H
#define C_SIGMOIDLAYER_H

#include <cstdio>
#include <cmath>
#include <ctime>

/************************************************************************
*
*						 sigmoid�������
*
*************************************************************************/

#include "cLayer.h"

//f(z)=1/(1+e^(-z))
//���� z ������
//sigmoid ���������Ŀһ�����������Ŀ
class cSigmoidLayer:public cLayer{
public:
	cSigmoidLayer(int inputNum,int outputNum,int batchSize=1);
	~cSigmoidLayer();
	cSigmoidLayer(const char *filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);

protected:
	//��ֹĬ�Ϲ���Ϳ���
	cSigmoidLayer();
	cSigmoidLayer& operator=(const cSigmoidLayer&);
};

#endif
