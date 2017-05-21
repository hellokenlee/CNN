/*ALL Copyright Reversed by KenLee@2015*/

#include "cLayer.h"

//构造 指定参数 <!注意在子类中分配空间!>
cLayer::cLayer(int _inputNum,int _outputNum,int _batchSize){
	inputNum = _inputNum;
	outputNum = _outputNum;
	batchSize = _batchSize;

	output = NULL;
	error = NULL;

	inputSize=-1;
	outputSize=-1;

	layerType="Layer";
}

//析构
cLayer::~cLayer(){

}

//获取器
double* cLayer::getOutput(){
	return output;
}
double* cLayer::getError(){
	return error;
}
int cLayer::getInputNum(){
	return inputNum;
}
int cLayer::getOutputNum(){
	return outputNum;
}
int cLayer::getBatchSize(){
	return batchSize;
}
int cLayer::getOutputSize(){
	return outputSize;
}
int cLayer::getInputSize(){
	return inputSize;
}
std::string cLayer::getName(){
	return layerType;
}

