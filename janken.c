#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// 乱数を作る関数
double GetRandom(){
    double r;
    r=(double)rand()/((double)RAND_MAX);
    return r;
}

int input_bool(char *buf){
    int len;
    int a2i;
    len = strlen(buf);
    if(len>1){
	printf("指定された数字を入力してね！\n");
	return 0;
    }
    else{
	a2i = atoi(buf);
	if(a2i==1){
	    return a2i;
	}
	else if(a2i==2){
	    return a2i;
	}
	else if(a2i==3){
	    return a2i;
	}
	else{
	    printf("指定された数字を入力してね！\n");
	    return 0;
	}
    }
}

int yomitori (){
    char buf[64];
    int status = 0;
    while(status==0){
	printf("何を出す?\n");
	printf("グー : 1  ,  チョキ : 2  ,  パー : 3\n");
	scanf("%s",buf);
	status = input_bool(buf);
    }
    return status;
}

int result(int input_acquisition, int my_ans){
    int sa;
    sa = input_acquisition - my_ans;
    if(sa == 0){
	printf("draw !\n");
	printf("もう一回だ!!\n");
	return 0;
    }
    if(sa == -1 || sa == 2){
	printf("YOU WIN !!\n");
	return 1;
    }
    else {
	printf("YOU LOSE !!\n");
	return 1;
    }
}

char *translation(int i){
    if(i==1){
	return "グー";
    }
    else if(i==2){
	return "チョキ";
    }
    else{
	return "パー";
    }
}

int main(){
    srand((unsigned)time(NULL));

    int input_acquisition;
    int my_ans;
    int do_bool=0;

    printf("じゃんけんしよう!!\n");
    printf("最初はグー\n");
    while(do_bool==0){
	printf("じゃんけん ぽい!!\n");
	printf("\n");
	printf("------------------------------------------\n");
	input_acquisition = yomitori();
	printf("------------------------------------------\n");
	my_ans = 3*GetRandom()+1;
	printf("相手は%sを出した!!\n",translation(my_ans));
	do_bool = result(input_acquisition, my_ans);
	printf("\n");
	printf("\n");
	printf("\n");
    }
    printf("\n");
    printf("また游んでね !!\n");
    return 0;
}
