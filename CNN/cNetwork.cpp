/*ALL Copyright Reversed by KenLee@2015*/

#include "cNetwork.h"

#define DEBUGn

cNetwork::cNetwork(int _inputNum,int _outputNum,int _batchSize,double _learningRate){
	inputNum=_inputNum;
	outputNum=_outputNum;
	batchSize=_batchSize;
	learningRate=_learningRate;
	layers.clear();
	//output永远指向最后一层的输出
	output=NULL;
	input=new double[batchSize*inputNum];
	error=new double[batchSize*outputNum];
	correct=new double[batchSize*outputNum];
}

cNetwork::cNetwork(const char *filePath){
	printf("\n[Network] Loading network from %s\n",filePath);
	string fileName(filePath);
	fileName+="/";
	fileName+=filePath;
	fileName+=".nn";
	FILE *f=fopen(fileName.c_str(),"r");
	if(!f){
		printf("[Network] Error: file not exist!\n");
		exit(-1);
	}
	char buf1[128];
	char buf2[128];
	int layerNum;
	cLayer *newLayer=NULL;
	layers.clear();
	fgets(buf1,128,f);
	fgets(buf2,128,f);
	printf("\t%s\n\t%s\n",buf1,buf2);
	fscanf(f,"%s",buf1);
	fscanf(f,"%s %d",buf1,&layerNum);
	fscanf(f,"%s %d",buf1,&batchSize);
	fscanf(f,"%s %d",buf1,&inputNum);
	fscanf(f,"%s %d",buf1,&outputNum);
	fscanf(f,"%s %lf",buf1,&learningRate);

	output=NULL;
	input=new double[batchSize*inputNum];
	error=new double[batchSize*outputNum];
	correct=new double[batchSize*outputNum];

	fscanf(f,"%s",buf1);
	for(int i=0;i<layerNum;++i){

		fscanf(f,"%s %s %s",buf1,buf1,buf2);
		//选择合适的层创建
		//	!如果增加了层，请修改这里
		if(strcmp(buf1,"FullyConnectedLayer")==0){
			//全连接层
			if(buf2[0]=='.')
				newLayer=new cFullyConnectedLayer(buf2);
			else if(buf2[0]=='N'){
				int layerIN,layerON;
				fscanf(f,"%s %d %s %d",buf1,&layerIN,buf2,&layerON);
				newLayer=new cFullyConnectedLayer(layerIN,layerON,batchSize,learningRate);
			}
		}else if(strcmp(buf1,"SigmoidLayer")==0){
			//Sigmoid层
			if(buf2[0]=='.')
				newLayer=new cSigmoidLayer(buf2);
			else if(buf2[0]=='N'){
				int layerIN;
				fscanf(f,"%s %d",buf1,&layerIN);
				newLayer=new cSigmoidLayer(layerIN,layerIN,batchSize);
			}
		}else if(strcmp(buf1,"ConvolutionalLayer")==0){
			//卷积层
			if(buf2[0]=='.')
				newLayer=new cConvolutionLayer(buf2);
			else if(buf2[0]=='N'){
				int layerIN,layerON,IR,IC,CR,CC,SR,SC;
				fscanf(f,"%s %d %s %d",buf1,&layerIN,buf2,&layerON);
				fscanf(f,"%s %d %d %s %d %d %s %d %d",buf1,&IR,&IC,buf1,&CR,&CC,buf2,&SR,&SC);
				newLayer=new cConvolutionLayer(layerIN,layerON,batchSize,IR,IC,CR,CC,SR,SC,learningRate);
			}
		}else if(strcmp(buf1,"MeanPoolingLayer")==0){
			//平均下采样层
			if(buf2[0]=='.')
				newLayer=new cMeanPoolingLayer(buf2);
			else if(buf2[0]=='N'){
				int layerIN,IR,IC,CR,CC;
				fscanf(f,"%s %d",buf1,&layerIN);
				fscanf(f,"%s %d %d %s %d %d",buf1,&IR,&IC,buf2,&CR,&CC);
				newLayer=new cMeanPoolingLayer(layerIN,layerIN,batchSize,IR,IC,CR,CC);
			}
		}else if(strcmp(buf1,"MinistInputLayer")==0){
			//Minist输入层
			if(buf2[0]=='.')
				newLayer=new cMinistInputLayer(buf2);
			else if(buf2[0]=='N'||buf2[0]=='n'){
				char inputFilePath[128];
				fscanf(f,"%s %s",buf1,inputFilePath);
				newLayer=new cMinistInputLayer(inputNum,outputNum,batchSize,inputFilePath);
			}
		}else if(strcmp(buf1,"SquareLossLayer")==0){
			//方差损失层
			if(buf2[0]=='.')
				newLayer=new cSquareLossLayer(buf2);
			else if(buf2[0]=='N'||buf2[0]=='n'){
				int layerIN;
				fscanf(f,"%s %d",buf1,&layerIN);
				newLayer=new cSquareLossLayer(layerIN,batchSize);
			}
		}else{
			printf("\n[Network] not suitable Layer called %s\n",buf2);
			system("PAUSE");
			exit(-1);
		}
		addLayer(newLayer);
	}
	fclose(f);
	printf("[Network] Done Loading!\n");
}

