#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <errno.h>
#include <iomanip>
#include <ios>
#include <stdlib.h>
#include <time.h>

// 乱数を作る関数
double GetRandom(){
	double r;
	r=(double)rand()/((double)RAND_MAX+1);
	return r;
}

// 引数として書き込むファイル名が必要。
int main(int argc,char *argv[]){

	if( argc<2 || argc>5 ){
		std::cout << " usage	:	This program generates random numbers according to Gaussian and outputs them to the file specified by the argument ." <<std::endl;
		std::cout << "	:	ex).	./Box_Muller [out_file] [numbers] [sigma] [average]" <<std::endl;
		std::cout << "	:	You can specify up to 4 argument " <<std::endl;
		std::cout << " [out_file]	:	Required argument. " <<std::endl;
		std::cout << " 		:	Name of the file to output random numbers. " <<std::endl;
		std::cout << " [numbers]	:	It does not have to be an argument. " <<std::endl;
		std::cout << " 		:	Number of random numbers to generate. " <<std::endl;
		std::cout << " 		:	If there are no arguments , it will automatically be 1000000. " <<std::endl;
		std::cout << " [sigma]	:	It does not have to be an argument. " <<std::endl;
		std::cout << " 		:	It is the value of sigma. " <<std::endl;
		std::cout << " 		:	If there are no arguments , the value of sigma will be 1 automatically. " <<std::endl;
		std::cout << " [average]  	:	It does not have to be an argument. " <<std::endl;
		std::cout << " 		:	It is the value of average. " <<std::endl;
		std::cout << " 		:	If there are no arguments , the value of average will be 0 automatically. " <<std::endl;
		exit (1);
	}

	int rand_numb;	//発生させる乱数の数
	double sigma;	//sigmaの値
	double ave;		//averageの値
	double R;		//Gaussianに従う乱数の値


// 引数の処理
	if(argc>2){
		rand_numb = atoi(argv[2]);
	}
	else{
		rand_numb = 1000000;
	}

	if(argc>3){
		sigma = atof(argv[3]);
	}
	else{
		sigma = 1;
	}

	if(argc==5){
		ave = atof(argv[4]);
	}
	else{
		ave = 0;
	}

	srand((unsigned)time(NULL));

	std::ofstream fout(argv[1]);

	for(int i=0; i<rand_numb; i++){
		R=(sigma*sqrt(-2*log(GetRandom()))*cos(2*M_PI*GetRandom()))+ave;	//ガウスに従う乱数をRに代入
		fout << R << " " << GetRandom() << std::endl;		//Rの値と一様乱数をファイルに出力。
	}

	fout.close();
	return 0;
}
