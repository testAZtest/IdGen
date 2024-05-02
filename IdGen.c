#include "IdGen.h"
int keep_alnum(char *p, int len){
	char *ptr_r=p;
	char *ptr_w=p;
	int drop_count=0;
	int write_count=0;
	for(int i=0;i<len;i++){
		char tmp = *(ptr_r++);
		if(!tmp)break;
		if(isalnum(tmp)){
			*ptr_w++ = tmp;
			write_count++;
		}else{
			drop_count++;
		}
	}
	if(drop_count)*ptr_w++=0;
	return write_count;
}
void upper(char *p, int len){
	char *ptr=p;
	for(int i=0;i<len;i++){
		char tmp = *(ptr);
		if(!tmp)break;
		if(isalpha(tmp)){
			*(ptr++)=toupper(tmp);
		}
	}
}
int create_tables(char mode,int *table,char *reverse_table, int lt, char *black_list,int lb){
	for(int i=0;i<lt;i++){table[i]=1;reverse_table[i]=33;}
	for(int i=0;i<lb;i++){
		char t1=black_list[i];		
		if(isdigit(mode) && isdigit(t1)){
			t1-= _NUMERALS_OFFSET_;
			table[t1]=-1;
		}		
		if(isalpha(mode) && isalpha(t1)){
			t1-= _ABC_OFFSET_;
			table[t1]=-1;
		}
	}
	int sum=0;
	for(int i=0;i<lt;i++){
		int TTT = isdigit(mode) ? _NUMERALS_OFFSET_ : _ABC_OFFSET_;
		if(table[i]>0){
			reverse_table[sum]=i+TTT;
			table[i]=sum++;
		}
	}
	return sum;
}
int char_to_int(char x,int *table,int lt){
	if(!isalnum(x)){
		printf("Uff!\n"); 
		return -1;
	}
	if(isdigit(x)){
		int idx = x-_NUMERALS_OFFSET_;
		if(idx<lt)return table[idx];
	}
	if(isalpha(x)){
		int idx = x-_ABC_OFFSET_;
		if(idx<lt)return table[idx];
	}
	return -1;
}

int init_IdGen_State_Mashine(
	IdGen_State_Mashine *ptr,char *string,int sl,char *black1,int len1,char *blackA,int lenA){
	ptr->err_code=0;
	char tmp_buffer[MAX_BUFF];
    strcpy(tmp_buffer,string);
	sl=strlen(tmp_buffer);
	sl=keep_alnum(tmp_buffer,sl);//(string,sl);
	//printf("tmp_buffer: %d %s \n",sl, tmp_buffer);
	if((sl>STATE_LEN) && (sl&1)){
		printf("Bad_string!\n");
		return ++ptr->err_code;
	}
	sl=strlen(tmp_buffer);
	upper(tmp_buffer,sl);//(string,sl);
	for(int i=0;i<STATE_LEN;i++)ptr->Buffer[i]=0;
	ptr->Buffer[STATE_LEN]=0;
	for(int i=0;i<STATE_LEN;i++)ptr->States[i]=-1;
	if(black1 && len1>0){
	ptr->L1 = create_tables('1',ptr->DT,ptr->revDT, NUMERALS_LENGTH, black1,len1);
	}
	if(blackA && lenA>0){
	ptr->LA = create_tables('A',ptr->LT,ptr->revLT, ABC_LENGTH, blackA,lenA);
	}
	int k1 = STATE_LEN-2;
	int k=sl;
	for(;;){
		if(!k)break;
		char digit = tmp_buffer[--k];
		if(!isdigit(digit)){printf("Bad numeral! \n");return ++ptr->err_code;}
		
		int d1 = char_to_int(digit,ptr->DT,NUMERALS_LENGTH);
		if(d1<0){printf("Black listed digit!\n");return ++ptr->err_code;}
		ptr->Buffer[k1+1]=ptr->revDT[d1];
		ptr->StartPtr = &(ptr->Buffer[k1+1]);
		ptr->StartNumber=k1+1;
		//d1=(d1+1)%ptr->L1;
		ptr->States[k1+1]=d1;
		if(!k)break;
		char letter = tmp_buffer[--k];
		if(!isalpha(letter)){printf("Bad letter!\n");return ++ptr->err_code;}
		int l1 = char_to_int(letter,ptr->LT,ABC_LENGTH);
		if(l1<0){printf("Black listed letter!\n");return ++ptr->err_code;}
		ptr->States[k1]=l1;
		ptr->Buffer[k1]=ptr->revLT[l1];
		ptr->StartPtr = &(ptr->Buffer[k1]);
		ptr->StartNumber=k1;
		k1-=2;
		if(k1<0)break;
	}
	//printf("init: %s\n",ptr->StartPtr);
	return ptr->err_code;
}

void run_IdGen_State_Mashine(IdGen_State_Mashine *ptr){
	int carrying = 0;
	int k;
	for(k=STATE_LEN-2;k>=0;k-=2){
		if(ptr->States[k]<0){
			ptr->StartPtr = &(ptr->Buffer[k]);
			ptr->StartNumber=k;
			//printf("*\n");
		}
		//if(k==STATE_LEN-4)printf("*");
		if(++ptr->States[k+1]>=ptr->L1){
			ptr->States[k+1]-=ptr->L1;
			carrying =1;
		}
		ptr->Buffer[k+1]=ptr->revDT[ptr->States[k+1]];
		
		
		if(!carrying){
			if(ptr->States[k]<0){
				ptr->States[k]=0;
				ptr->Buffer[k]=ptr->revLT[ptr->States[k]];
				
			}
			break;
		}
		carrying = 0;
		
		if(++ptr->States[k]>=ptr->LA){
			ptr->States[k]-=ptr->LA;
			carrying =1;
		}
		ptr->Buffer[k]=ptr->revLT[ptr->States[k]];
		
		if(!carrying)break;
		carrying = 0;
	}
	//printf("run: %s %d\n",ptr->StartPtr,k);
	
}
int IdGen_sprintf(IdGen_State_Mashine *ptr,char *p,int len){
	int p_count=0;
	int first_time =1;
	for(int i = ptr->StartNumber ;i < STATE_LEN; i+=2){
		if(first_time){
			first_time =0;
		}else{
			p[p_count++]='-';if(p_count>=len){return p_count;}
		}
		p[p_count++]=ptr->Buffer[i];if(p_count>=len){return p_count;}
		p[p_count++]=ptr->Buffer[i+1];if(p_count>=len){return p_count;}
		
	}
	p[p_count++]=0;if(p_count>=len){return p_count;}
}
char * identificator_generation(IdGen_State_Mashine *ptr,char *in){
	char b[MAX_BUFF];
    strcpy(b,in);
	int sl=strlen(b);
	int err = init_IdGen_State_Mashine(ptr,b,sl,NULL,0,NULL,0);
	if(!err)run_IdGen_State_Mashine(ptr);
	IdGen_sprintf(ptr,ptr->Buffer_with_delimeters,2*STATE_LEN+1);
	//return err ? NULL : ptr->Buffer_with_delimeters;
	return ptr->Buffer_with_delimeters;
}