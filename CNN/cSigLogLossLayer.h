#ifndef C_SIGLOGLOSSLAYER_H
#define C_SIGLOGLOSSLAYER_H

#include "cLayer.h"

/************************************************************************
*
*						 sigmoid��ʧ������
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
	//��ȷ��
	double *correct;
	//output: ��������
	//error: �в�
};
#endif
