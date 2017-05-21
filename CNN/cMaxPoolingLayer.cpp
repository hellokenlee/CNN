/*ALL Copyright Reversed by KenLee@2015*/
#include "cMaxPoolingLayer.h"


cMaxPoolingLayer::cMaxPoolingLayer(int _inputNum,int _outputNum,int _batchSize,int _inputR,int _inputC,int _poolR,int _poolC)
	:cLayer(_inputNum,_outputNum,_batchSize){

	if(_inputNum!=_outputNum)
		printf("[MaxPoolingLayer] Error:inputNum not equal outputNum!\n");
	layerType="MaxPoolingLayer";

	inputR=_inputR;
	inputC=_inputC;
	poolR=_poolR;
	poolC=_poolC;

	outputR=(inputR-1)/poolR+1;
	outputC=(inputC-1)/poolC+1;

	outputSize=outputNum*outputR*outputC;
	inputSize=inputNum*inputR*inputC;

	output=new double[batchSize*outputNum*outputR*outputC];
	error=new double[batchSize*inputNum*inputR*inputC];
	maxPosition=new sPos[batchSize*outputNum*outputR*outputC];

}

cMaxPoolingLayer::cMaxPoolingLayer(const char *filePath)
	:cLayer(inputNum,outputNum,batchSize){

}

cMaxPoolingLayer::~cMaxPoolingLayer(){
	delete[] output;
	delete[] error;
	delete[] maxPosition;
}

//前向传播
//pR*pC中取最大的
void cMaxPoolingLayer::fPropagation(double *preLayerOutput){


}


//反向传播
void cMaxPoolingLayer::bPropagation(double *nextLayerError,double *preLayerOutput){

}

void cMaxPoolingLayer::saveLayer(const char *filePath){

}