/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_LAYER_H
#define C_LAYER_H

#ifdef _MSC_VER // 适配Visual C++ 编译器
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // 适配GCC
    #include <cblas.h>
#endif // __GNUC__

#include <string>
/************************************************************************
*
*							层抽象类
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
	//禁止调用默认构造函数
	cLayer();
	cLayer& operator=(const cLayer&);

	//输入大小	aka. 上一层的输出
	int inputNum;
	//输出大小	aka. 下一层的输入
	int outputNum;
	//批大小
	int batchSize;
	//该层的输出 [outputNum][batchSize]
	double *output;
	//该层的残差 [input][batchSize]
	double *error;
	//名字
	std::string layerType;
	//输入大小 对于输入一维inputSize=inputNum,对于多维 inputSize=inputNum*sizeof(多维矩阵)
	int inputSize;
	int outputSize;
};


#endif
