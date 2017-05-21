#ifndef C_SQUARELOSSLAYER_H
#define C_SQUARELOSSLAYER_H

/************************************************************************
*
*							ƽ������ʧ��
*						( cost(i)=1/2*(h�� - y)^2 )
*************************************************************************/

#include "cLayer.h"
#include "ctime"

//��ʧ�㣺
//	- ���򴫲����ڶ�correct�𰸼������
//	- ���򴫲���Ӧ�ñ�ִ��
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
