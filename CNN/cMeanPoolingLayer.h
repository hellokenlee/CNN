/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_MEANPOOLINGLAYER_H
#define C_MEANPOOLINGLAYER_H

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
*						 ƽ��ȡ����
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
	//��ֹĬ�Ϲ���Ϳ���
	cMeanPoolingLayer();
	cMeanPoolingLayer& operator=(cMeanPoolingLayer &);

	//�������(����ͼ)��С
	int inputR,inputC;
	//�ػ��˴�С
	int poolR,poolC;
	//��������С
	int outputR,outputC;
	//error[batchSize][inputNum][inputR][inputC]
	//output[batchSize][outputNum][outputR][outputC]

	//scale=1/(pC*pR)
	double scale;
};

#endif