cNetwork::~cNetwork(){
	delete[] input;
	delete[] error;
	delete[] correct;
	/*
	for (int i=0;i<layers.size();++i){
		delete layers.at(i);
	}
	*/

}

//前向传播
void cNetwork::fPropagation(){
	if(layers.size()==0)
		return;
	//对于输入层前向传播,并把正确结果读到correct中
	layers.at(0)->fPropagation(correct);
	//对于后面的层,做前向传播
	for(unsigned int i=1;i<layers.size();++i){
		layers.at(i)->fPropagation(layers.at(i-1)->getOutput());
	}

}

//反向传播
void cNetwork::bPropagation(){


	int layerSize=layers.size();
	//损失层反向传播,得到误差
	layers.back()->bPropagation(correct,layers.at(layerSize-2)->getOutput());

	//反向传播中间的隐藏层(不传0层因为是输入层)
	for(int i=layerSize-2;i>0;i--){
		layers.at(i)->bPropagation(layers.at(i+1)->getError(),layers.at(i-1)->getOutput());
	}
}

void cNetwork::trainMinist(char* trainFile,int times,int display){
	//错误提示
	if(layers.size()<3){
		printf("[Network] Error: a network must at least has 3 layers");
		system("PAUSE");
	}
	//构造输入层
	delete layers.at(0);
	layers.at(0)=new cMinistInputLayer(inputNum,outputNum,batchSize,trainFile);
	//训练
	int i=0;
	long ti=(unsigned)time(NULL);
	while (times--){
		fPropagation();
		bPropagation();
		if(i%display==0){
			printf("[Network]Trained Batch %d(batchSize=%d)...\n",i,batchSize);
			printError();
		}
		i++;
	}
}
//把out[batchSize][10]->guess[batchSize]
void cNetwork::deCode(double *out,int *guess){
	for(int b=0;b<batchSize;++b){
		double max=-1;
		int maxPos=-1;
		for(int o=0;o<outputNum;++o){
			if(max<out[b*outputNum+o]){
				max=out[b*outputNum+o];
				maxPos=o;
			}
		}
		guess[b]=maxPos;
	}
}

int cNetwork::testSingleImage(const char* file){
    //构造输入层
	delete layers.at(0);
	cImageInputLayer* iil = new cImageInputLayer(inputNum,outputNum,batchSize);
    layers.at(0) = iil;
    //
    int guess;
    iil->getDataFromImage(file);
    fPropagation();
    deCode(output, &guess);
    printf("Guess: %d\n", guess);
    return guess;
}

