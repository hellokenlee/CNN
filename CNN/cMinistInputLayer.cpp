#include "cMinistInputLayer.h"
#define DEBUGn
cMinistInputLayer::cMinistInputLayer(int _inputNum,int _NetworkOutputNum,int _batchSize,char* _filePath)
	:cLayer(_inputNum,_inputNum,_batchSize){

	layerType="MinistInputLayer";
	inputSize=inputNum;
	outputSize=outputNum;
	NetworkOutputNum=_NetworkOutputNum;

	filePath=new char[128];
	strcpy(filePath,_filePath);
	file=fopen(filePath,"r");
	if(!file){
		printf("[MinistInputLayer]Error: %d not found!\n",filePath);
		system("PAUSE");
		exit(-1);
	}
	output=new double[outputNum*batchSize];
	answer=new int[batchSize];
	//不需要有误差
	error=NULL;
	//对文件头进行处理
	fscanf(file,"%s",buf);
	isEOF=false;
}
cMinistInputLayer::cMinistInputLayer(const char* _filePath)
	:cLayer(inputNum,outputNum,batchSize){
	FILE *f;
	char buff[128];
	filePath=new char[128];
	f=fopen(_filePath,"r");
	if(!f){
		printf("[MIL]file %s Not found\n", f);
        exit(-1);
	}
	fscanf(f,"%s %s %s %s",buff,buff,buff,buff);
	fscanf(f,"%s %d",buff,&inputNum);
	fscanf(f,"%s %d",buff,&NetworkOutputNum);
	fscanf(f,"%s %d",buff,&batchSize);
	fscanf(f,"%s %s",buff,filePath);

	layerType="MinistInputLayer";
	outputNum=inputNum;
	inputSize=inputNum;
	outputSize=outputNum;
	file=fopen(filePath,"r");
	if(!file){
		printf("[MinistInputLayer]Error: %s not found!\n",filePath);
		system("PAUSE");
		exit(-1);
	}
	output=new double[outputNum*batchSize];
	//不需要有误差
	error=NULL;
	//对文件头进行处理
	fscanf(file,"%s",buf);
	isEOF=false;
}
cMinistInputLayer::~cMinistInputLayer(){
	fclose(file);
	delete[] output;
	delete[] filePath;
	delete[] answer;
};

//change result[batchsize] -> answer[batchSize][10]
void cMinistInputLayer::enCode(double *correct){
	memset(correct,0,sizeof(double)*NetworkOutputNum*batchSize);
	for(int b=0;b<batchSize;++b){
		//printf("%d ",result[b]);
		correct[outputNum*b+answer[b]]=1;
	}
}

//前向传播:
//	- 从文件读取一批的数据,放入output;
//	- 从文件读取答案x(0~9),然后转成10位的double数组,存入correct[batchSize*NetworkOutputNum]中
void cMinistInputLayer::fPropagation(double *correct){
	if(isEOF){
		fclose(file);
		file=fopen(filePath,"r");
		//对文件头进行处理
		fscanf(file,"%s",buf);
		isEOF=false;
	}
	for(int b=0;b<batchSize;++b){
		if(fscanf(file,"%d,",&num)==EOF){
			isEOF=true;
			break;
		}
		//读入一个图片数据
		for(int i=0;i<inputNum;++i){
			fscanf(file,"%lf,",&output[b*inputNum+i]);
		}
		//读入答案
		fscanf(file,"%d",&answer[b]);
#ifdef DEBUG
		printf("\nnum:%d  answer:%d\n",num,answer[b] );

#endif
	}
	//mPrint(output,batchSize,inputNum);
	enCode(correct);
}

//后向传播:
//	- 不应该被执行
void cMinistInputLayer::bPropagation(double *nextLayerError,double *preLayerOutput){
	printf("[MinistInputLayer]Error: bPropagation should not be used!\n");
	system("PAUSE");
	exit(-1);
}


//保存文件
void cMinistInputLayer::saveLayer(const char *_filePath){
	FILE *f;
	f=fopen(_filePath,"w");
	time_t t=time(NULL);
	tm *date=localtime(&t);
	fprintf(f,"%s  modify_at ",layerType.c_str());
	fprintf(f,"%d-%d-%d %d:%d:%d\n\n",date->tm_year+1990,date->tm_mon+1,date->tm_mday,date->tm_hour,date->tm_min,date->tm_sec);

	fprintf(f,"InputNum %d\n",inputNum);
	fprintf(f,"NetworkOutputNum %d\n",NetworkOutputNum);
	fprintf(f,"BatchSize %d\n",batchSize);
	fprintf(f,"InputFile %s\n",filePath);
	fclose(f);
}
char* cMinistInputLayer::getInputFilePath(){
	return filePath;
}
