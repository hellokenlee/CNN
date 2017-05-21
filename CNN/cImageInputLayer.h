/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef C_IMAGEINPUTLAYER_H
#define C_IMAGEINPUTLAYER_H

#ifdef _MSC_VER // 适配Visual C++ 编译器
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // 适配GCC
    #include <cblas.h>
#endif // __GNUC__

#include <cstdlib>
#include <ctime>
#include <FreeImage.h>

/************************************************************************
*
*							单张图片输入层
*
*************************************************************************/

#include "cLayer.h"
class cImageInputLayer:public cLayer{
public:
	cImageInputLayer(int inputNum,int NetworkOutputNum,int batchSize);
	~cImageInputLayer();

	cImageInputLayer(const char *filePath);
	void saveLayer(const char *filePath);

	void getDataFromImage(const char* filePath);

	void fPropagation(double *NetworkCorrect);

	void bPropagation(double *nextLayerError,double *preLayerOutput);// Not define
protected:
	cImageInputLayer();// Not define
	cImageInputLayer& operator=(cImageInputLayer&);// Not define
	//change result[batchsize] -> answer[batchSize][10]
    int* answer;
};
#endif
