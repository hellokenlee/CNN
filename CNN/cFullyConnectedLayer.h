/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_FULLYCONNECTEDLAYER_H
#define C_FULLYCONNECTEDLAYER_H

#ifdef _MSC_VER // ����Visual C++ ������
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // ����GCC
    #include <cblas.h>
#endif // __GNUC__

#include <cstdlib>
#include <ctime>
/************************************************************************
*
*						 ȫ���Ӳ�
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
	//��ֹĬ�Ϲ���Ϳ���
	cFullyConnectedLayer();
	cFullyConnectedLayer& operator=(cFullyConnectedLayer &);

	//ѧϰ��
	double learningRate;
	//Ȩ�� [inputNum][outputNum]
	double *weights;
	//�ۼƲв� �����ݶ��½�
	//[inputNum][outputNum]
	double *De;
	//������ [batchSize][outputNum]
	double *bias;
	//��������ݶ��½�
	double *Be;
};
#endif
