/*ALL Copyright Reversed by KenLee@2015*/

#include "cLayer.h"

//���� ָ������ <!ע���������з���ռ�!>
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

//����
cLayer::~cLayer(){

}

//��ȡ��
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

