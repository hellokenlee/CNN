/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_NETWORK_H
#define C_NETWORK_H

#include <vector>
#include <cstring>
#include <string>
#include <ctime>
#include <iostream>

#ifdef _MSC_VER // ����Visual C++ ������
    #include <direct.h>
#endif // _MSC_VER

#ifdef __GNUC__ // ����GCC
    #include <sys/stat.h>
#endif // __GNUC__


using namespace std;

/************************************************************************
*
*							��������
*
************************************************************************/

#include "cLayer.h"
#include "cSigmoidLayer.h"
#include "cFullyConnectedLayer.h"
#include "cConvolutionLayer.h"
#include "cMaxPoolingLayer.h"
#include "cMeanPoolingLayer.h"
#include "cMinistInputLayer.h"
#include "cSquareLossLayer.h"
#include "cImageInputLayer.h"

class cNetwork{
public:
	cNetwork(int inputNum,int outputNum,int batchSize,double learningRate);
	cNetwork(const char *filePath);
	~cNetwork();

	void fPropagation();
	void bPropagation();
	void saveNetwork(const char* filePath);
	//����һ��
	void addLayer(cLayer *L);
	//ѵ���Լ�����
	void trainMinist(char* trainFile,int times,int display);
	double testMisist(char* testFile,int times);
    int testSingleImage(const char* testFile);
	//Getters
	double *getOutput();
	double *getInput();
	double *getError();
	double *getCorrect();
	int getBatchSize();
	//��ӡ���
	void printError();
private:
	//��ֹĬ�Ϲ���Ϳ���
	cNetwork();
	cNetwork& operator=(const cNetwork&);
	//��out[batchSize][10]->guess[batchSize]
	void deCode(double *out,int *guess);


	int batchSize;
	int inputNum;
	int outputNum;
	double learningRate;
	vector<cLayer*> layers;
	double *input;
	double *output;
	double *error;
	double *correct;
};

#endif
