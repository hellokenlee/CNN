/*ALL Copyright Reversed by KenLee@2015*/
#ifndef M_NATH_H
#define M_NATH_H

#include <cstdio>
#include <cstring>

/************************************************************************
*
*						�����ѧ���㺯��
*
*************************************************************************/

//��ά�����С
struct sMatrixSize{
	int row;
	int col;
	int size;
	sMatrixSize(){
		row=col=size=0;
	}
	sMatrixSize(int r,int c){
		row=r;
		col=c;
		size=r*c;
	};

};

//�������ز���
void convolution(double *map,const int mR,const int mC,double *core,const int cR,const int cC,double *out);

//��ת����180'
void mRotate(double *mat,const int mR,const int mC,double *out);

//mR x mC�ľ�����Χ������չ�� mR+2(pR-1) x mC+2(pC-1)�ľ���
void mFillZero(double *mat,int mR,int mC,int pR,int pC,double *out);

//mR x mC�ľ�����(pR x pC)��������ΪoR x oC�ľ���
void mExpand(double *mat,int mR,int mC,int pR,int pC,double *out,int oR,int oC);

//�����²���(����ƽ���²���)
void meanPolling(double *map,const int mR,const int mC,const int pR,const int pC,double *out);

//������� [a+b->b]
void mSum(double *a,double *b,int size);

//��ӡ����
void mPrint(double *a,int R,int C);

//��һ��num��ͼƬ(picR*picC)������ת����һ���м�����ת��
//pics [num][row][col]
//				   ����ͼƬ�� , ͼƬ��,  ͼƬ��С(picR*picC),
void nPics2TransMat(double *pics,const int num,const int picR,const int picC,
					//				����˴�С(cR*cC), ������С(sR*sC),
					const int cR,const int cC,const int sR,const int sC,double *out);

void matTransNpics(double* mat, const int cR,const int cC,int sR,int sC,
				   //				����ͼƬ�� , ͼƬ��,  ͼƬ��С(picR*picC),
				   double *pics,const int num,const int picR,const int picC);
#endif
