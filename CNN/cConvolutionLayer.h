/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_CONVOLUTIONLAYER_H
#define C_CONVOLUTIONLAYER_H

#ifdef _MSC_VER // 适配Visual C++ 编译器
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // 适配GCC
    #include <cblas.h>
#endif // __GNUC__

#include <ctime>

/************************************************************************
*
*						  卷积层
*
*************************************************************************/
#include "mMath.h"
#include "cLayer.h"

//输入应该是 [batchSize][inputNum][inputRow][inputCol]
//的4维矩阵
//其中 访问 在批中的第j个的第i个输入矩阵应该是 start=i*(batchSize*inputRow*inputCol)+j*(inputRow*inputCol)
//												for(r=0;r<inputRow;++r)
//													for(c=0;c<inputCol;++c)
//														input[start+r*inputCol+c]->就是这个矩阵的r行c列的元素
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
	//禁止默认构造和拷贝
	cConvolutionLayer();
	cConvolutionLayer& operator=(cConvolutionLayer &);

	//输入矩阵个数 inputNum
	//输入矩阵大小
	int inputR,inputC;
	//卷积组个数 outputNum
	//卷积组中的卷积核个数 inputNum
	//卷积核大小
	int convR,convC;
	//卷积核数组 [outputNum][inputNum][convR][convC]
	double *convCores;
	//输出矩阵大小
	//int outR,outC;

	//卷积的步进长度
	int stepR,stepC;

	//输出的特征图大小 aka. 输出矩阵大小
	int fmR,fmC;


	//学习率
	double learningRate;
	/***中间变量***/
	//利用矩阵乘法算卷积
	//需要的midMatrix是需要**m ，因为对批处理暂存 [batchSize]个midMatrix
	double **midMatrix;
	double *midError;
	double *De;
	double *tempDe;
};

#endif
