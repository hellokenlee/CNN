/*ALL Copyright Reversed by KenLee@2015*/

#include "cConvolutionLayer.h"
#define DEBUGn
//构造
cConvolutionLayer::cConvolutionLayer(int _inputNum,int _outputNum,int _batchSize,
									 int _inputR,int _inputC,int _convR,int _convC,int _stepR,int _stepC,
									 double _learningRate)
	:cLayer(_inputNum,_outputNum,_batchSize){

	layerType="ConvolutionalLayer";
	
	
	
	inputR=_inputR;
	inputC=_inputC;

	convR=_convR;
	convC=_convC;

	learningRate=_learningRate;

	stepR=_stepR;
	stepC=_stepC;

	//输出每一张特征图大小
	fmR=(int)((double)(inputR-convR)/(double)stepR+0.5)+1;
	fmC=(int)((double)(inputC-convC)/(double)stepC+0.5)+1;

	//input [batchSize][inputNum][inputR][inputC]
	convCores=new double[outputNum*(inputNum*convR*convC)];
	output=new double[batchSize*(outputNum)*(fmR*fmC)]; 
	error=new double[batchSize*inputNum*(inputR*inputC)];


	//初始化中间变量
	//批中每一个输入和输出占的大小
	inputSize=inputNum*inputR*inputC;
	outputSize=(outputNum)*(fmR*fmC);
	//把输入转成一个2维矩阵,缓存输入
	midMatrix=new double*[batchSize];
	for (int b=0;b<batchSize;++b){
		midMatrix[b]=new double[(inputNum*convR*convC)*(fmR*fmC)];
	}
	//残差中间矩阵
	midError=new double[(inputNum*convR*convC)*(fmR*fmC)];
	//梯度
	De=new double[outputNum*(inputNum*convR*convC)];
	tempDe=new double[outputNum*(inputNum*convR*convC)];

	randomInit();
}
//从文件中构造
cConvolutionLayer::cConvolutionLayer(const char *filePath)
	:cLayer(inputNum,outputNum,batchSize){
	FILE *f;
	char buff[128];
	f=fopen(filePath,"r");
	fscanf(f,"%s %s %s %s",buff,buff,buff,buff);

	fscanf(f,"%s %d",buff,&inputNum);
	fscanf(f,"%s %d",buff,&outputNum);
	fscanf(f,"%s %d",buff,&batchSize);
	fscanf(f,"%s %lf",buff,&learningRate);

	fscanf(f,"%s %d %d",buff,&inputR,&inputC);
	fscanf(f,"%s %d %d",buff,&convR,&convC);
	fscanf(f,"%s %d %d",buff,&stepR,&stepC);

	layerType="ConvolutionalLayer";
	//输出每一张特征图大小
	fmR=(int)((double)(inputR-convR)/(double)stepR+0.5)+1;
	fmC=(int)((double)(inputC-convC)/(double)stepC+0.5)+1;

	//input [batchSize][inputNum][inputR][inputC]
	convCores=new double[outputNum*(inputNum*convR*convC)];
	output=new double[batchSize*(outputNum)*(fmR*fmC)]; 
	error=new double[batchSize*inputNum*(inputR*inputC)];


	//初始化中间变量
	//批中每一个输入和输出占的大小
	inputSize=inputNum*inputR*inputC;
	outputSize=(outputNum)*(fmR*fmC);
	//把输入转成一个2维矩阵,缓存输入
	midMatrix=new double*[batchSize];
	for (int b=0;b<batchSize;++b){
		midMatrix[b]=new double[(inputNum*convR*convC)*(fmR*fmC)];
	}
	//残差中间矩阵
	midError=new double[(inputNum*convR*convC)*(fmR*fmC)];
	//梯度
	De=new double[outputNum*(inputNum*convR*convC)];
	tempDe=new double[outputNum*(inputNum*convR*convC)];

	//读入卷积核
	for(int o=0;o<outputNum;++o){
		//每一组卷积核
		fscanf(f,"%s",buff);
		for(int i=0;i<inputNum;++i){
			//每一个卷积核
			for(int r=0;r<convR;++r){
				for(int c=0;c<convC;++c){
					fscanf(f,"%lf ",&convCores[o*(inputNum*convR*convC)+i*(convR*convC)+r*convC+c]);
				}
			}
		}
	}
	fclose(f);
}
//析构
cConvolutionLayer::~cConvolutionLayer(){

	for(int b=0;b<batchSize;++b){
		delete []midMatrix[b];
	}
	delete[]midMatrix;
	delete[] convCores;
	delete[] output;
	delete[] error;
	delete[] midError;
	delete[] De;
	delete[] tempDe;
}

