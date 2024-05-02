#include "IdGen.h"
void main(){
	IdGen_State_Mashine A;
	init_IdGen_State_Mashine(&A,"A1",2,"0",1,"DFGJMQV",7);
	char *Str = "A1";
	
	for(int k=0;k<19*9*19*9*19*9;k++){
		Str = identificator_generation(&A,Str);
		if(A.err_code)break;
		printf(" %s \n",Str);
	}
	
	
	
	/*
	for(int k=0;k<19*9*19*9*19*9;k++){
		run_IdGen_State_Mashine(&A);
		//printf(" %d: %s \n",A.StartNumber, A.StartPtr);
		printf("%s \n", A.StartPtr);
		
		//if(k%10==9)getchar();
		
		//for(int i=0;i<STATE_LEN;i++){
		//	if(A.Buffer[i])printf(" %c\n",A.Buffer[i]);
		//}
		
	}
	*/
	
	
}