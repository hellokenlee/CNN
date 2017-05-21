/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_CONVOLUTIONLAYER_H
#define C_CONVOLUTIONLAYER_H

#ifdef _MSC_VER // ����Visual C++ ������
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // ����GCC
    #include <cblas.h>
#endif // __GNUC__

#include <ctime>

/************************************************************************
*
*						  �����
*
*************************************************************************/
#include "mMath.h"
#include "cLayer.h"

//����Ӧ���� [batchSize][inputNum][inputRow][inputCol]
//��4ά����
//���� ���� �����еĵ�j���ĵ�i���������Ӧ���� start=i*(batchSize*inputRow*inputCol)+j*(inputRow*inputCol)
//												for(r=0;r<inputRow;++r)
//													for(c=0;c<inputCol;++c)
//														input[start+r*inputCol+c]->������������r��c�е�Ԫ��
class cConvolutionLayer:public cLayer{
public:
	cConvolutionLayer(int inputNum,int outputNum,int batchSize,int inputR,int inputC,int convR,int convC,int stepR,int stepC,double learningRate=0.3);
	~cConvolutionLayer();
	cConvolutionLayer(const char* filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);

	void randomInit();
	double* getConvCores();
	void setConvCores(double *cc);
private:
	//��ֹĬ�Ϲ���Ϳ���
	cConvolutionLayer();
	cConvolutionLayer& operator=(cConvolutionLayer &);

	//���������� inputNum
	//��������С
	int inputR,inputC;
	//�������� outputNum
	//������еľ���˸��� inputNum
	//����˴�С
	int convR,convC;
	//��������� [outputNum][inputNum][convR][convC]
	double *convCores;
	//��������С
	//int outR,outC;

	//����Ĳ�������
	int stepR,stepC;

	//���������ͼ��С aka. ��������С
	int fmR,fmC;


	//ѧϰ��
	double learningRate;
	/***�м����***/
	//���þ���˷�����
	//��Ҫ��midMatrix����Ҫ**m ����Ϊ���������ݴ� [batchSize]��midMatrix
	double **midMatrix;
	double *midError;
	double *De;
	double *tempDe;
};

#endif