void cConvolutionLayer::randomInit(){
	srand((unsigned)time(NULL));
	for(int i=0;i<outputNum*(inputNum*convR*convC);++i){
		convCores[i]=(double)(rand()%1000-500)/5000.0;
	}
}
//前向传播，对于每一个输入的矩阵做(相关计算)卷积计算出输出
//preLayerOutput [batchSize][inputNum][inputR][inputC]
void cConvolutionLayer::fPropagation(double *preLayerOutput){

	//对于一批中的每一个做处理
	for(int b=0;b<batchSize;++b){
		//input[b]->midMatrix
		//利用卷积核计算中间矩阵
		nPics2TransMat(preLayerOutput+b*inputSize,inputNum,inputR,inputC,convR,convC,stepR,stepC,midMatrix[b]);

		//output[b]=convCores * midMatrix
		//利用矩阵乘法计算卷积
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,outputNum,(fmR*fmC),(inputNum*convR*convC),1,
			convCores,(inputNum*convR*convC), midMatrix[b],(fmR*fmC), 0,output+b*outputSize,(fmR*fmC));
	}

#ifdef DEBUG
	printf("output:\n");
	for(int b=0;b<batchSize;++b){
		
		for (int o=0;o<outputNum;o++){
			printf("%dnum %dth output:\n",b,o);
			mPrint(&output[b*outputSize+o*fmR*fmC],fmR,fmC);
		}
		/*
		printf("%dth output:\n",b);
		mPrint(output+b*outputSize,outputNum,fmR*fmC);
		*/
	}
#endif // DEBUG
}

//				nLE:[batchSize][outputNum][outputR][outputC]  pLO:[batchSize][inputNum][inputR][inputC]
void cConvolutionLayer::bPropagation(double *nextLayerError,double *preLayerOutput){
	//初始化梯度
	memset(De,0,sizeof(double)*outputNum*(inputNum*convR*convC));

	//对于一批更新残差
	for(int b=0;b<batchSize;++b){
		//算出误差中间矩阵
		//计算e_j=w^T*e_(j+1)
		//w=convCores: [outputNum][inputNum*convR*convC]
		//e_(j+1)[b]: [outputNum][outputR*outputC] 
		//e_j[b]: [inputNum*convR*convC][outputR*outputC]
		cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans,inputNum*convR*convC,fmR*fmC,outputNum,1,
			convCores,inputNum*convR*convC, &nextLayerError[b*(outputSize)],fmR*fmC, 0,midError,fmR*fmC);

		//根据中间矩阵计算误差
		matTransNpics(midError,convR,convC,stepR,stepC,&error[b*inputSize],inputNum,inputR,inputC);

		//利用下一层的残差计算梯度求和
		//De+=e_(j+1)*a_j^T
		//	 =e_(j+1)*midMatrix^T                  [outputNum][fmR*fmC] x [inputNum*convR*convC][fmR*fmC]^T
		//		1.tempDe=e_(j+1)[b]*midMatrix^T
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,outputNum,inputNum*convR*convC,fmR*fmC,1,
			&nextLayerError[b*(outputSize)],fmR*fmC, midMatrix[b],fmR*fmC, 0,tempDe,inputNum*convR*convC);
		//		2.De=De+tempDe
		cblas_daxpy(outputNum*(inputNum*convR*convC) ,1, tempDe,1, De, 1);
	}
	//求梯度的平均值并且乘学习率
	//De=learningRate*aver(De)
	cblas_dscal(outputNum*(inputNum*convR*convC), learningRate/(double)batchSize, De, 1);
#ifdef DEBUG
	printf(" De:\n");
	mPrint(De,outputNum,inputNum*convR*convC);
#endif // DEBUG
	//根据De更新权值
	//w=w-De
	//	w=-De+w
	cblas_daxpy(outputNum*(inputNum*convR*convC),-1, De,1, convCores, 1);
#ifdef DEBUG
	printf("ConvC:\n");
	mPrint(convCores,outputNum,inputNum*convR*convC);
#endif // DEBUG
}

void cConvolutionLayer::saveLayer(const char *filePath){
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

	fprintf(f,"InputMatrix %d %d\n",inputR,inputC);
	fprintf(f,"ConvMatrix %d %d\n",convR,convC);
	fprintf(f,"StepMatrix %d %d\n",stepR,stepC);

	for(int o=0;o<outputNum;++o){
		//每一组卷积核
		fprintf(f,"ConvCore[%d]:\n",o);
		for(int i=0;i<inputNum;++i){
			//每一个卷积核
			for(int r=0;r<convR;++r){
				for(int c=0;c<convC;++c){
					int a=o*(inputNum*convR*convC)+i*(convR*convC)+r*convC+c;
					fprintf(f,"%lf ",convCores[o*(inputNum*convR*convC)+i*(convR*convC)+r*convC+c]);
				}
				fprintf(f,"\n");
			}
			fprintf(f,"\n");
		}
	}

	fclose(f);
}

double* cConvolutionLayer::getConvCores(){
	return convCores;
}

void cConvolutionLayer::setConvCores(double *cc){
	convCores=cc;
}