/*ALL Copyright Reversed by KenLee@2015*/
#include "mMath.h"
/************************************************************************/
/*
/*						相关数学运算函数
/*
/************************************************************************/

//卷积的相关操作
void convolution(double *map,const int mR,const int mC,double *core,const int cR,const int cC,double *out){
	if(mR<cR||mC<cC){
		printf("[convolution] Error: input matrix is smaller than core matrix");
	}
	const int oC=mC-cC+1;
	const int oR=mR-cR+1;
	int sr=0,sc=0;//卷积框左上角的位置
	//移动框
	for(sr=0;sr<oR;++sr){
		for(sc=0;sc<oC;++sc){
			//乘法之后相加
			double sum=0;
			for(int i=0;i<cR;++i){
				for(int j=0;j<cC;++j){
					sum+=map[(sr+i)*mC+(sc+j)]*core[i*cC+j];
				}
			}
			out[sr*oC+sc]=sum;
		}
	}

}

//旋转矩阵180'
void mRotate(double *mat,const int mR,const int mC,double *out){
	int size=mR*mC;

	for(int i=0;i<size;++i){
		out[size-i-1]=mat[i];
	}
}

//mR x mC的矩阵周围补零扩展成 mR+2(pR-1) x mC+2(pC-1)的矩阵
void mFillZero(double *mat,int mR,int mC,int pR,int pC,double *out){
	int oR=mR+2*(pR-1);
	int oC=mC+2*(pC-1);
	memset(out,0,sizeof(double)*oR*oC);
	for(int i=0;i<mR;++i){
		for(int j=0;j<mC;++j){
			out[(pR-1+i)*oC+(pC-1+j)]=mat[i*mC+j];
		}
	}
}

//mR x mC的矩阵复制扩充为(mR*pR) x (mC*pC)的矩阵
void mExpand(double *mat,int mR,int mC,int pR,int pC,double *out,int oR,int oC){
	for(int r=0;r<mR;++r){
		for(int c=0;c<mC;++c){
			for(int i=0;i<pR;++i){
				for(int j=0;j<pC;++j){
					if(r*pR+i<oR && c*pC+j<oC)
						out[(r*pR+i)*oC+(c*pC+j)]=mat[r*mC+c];
				}
			}
		}
	}
}

//计算下采样(采用平均下采样)
void meanPolling(double *map,const int mR,const int mC,const int pR,const int pC,double *out){
	double scale=1.0/(double)(pC*pR);
	//目标矩阵大小
	const int oR=(mR-1)/pR+1;
	const int oC=(mC-1)/pC+1;
	//左上角元素下标
	int sr,sc;
	int _or=0,_oc=0;
	for(sr=0;sr<mR;sr+=pR){
		for(sc=0;sc<mC;sc+=pC){
			double sum=0;
			for(int i=0;i<pR;++i){
				for(int j=0;j<pC;++j){
					if(sr+i>=mR || sc+j>=mC)
						sum+=0;
					else
						sum+=map[(sr+i)*mC+(sc+j)]*scale;
				}
			}
			out[_or*oC+_oc]=sum;
			_oc++;
		}
		_or++;
		_oc=0;
	}
}

void maxPolling(double *map,const int mR,const int mC,const int pR,const int pC,double *out,double *position){

}

//向量相加 [a+b->b]
void mSum(double *a,double *b,int size){
	for(int i=0;i<size;++i){
		b[i]=a[i]+b[i];
	}
}

//打印矩阵
void mPrint(double *a,int R,int C){
	printf("matrix %dx%d:\n",R,C);
	for (int i=0;i<R;++i){
		for(int j=0;j<C;++j){
			printf("%lf ",a[i*C+j]);
		}
		printf("\n");
	}
	printf("\n");
}

//把一个num张图片(picR*picC)的数组转换成一个中间矩阵的转置
//pics [num][row][col]
//				   输入图片组 , 图片数,  图片大小(picR*picC),
void nPics2TransMat(double *pics,const int num,const int picR,const int picC,
//				卷积核大小(cR*cC), 步进大小(sR*sC),
					const int cR,const int cC,const int sR,const int sC,double *out){
	//输出特征图的大小
	int fR=(int)((double)(picR-cR)/(double)sR+0.5)+1;
	int fC=(int)((double)(picC-cC)/(double)sC+0.5)+1;
	//输出矩阵列数为卷积后特征图的R*C
	int mC=fR*fC;
	//输出矩阵行为卷积核R*C*num
	int mR=cR*cC*num;
	//输出矩阵i行j列
	int i=0,j=0;
	int sr,sc;
	//对于每一张图片
	for(int p=0;p<num;++p){
		//行和列走的步数是(r,c)
		for(int r=0;r<fR;++r){
			for (int c=0;c<fC;++c){
				  //卷积框左上角元素的位置(sr,sc)
				  sr=r*sR;sc=c*sC;
				  //卷积框内对应每一个元素(x,y)
				  for(int x=0;x<cR;++x){
					  for(int y=0;y<cC;++y){
						  int t1=p*picC*picR;
						  int t2=(sr+x);
						  if(sr+x>=picR||sc+y>=picC){
							   out[i*mC+j]=0;
						  }else{
							   out[i*mC+j]=pics[p*picC*picR+(sr+x)*picC+(sc+y)];
						  }
						  i++;
					  }
				  }
				  i-=cR*cC;
				  j++;
			}
		}
		i+=cR*cC;
		j-=mC;
	}
}

//					中间矩阵,  卷积核大小(cR*cC),        步进大小(sR*sC),
void matTransNpics(double* mat, const int cR,const int cC,int sR,int sC,
//				输入图片组 , 图片数,  图片大小(picR*picC),
				double *pics,const int num,const int picR,const int picC){
	//初始化pics
	memset(pics,0,sizeof(double)*num*picR*picC);
	//输出特征图的大小
	int fR=(int)((double)(picR-cR)/(double)sR+0.5)+1;
	int fC=(int)((double)(picC-cC)/(double)sC+0.5)+1;
	//输出矩阵列数为卷积后特征图的R*C
	int mC=fR*fC;
	//输出矩阵行为卷积核R*C*num
	int mR=cR*cC*num;
	//输出矩阵i行j列
	int i=0,j=0;
	int sr,sc;
	//对于每一张图片
	for(int p=0;p<num;++p){
		//行和列走的步数是(r,c)
		for(int r=0;r<fR;++r){
			for (int c=0;c<fC;++c){
				  //卷积框左上角元素的位置(sr,sc)
				  sr=r*sR;sc=c*sC;
				  //卷积框内对应每一个元素(x,y)
				  for(int x=0;x<cR;++x){
					  for(int y=0;y<cC;++y){
						  int t1=p*picC*picR;
						  int t2=(sr+x);
						  if(sr+x<picR&&sc+y<picC)
							   pics[p*picC*picR+(sr+x)*picC+(sc+y)]+=mat[i*mC+j];
						  i++;
					  }
				  }
				  i-=cR*cC;
				  j++;
			}
		}
		i+=cR*cC;
		j-=mC;
	}
}
