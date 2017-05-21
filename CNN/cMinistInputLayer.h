#ifndef C_MINISTINPUTLAYER_H
#define C_MINISTINPUTLAYER_H

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
*							Minist�����
*
*************************************************************************/

#include "cLayer.h"

/*
	���ڴ����ļ���д�����ݵĶ���
	������ǰ�򴫲����ǰ����ݶ�����Ȼ��ŵ�
	����㲻Ӧ���з��򴫲��Ĺ��̣����򴫲���Ӧ�ñ�ִ�е�
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
