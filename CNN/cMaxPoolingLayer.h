/*ALL Copyright Reversed by KenLee@2015*/
#ifndef C_MAXPOOLINGLAYER_H
#define C_MAXPOOLINGLAYER_H

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
*						 Max������
*
************************************************************************/
#include "cLayer.h"

struct sPos
{
	int x,y;
	sPos(){
		x=y=0;
	}
	sPos(int _x,int _y){
		x=_x;
		y=_y;
	}
};

class cMaxPoolingLayer:public cLayer{
public:
	cMaxPoolingLayer(int inputNum,int outputNum,int batchSize,int inputR,int inputC,int poolR,int poolC);
	~cMaxPoolingLayer();
	cMaxPoolingLayer(const char* filePath);
	void fPropagation(double *preLayerOutput);
	void bPropagation(double *nextLayerError,double *preLayerOutput);
	void saveLayer(const char *filePath);

protected:
	//��ֹĬ�Ϲ���Ϳ�������
	cMaxPoolingLayer();
	cMaxPoolingLayer& operator=(cMaxPoolingLayer&);

	//��������С
	int inputR,inputC;
	//�ػ������С
	int poolR,poolC;
	//��������С
	int outputR,outputC;

	//��¼����ֵ��λ��
	sPos *maxPosition;
	//output

};

#endif
