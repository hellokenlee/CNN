/*ALL Copyright Reversed by KenLee@2015*/
#include "mMath.h"
/************************************************************************/
/*
/*						�����ѧ���㺯��
/*
/************************************************************************/

//�������ز���
void convolution(double *map,const int mR,const int mC,double *core,const int cR,const int cC,double *out){
	if(mR<cR||mC<cC){
		printf("[convolution] Error: input matrix is smaller than core matrix");
	}
	const int oC=mC-cC+1;
	const int oR=mR-cR+1;
	int sr=0,sc=0;//��������Ͻǵ�λ��
	//�ƶ���
	for(sr=0;sr<oR;++sr){
		for(sc=0;sc<oC;++sc){
			//�˷�֮�����
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

//��ת����180'
void mRotate(double *mat,const int mR,const int mC,double *out){
	int size=mR*mC;

	for(int i=0;i<size;++i){
		out[size-i-1]=mat[i];
	}
}

//mR x mC�ľ�����Χ������չ�� mR+2(pR-1) x mC+2(pC-1)�ľ���
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

//mR x mC�ľ���������Ϊ(mR*pR) x (mC*pC)�ľ���
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

//�����²���(����ƽ���²���)
void meanPolling(double *map,const int mR,const int mC,const int pR,const int pC,double *out){
	double scale=1.0/(double)(pC*pR);
	//Ŀ������С
	const int oR=(mR-1)/pR+1;
	const int oC=(mC-1)/pC+1;
	//���Ͻ�Ԫ���±�
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

//������� [a+b->b]
void mSum(double *a,double *b,int size){
	for(int i=0;i<size;++i){
		b[i]=a[i]+b[i];
	}
}

//��ӡ����
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

//��һ��num��ͼƬ(picR*picC)������ת����һ���м�����ת��
//pics [num][row][col]
//				   ����ͼƬ�� , ͼƬ��,  ͼƬ��С(picR*picC),
void nPics2TransMat(double *pics,const int num,const int picR,const int picC,
//				����˴�С(cR*cC), ������С(sR*sC),
					const int cR,const int cC,const int sR,const int sC,double *out){
	//�������ͼ�Ĵ�С
	int fR=(int)((double)(picR-cR)/(double)sR+0.5)+1;
	int fC=(int)((double)(picC-cC)/(double)sC+0.5)+1;
	//�����������Ϊ���������ͼ��R*C
	int mC=fR*fC;
	//���������Ϊ�����R*C*num
	int mR=cR*cC*num;
	//�������i��j��
	int i=0,j=0;
	int sr,sc;
	//����ÿһ��ͼƬ
	for(int p=0;p<num;++p){
		//�к����ߵĲ�����(r,c)
		for(int r=0;r<fR;++r){
			for (int c=0;c<fC;++c){
				  //��������Ͻ�Ԫ�ص�λ��(sr,sc)
				  sr=r*sR;sc=c*sC;
				  //������ڶ�Ӧÿһ��Ԫ��(x,y)
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

//					�м����,  ����˴�С(cR*cC),        ������С(sR*sC),
void matTransNpics(double* mat, const int cR,const int cC,int sR,int sC,
//				����ͼƬ�� , ͼƬ��,  ͼƬ��С(picR*picC),
				double *pics,const int num,const int picR,const int picC){
	//��ʼ��pics
	memset(pics,0,sizeof(double)*num*picR*picC);
	//�������ͼ�Ĵ�С
	int fR=(int)((double)(picR-cR)/(double)sR+0.5)+1;
	int fC=(int)((double)(picC-cC)/(double)sC+0.5)+1;
	//�����������Ϊ���������ͼ��R*C
	int mC=fR*fC;
	//���������Ϊ�����R*C*num
	int mR=cR*cC*num;
	//�������i��j��
	int i=0,j=0;
	int sr,sc;
	//����ÿһ��ͼƬ
	for(int p=0;p<num;++p){
		//�к����ߵĲ�����(r,c)
		for(int r=0;r<fR;++r){
			for (int c=0;c<fC;++c){
				  //��������Ͻ�Ԫ�ص�λ��(sr,sc)
				  sr=r*sR;sc=c*sC;
				  //������ڶ�Ӧÿһ��Ԫ��(x,y)
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
