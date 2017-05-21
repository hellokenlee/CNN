/*ALL Copyright Reversed by KenLee@2015*/
#include "cFullyConnectedLayer.h"

#define DEBUGn
#define BIAS
cFullyConnectedLayer::cFullyConnectedLayer(int _inputNum,int _outputNum,int _batchSize,double _learningRate)
	:cLayer(_inputNum,_outputNum,_batchSize){
	learningRate = _learningRate;
	layerType="FullyConnectedLayer";

	inputSize=_inputNum;
	outputSize=_outputNum;
	//输出
	output = new double[batchSize*outputNum];

	//残差
	error = new double[batchSize*inputNum];
	//权值
	weights =  new double[inputNum*outputNum];
	//累计误差
	De = new double[inputNum*outputNum];
	//常数项
	bias = new double[outputNum];
	//常数项梯度
	Be = new double[outputNum];
	randomInit();
}
//从文件中构造
cFullyConnectedLayer::cFullyConnectedLayer(const char* filePath)
	:cLayer(inputNum,outputNum,batchSize){
	FILE *f;
	char buff[128];
	f=fopen(filePath,"r");
	fscanf(f,"%s %s %s %s",buff,buff,buff,buff);

	fscanf(f,"%s %d",buff,&inputNum);
	fscanf(f,"%s %d",buff,&outputNum);
	fscanf(f,"%s %d",buff,&batchSize);
	fscanf(f,"%s %lf",buff,&learningRate);

	layerType="FullyConnectedLayer";

	inputSize=inputNum;
	outputSize=outputNum;
	//输出
	output = new double[batchSize*outputNum];

	//残差
	error = new double[batchSize*inputNum];
	//权值
	weights =  new double[inputNum*outputNum];
	//累计误差
	De = new double[inputNum*outputNum];
	//常数项
	bias = new double[outputNum];
	//常数项梯度
	Be = new double[outputNum];

	//读入权值
	fscanf(f,"%s",buff);
	for (int r=0;r<inputNum;++r){
		for(int c=0;c<outputNum;++c){
			fscanf(f,"%lf ",&weights[r*outputNum+c]);
		}
	}
	//读取偏差
	fscanf(f,"%s",buff);
	for(int i=0;i<outputNum;++i){
		fscanf(f,"%lf ",&bias[i]);
	}
	
	fclose(f);
}

cFullyConnectedLayer::~cFullyConnectedLayer(){
	delete[] weights;
	delete[] De;
	delete[] output;
	delete[] error;
	delete[] bias;
	delete[] Be;
}

void cFullyConnectedLayer::randomInit(){
	srand((unsigned)time(NULL));
#ifdef DEBUG
	srand(123);
#endif
	//srand(123);
	//对于权值初始化,权值范围 : -0.1~0.1
	/*
	for(int i=0;i<outputNum*inputNum;++i){
		weights[i]=(double)(rand()%10000-5000)/50000.0;
	}*/
	for(int i=0;i<outputNum;++i){
		for(int j=0;j<inputNum;++j){	
			//产生-0.1~0.1随机数
			srand((unsigned)i*10+j);
			weights[j*outputNum+i]=(double)(rand()%1000-500)/5000.0;
		}
	}
#ifdef DEBUG
	printf("weights:");
	mPrint(weights,inputNum,outputNum);
#endif
	//初始化bias为 -0.1~0.1
	for(int i=0;i<outputNum;++i){
		srand((unsigned)i);
		bias[i]=(double)(rand()%1000-500)/5000.0;
	}
}

void cFullyConnectedLayer::fPropagation(double *preLayerOutput){

	//计算 z=Θ*input
	//我们把input当作一行而非一列
	//按批处理 计算 z=θ*input(Z^T=input^T *θ^T)           [batchSize][inputNum] x [inputNum][outputNum]
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,batchSize,outputNum,inputNum,1,
		preLayerOutput,inputNum, weights,outputNum, 0,output,outputNum);
#ifdef BIAS
	//输出加上bias项
	//output: [batchsize][outputNum]
	//output=output+bias
	

	for(int b=0;b<batchSize;++b){
		int row=b*outputNum;
		for (int i=0;i<outputNum;++i){
			output[row+i] = output[row+i]+bias[i];
		}
	}

#endif // BIAS


}

void cFullyConnectedLayer::bPropagation(double *nextLayerError,double *preLayerOutput){

	//更新残差
	//计算 e_j=w^T *e_(j+1)							e_(j+1)						w^T						e_j
	//事实上我们计算 e_j^T=e_(j+1)^T*(w^T)^T  [batchSize][outputNum] x ([inputNum][outputNum])^T = [batchsize][inputNum]
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,batchSize,inputNum,outputNum,1,
		nextLayerError,outputNum, weights,outputNum, 0,error,inputNum);
#ifdef DEBUG

	printf("FCLerror:\n");
	mPrint(error,batchSize,inputNum);
#endif
	//计算并累计偏差 De=e_(j+1)*a_j^T					out_(j-1)^T					e_(j+i)					De_j
	//事实上我们计算 De^T=(a_j^T)^T * e_(j+1)^T  ([batchSize][inputNum])^T *([batchSize][outputNum]) = [inputNum][outputNum]
	cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans,inputNum,outputNum,batchSize,1,
		preLayerOutput,inputNum, nextLayerError,outputNum, 0,De,outputNum);

	//更新权值 w=w-learningRate*aver(De)
	//	De=learningRate*aver(De)
	cblas_dscal(inputNum*outputNum, learningRate/(double)batchSize, De, 1);
	//	w=-De+w
	cblas_daxpy(inputNum*outputNum,-1, De,1, weights, 1);

#ifdef BIAS
	//对 bias的梯度(下一层的残差) 求平均
	memset(Be,0,sizeof(double)*outputNum);
	for(int b=0;b<batchSize;++b){
		//加第i批的残差的和
		//Be=e_(j+1)[b]+Be
		cblas_daxpy(outputNum,1,nextLayerError+(b*outputNum),1,Be,1);
	}
	//对bias求平均！还有乘学习率
	cblas_dscal(outputNum,learningRate/(double)batchSize , Be, 1);
	//更新bias
	//bias=-Be+bias
	cblas_daxpy(outputNum ,-1, Be,1, bias, 1);
#endif // BIAS


}

void cFullyConnectedLayer::saveLayer(const char *filePath){
	FILE *f;
	f=fopen(filePath,"w");
	time_t t=time(NULL);
	tm *date=localtime(&t);
	fprintf(f,"%s  modify_at ",layerType.c_str());
	fprintf(f,"%d-%d-%d %d:%d:%d\n\n",date->tm_year+1990,date->tm_mon+1,date->tm_mday,date->tm_hour,date->tm_min,date->tm_sec);

	fprintf(f,"InputNum %d\n",inputNum);
	fprintf(f,"OutputNum %d\n",outputNum);
	fprintf(f,"BatchSize %d\n",batchSize);
	fprintf(f,"LearningRate %lf\n",learningRate);

	fprintf(f,"Weights:\n");
	for (int r=0;r<inputNum;++r){
		for(int c=0;c<outputNum;++c){
			fprintf(f,"%lf ",weights[r*outputNum+c]);
		}
		fprintf(f,"\n");
	}

	fprintf(f,"Bias:\n");
	for(int i=0;i<outputNum;++i){
		fprintf(f,"%lf ",bias[i]);
	}
	fclose(f);
}

double cFullyConnectedLayer::getLeariningRate(){
	return learningRate;
}