double cNetwork::testMisist(char* testFile,int times){
	printf("[Network] Testing...\n");
	//错误提示
	if(layers.size()<3){
		printf("[Network] Error: a network must at least has 3 layers");
		system("PAUSE");
	}
	//构造输入层
	delete layers.at(0);
	layers.at(0)=new cMinistInputLayer(inputNum,outputNum,batchSize,testFile);
	//测试
	int *guess=new int[batchSize];
	int *answer=new int[batchSize];
	int *fault=new int[outputNum];
	memset(fault,0,sizeof(int)*outputNum);
	int count=0,passNum=0,failNum=0;
	while (times--){
		fPropagation();
		deCode(output,guess);
		deCode(correct,answer);
		for(int i=0;i<batchSize;++i){
			//printf("\nnum: %d, guess: %d,reslut:%d ",count++,guess[i],answer[i]);

			if(guess[i]==answer[i]){
				passNum++;
				//printf("  [pass]\n");

			}
			else{
				failNum++;
				//printf("  [fail]\n");
				fault[answer[i]]++;

			}
		}
	}
	printf("[Network] Test Result:\n\tPass:%d  \n\tFail:%d\n\tAccuracy=%lf %%\n",passNum,failNum,(double)passNum/((double)passNum+(double)failNum)*100.0);
	printf("\tFault distribution:\n");
	for (int i=0;i<10;++i){
		printf("\t\t%d:  %d\n",i,fault[i]);
	}
	return (double)passNum/((double)passNum+(double)failNum);
}

void cNetwork::saveNetwork(const char* filePath){
	//建立文件夹
	string fileName(filePath);
#ifdef _MSC_VER
    _mkdir(filePath);
#endif

#ifdef __GNUC__
    mkdir(filePath, S_IRWXG);
#endif
	//保存网络配置
	fileName+="/";
	fileName+=filePath;
	fileName+=".nn";
	FILE *f=fopen(fileName.c_str(),"w");
	fprintf(f,"NeuralNetwork modify_at ");
	time_t t=time(NULL);
	tm *date=localtime(&t);
	fprintf(f,"%d-%d-%d %d:%d:%d\n",date->tm_year+1900,date->tm_mon+1,date->tm_mday,date->tm_hour,date->tm_min,date->tm_sec);
	fprintf(f,"Power by KenLee@2015\n\n");
	fprintf(f,"Network:\n");
	fprintf(f,"LayerNum %d\nBatchSize %d\nInputNum %d\nOutputNum %d\nLearningRate %lf\n\n"
		,layers.size(),batchSize,inputNum,outputNum,learningRate);

	//保存每一层
	string layerPath(filePath);
	layerPath+="/layer";
	fprintf(f,"Layers:\n");
	for(unsigned int i=0;i<layers.size();++i){
		string layerName=layerPath;
		char num[10];
		sprintf(num,"%d\0",i);
		layerName+=num;
		layerName+=".nnl";;
		fprintf(f,"layer[%d] %s ./%s\n",i,layers.at(i)->getName().c_str(),layerName.c_str());
	}
	fclose(f);
	for(unsigned int i=0;i<layers.size();++i){
		string layerName=layerPath;
		char num[10];
		sprintf(num,"%d\0",i);
		layerName+=num;
		layerName+=".nnl";
		layers.at(i)->saveLayer(layerName.c_str());
	}
	printf("[Network] save Network to %s\n",filePath);
}

void cNetwork::addLayer(cLayer *L){
	if (layers.size()>0&&layers.back()->getOutputSize()!=L->getInputSize()){
		printf("[Network] Error: inputSzie of th layer you added is not equal to outputSize of the last layer.\n");
		system("PAUSE");
		exit(-1);
	}
	layers.push_back(L);
	output=L->getOutput();
}

double* cNetwork::getError(){
	return error;
}
double* cNetwork::getInput(){
	return input;
}
double* cNetwork::getOutput(){
	return output;
}
int cNetwork::getBatchSize(){
	return batchSize;
}

double* cNetwork::getCorrect(){
	return correct;
}
void cNetwork::printError(){
	for (int j=0;j<batchSize;j++){
		double eSum=0;
		for (int i=0;i<outputNum;++i){
			double e=correct[i*batchSize+j]-output[i*batchSize+j];
			e=abs(e);
			eSum+=e;
		}
		/*
		printf("y[]=");
		for (int i=0;i<outputNum;++i){
			printf("%.2lf ",correct[i*batchSize+j]);
		}
		printf("\n");
		printf("o[]=");
		for (int i=0;i<outputNum;++i){
			printf("%.2lf ",output[i*batchSize+j]);
		}*/
		printf("\tError:%lf\n",eSum);
	}
}
