/*ALL Copyright Reversed by KenLee@2015*/
#ifndef M_NATH_H
#define M_NATH_H

#include <cstdio>
#include <cstring>

/************************************************************************
*
*						相关数学运算函数
*
*************************************************************************/

//二维矩阵大小
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

//卷积的相关操作
void convolution(double *map,const int mR,const int mC,double *core,const int cR,const int cC,double *out);

//旋转矩阵180'
void mRotate(double *mat,const int mR,const int mC,double *out);

//mR x mC的矩阵周围补零扩展成 mR+2(pR-1) x mC+2(pC-1)的矩阵
void mFillZero(double *mat,int mR,int mC,int pR,int pC,double *out);

//mR x mC的矩阵用(pR x pC)复制扩充为oR x oC的矩阵
void mExpand(double *mat,int mR,int mC,int pR,int pC,double *out,int oR,int oC);

//计算下采样(采用平均下采样)
void meanPolling(double *map,const int mR,const int mC,const int pR,const int pC,double *out);

//向量相加 [a+b->b]
void mSum(double *a,double *b,int size);

//打印矩阵
void mPrint(double *a,int R,int C);

//把一个num张图片(picR*picC)的数组转换成一个中间矩阵的转置
//pics [num][row][col]
//				   输入图片组 , 图片数,  图片大小(picR*picC),
void nPics2TransMat(double *pics,const int num,const int picR,const int picC,
					//				卷积核大小(cR*cC), 步进大小(sR*sC),
					const int cR,const int cC,const int sR,const int sC,double *out);

void matTransNpics(double* mat, const int cR,const int cC,int sR,int sC,
				   //				输入图片组 , 图片数,  图片大小(picR*picC),
				   double *pics,const int num,const int picR,const int picC);
#endif
