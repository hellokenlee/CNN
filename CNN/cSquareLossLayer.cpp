#include "cSquareLossLayer.h"

cSquareLossLayer::cSquareLossLayer(int _inputNum,int _batchSize)
	:cLayer(_inputNum,_inputNum,_batchSize){
	
	inputSize=inputNum;
	outputSize=outputNum;
	layerType="SquareLossLayer";
	
	error=new double[batchSize*outputNum];
	output=new double[batchSize*outputNum];
}

cSquareLossLayer::cSquareLossLayer(const char *filePath)
	:cLayer(inputNum,outputNum,batchSize){
	FILE *f;
	char buff[128];
	f=fopen(filePath,"r");
	fscanf(f,"%s %s %s %s",buff,buff,buff,buff);

	fscanf(f,"%s %d",buff,&inputNum);
	fscanf(f,"%s %d",buff,&batchSize);
	outputNum=inputNum;
	inputSize=inputNum;
	outputSize=outputNum;
	layerType="SquareLossLayer";

	error=new double[batchSize*outputNum];
	output=new double[batchSize*outputNum];

	fclose(f);
}

cSquareLossLayer::~cSquareLossLayer(){
	delete[] error;
	delete[] output;
}



//正向传播:
void cSquareLossLayer::fPropagation(double *preLayerOutput){
	for(int i=0;i<batchSize*outputNum;++i){
		output[i]=preLayerOutput[i];
	}
}
//后向传播:
//	- 计算误差
void cSquareLossLayer::bPropagation(double *NetworkCorrect,double *preLayerOutput){
	//mPrint(preLayerOutput,batchSize,inputNum);
	//mPrint(NetworkCorrect,batchSize,outputNum);
	for(int i=0;i<batchSize*outputNum;++i){
		error[i]=preLayerOutput[i]-NetworkCorrect[i];
	}
	//mPrint(error,batchSize,outputNum);
}

void cSquareLossLayer::saveLayer(const char *filePath){
	FILE *f;
	f=fopen(filePath,"w");
	time_t t=time(NULL);
	tm *date=localtime(&t);
	fprintf(f,"%s  modify_at ",layerType.c_str());
	fprintf(f,"%d-%d-%d %d:%d:%d\n\n",date->tm_year+1990,date->tm_mon+1,date->tm_mday,date->tm_hour,date->tm_min,date->tm_sec);
	
	fprintf(f,"InputNum %d\n",inputNum);
	fprintf(f,"BatchSize %d\n",batchSize);
	fclose(f);
}