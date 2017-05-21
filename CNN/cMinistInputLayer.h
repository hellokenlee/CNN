#ifndef C_MINISTINPUTLAYER_H
#define C_MINISTINPUTLAYER_H

#ifdef _MSC_VER // 适配Visual C++ 编译器
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // 适配GCC
    #include <cblas.h>
#endif // __GNUC__

#include <cstdlib>
#include <ctime>
/************************************************************************
*
*							Minist输入层
*
*************************************************************************/

#include "cLayer.h"

/*
	用于处理文件读写，数据的读入
	输入层的前向传播就是把数据读进来然后放到
	输入层不应该有反向传播的过程，反向传播不应该被执行到
*/
class cMinistInputLayer:public cLayer{
public:
	cMinistInputLayer(int inputNum,int NetworkOutputNum,int batchSize,char* filePath);
	~cMinistInputLayer();
	cMinistInputLayer(const char *filePath);
	void fPropagation(double *NetworkCorrect);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);

	char* getInputFilePath();
protected:
	cMinistInputLayer();
	cMinistInputLayer& operator=(cMinistInputLayer&);

	//change result[batchsize] -> answer[batchSize][10]
	void enCode(double *correct);

	FILE *file;
	char *filePath;
	char buf[128];
	bool isEOF;
	int num;
	int NetworkOutputNum;
	int *answer;
};
#endif
