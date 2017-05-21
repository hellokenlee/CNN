/*ALL Copyright Reversed by KenLee@2015*/
#include "cSigmoidLayer.h"
// [batchSize][outputNum]

#define DEBUGn
cSigmoidLayer::cSigmoidLayer(int _inputNum,int _outputNum,int _batchSize)
	:cLayer(_inputNum,_outputNum,_batchSize){
	
	if(_inputNum!=_outputNum)
		printf("[sigmoidLayer] Error:inputNum not equal outputNum!\n");
	layerType="SigmoidLayer";
	output = new double[batchSize*outputNum];
	error = new double[batchSize*inputNum];
	inputSize=_inputNum;
	outputSize=_outputNum;
}


cSigmoidLayer::cSigmoidLayer(const char* filePath)
	:cLayer(inputNum,outputNum,batchSize){
	FILE *f;
	char buff[128];
	f=fopen(filePath,"r");
	fscanf(f,"%s %s %s %s",buff,buff,buff,buff);

	fscanf(f,"%s %d",buff,&inputNum);
	fscanf(f,"%s %d",buff,&batchSize);

	outputNum=inputNum;
	layerType="SigmoidLayer";
	output = new double[batchSize*outputNum];
	error = new double[batchSize*inputNum];
	inputSize=inputNum;
	outputSize=outputNum;
	fclose(f);
}
cSigmoidLayer::~cSigmoidLayer(){
	delete[] output;
	delete[] error;
}

void cSigmoidLayer::fPropagation(double *preLayerOutput){
	for(int i=0;i<batchSize*inputNum;++i){
		output[i]=1.0/(1.0+exp(-preLayerOutput[i]));
	}

}


void cSigmoidLayer::bPropagation(double *nextLayerError,double *preLayerOutput){
	for (int i=0;i<batchSize*inputNum;++i){
		error[i]=output[i]*(1-output[i])*nextLayerError[i];
	}
#ifdef DEBUG
	printf("nextErr:\n");
	mPrint(nextLayerError,batchSize,outputNum);
	printf("SIG Err:\n");
	mPrint(error,batchSize,outputNum);
#endif
}

void cSigmoidLayer::saveLayer(const char *filePath){
	FILE *f;
	f=fopen(filePath,"w");
	time_t t=time(NULL);
	tm *date=localtime(&t);
	fprintf(f,"%s  modify_at ",layerType.c_str());
	fprintf(f,"%d-%d-%d %d:%d:%d\n\n",date->tm_year+1990,date->tm_mon+1,date->tm_mday,date->tm_hour,date->tm_min,date->tm_sec);

	fprintf(f,"InputNum %d\n",inputNum);
	fprintf(f,"BathcSize %d\n",batchSize);

	fclose(f);
}