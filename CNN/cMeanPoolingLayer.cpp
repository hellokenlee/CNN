/*ALL Copyright Reversed by KenLee@2015*/

#include "cMeanPoolingLayer.h"
#define DEBUGn
cMeanPoolingLayer::cMeanPoolingLayer(int _inputNum,int _outputNum,int _batchSize,int _inputR,int _inputC,int _poolR,int _poolC)
	:cLayer(_inputNum,_outputNum,_batchSize){

	if(_inputNum!=_outputNum)
		printf("[MeanPoolingLayer] Error:inputNum not equal outputNum!\n");
	
	inputR=_inputR;
	inputC=_inputC;
	poolR=_poolR;
	poolC=_poolC;

	scale=1.0/(poolR*poolC);
	layerType="MeanPoolingLayer";
	outputR=(inputR-1)/poolR+1;
	outputC=(inputC-1)/poolC+1;

	outputSize=outputNum*outputR*outputC;
	inputSize=inputNum*inputR*inputC;
	
	output=new double[batchSize*outputNum*outputR*outputC];
	error=new double[batchSize*inputNum*inputR*inputC];
	
	
}

cMeanPoolingLayer::cMeanPoolingLayer(const char *filePath )
	:cLayer(inputNum,outputNum,batchSize){
	FILE *f;
	char buff[128];
	f=fopen(filePath,"r");
	fscanf(f,"%s %s %s %s",buff,buff,buff,buff);

	fscanf(f,"%s %d",buff,&inputNum);
	fscanf(f,"%s %d",buff,&outputNum);
	fscanf(f,"%s %d",buff,&batchSize);
	
	fscanf(f,"%s %d %d",buff,&inputR,&inputC);
	fscanf(f,"%s %d %d",buff,&poolR,&poolC);

	scale=1.0/(poolR*poolC);
	layerType="MeanPoolingLayer";
	outputR=(inputR-1)/poolR+1;
	outputC=(inputC-1)/poolC+1;

	outputSize=outputNum*outputR*outputC;
	inputSize=inputNum*inputR*inputC;

	output=new double[batchSize*outputNum*outputR*outputC];
	error=new double[batchSize*inputNum*inputR*inputC];
	fclose(f);
}

cMeanPoolingLayer::~cMeanPoolingLayer(){
	delete[] output;
	delete[] error;
}

//正向传播
void cMeanPoolingLayer::fPropagation(double *preLayerOutput){
	for(int b=0;b<batchSize;++b){
		for(int i=0;i<outputNum;++i){

			meanPolling(&preLayerOutput[b*inputSize+i*inputR*inputC],inputR,inputC,poolR,poolC,&output[b*outputSize+i*outputR*outputC]);

		}
	}
}

//反向传播
void cMeanPoolingLayer::bPropagation(double *nextLayerError,double *preLayerOutput){
	//更新残差
	//一批中
	for(int b=0;b<batchSize;++b){
		//每一个残差
		for(int i=0;i<outputNum;++i){
			mExpand(&nextLayerError[b*outputSize+i*outputR*outputC],outputR,outputC,poolR,poolC,&error[b*inputSize+i*inputR*inputC],inputR,inputC);
			
		}
	}
	cblas_dscal(batchSize*inputSize, scale, error, 1);
	//pooling层不需要更新权值,因此不需要计算梯度
}

void cMeanPoolingLayer::saveLayer(const char *filePath){
	FILE *f;
	f=fopen(filePath,"w");
	time_t t=time(NULL);
	tm *date=localtime(&t);
	fprintf(f,"%s  modify_at ",layerType.c_str());
	fprintf(f,"%d-%d-%d %d:%d:%d\n\n",date->tm_year+1990,date->tm_mon+1,date->tm_mday,date->tm_hour,date->tm_min,date->tm_sec);
	
	fprintf(f,"InputNum %d\n",inputNum);
	fprintf(f,"OutputNum %d\n",outputNum);
	fprintf(f,"BatchSize %d\n",batchSize);

	fprintf(f,"InputMatrix %d %d\n",inputR,inputC);
	fprintf(f,"ConvMatrix %d %d\n",poolR,poolC);

	fclose(f);
}