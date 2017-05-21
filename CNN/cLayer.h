/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_LAYER_H
#define C_LAYER_H

#ifdef _MSC_VER // ����Visual C++ ������
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // ����GCC
    #include <cblas.h>
#endif // __GNUC__

#include <string>
/************************************************************************
*
*							�������
*
*************************************************************************/
#include "mMath.h"
class cLayer{
public:
	cLayer(int inputNum,int outputNum,int batchSize=1);
	virtual ~cLayer();
	virtual void fPropagation(double *preLayerOutput)=0;
	virtual void bPropagation(double *nextLayerError,double *preLayerOutput)=0;
	virtual void saveLayer(const char *filePath)=0;


	double* getOutput();
	double* getError();
	int getInputNum();
	int getOutputNum();
	int getBatchSize();
	int getOutputSize();
	int getInputSize();
	std::string getName();


protected:
	//��ֹ����Ĭ�Ϲ��캯��
	cLayer();
	cLayer& operator=(const cLayer&);

	//�����С	aka. ��һ������
	int inputNum;
	//�����С	aka. ��һ�������
	int outputNum;
	//����С
	int batchSize;
	//�ò����� [outputNum][batchSize]
	double *output;
	//�ò�Ĳв� [input][batchSize]
	double *error;
	//����
	std::string layerType;
	//�����С ��������һάinputSize=inputNum,���ڶ�ά inputSize=inputNum*sizeof(��ά����)
	int inputSize;
	int outputSize;
};


#endif
