#include <stdio.h>
#include <stdlib.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int table[11][7] = {{-1, 1,  2,  3,  4,  5,  6}, //可對照下面 checks function  
                   {  1, 2, -1,  8, 11, -1,  8},
                   {  2, 2,  3,  5, 11, -1, -1},
                   {  3, 4, -1, -1, -1, -1, -1},
                   {  4, 4, -1,  5, 11, -1, -1},
                   {  5, 7, -1, -1,  6, -1, -1},
                   {  6, 7, -1, -1, -1, -1, -1},
                   {  7, 7, -1, -1, 11, -1, -1},
		   {  8, 8, -1,  8, 11, -1,  8},};
				   							 //SCANNER 表
				   							 //狀態一 -> 狀態二 (進入判斷是否為number 的 DFA) 老師PPT  lecture 1 p.49 的 DFA (13為2,14為3 以此類推) 
				   							 //狀態一 -> 狀態八 (進入判斷是否為id 的 DFA 只有一個STATE 因為不論數字或英文都可接受 ) 
				   							 //狀態11為可切割時進入的STATE  1.遇到TER 已不符合 DFA  但卻U停留在 可接受的狀態  則可切割 
											                           
struct parser{			
        char context[10];
    };								
									//PARSER表 
									//AssignStmts  →  Stmt  AssignStmts  |  Stmt	
									//轉換為:A ->CB
									//		 B ->f | 空								
struct parser parsetable[12][17] = {{"x",  "+",  "-",  "*",  "/",  "(", ")", "N", "I",    "D", ";", "=", "{", "}",        "i", ">", "$"}, //TER 
				    {"S",  "x",  "x",  "x",  "x",  "x", "x", "x", "x",   "Z$", "x", "x", "x", "x",       "Z$", "x", "x"}, //S ->Start 
				    {"Z",  "x",  "x",  "x",  "x",  "x", "x", "x", "x",    "A", "x", "x", "x", "x",        "f", "x", "x"}, //Z ->Stmts
				    {"A",  "x",  "x",  "x",  "x",  "x", "x", "x", "x",   "CB", "x", "x", "x", "x",        "x", "x", "x"}, //A ->AssignStmts
				    {"B",  "x",  "x",  "x",  "x",  "x", "x", "x", "x",    "A",  "",  "",  "",  "",         "",  "",  ""}, //B  
				    {"f",  "x",  "x",  "x",  "x",  "x", "x", "x", "x",    "x", "x", "x", "x", "x","i(I>d){A}", "x", "x"}, //f ->ifStmt 
				    {"C",  "x",  "x",  "x",  "x",  "x", "x", "x", "x","DI=E;", "x", "x", "x", "x",        "x", "x", "x"}, //C ->Stmt
    				    {"E",  "x",  "x",  "x",  "x", "TR", "x","TR","TR",    "x", "x", "x", "x", "x",        "x", "x", "x"}, //E ->expression
    				    {"R","+TR","-TR",  "x",  "x",  "x",  "", "x", "x",    "x",  "",  "",  "",  "",         "",  "",  ""}, 
    				    {"T",  "x",  "x",  "x",  "x", "FY", "x","FY","FY",    "x", "x", "x", "x", "x",        "x", "x", "x"}, //T ->term 
    				    {"Y",   "",   "","*FY","/FY",  "x",  "", "x", "x",    "x",  "",  "",  "",  "",         "",  "",  ""},
   				    {"F",  "x",  "x",  "x",  "x","(E)", "x", "N", "I",    "x", "x", "x", "x", "x",        "x", "x", "x"}};//F ->factor
struct stack{
	char stk[100];
	int top;
};
typedef struct stack STACK;
STACK s;
void push(char);
char  pop(void);
void display (void);
int  checkp(char);
int checkparserstate(char);
int  parsertest(void);
int  checks(char);
int  scanner(void);
char n[100],pn[50];
int  off=0;
int main(int argc, char *argv[]) 
{   
        printf("This is an identify number machine!!!\n");
	printf("Please enter a test data:\n");
	memset(n,0,100);		
	gets(n);
   if(scanner()==1){
    	printf("After scanner :\n");
    	printf("%s\n",pn);
    	printf("The result is:\n");
		if(parsertest()==1) 
		  printf("yes\n"); 
		else printf("syntax_error\n");
    }else{
    	printf("After scanner :\n");
    	printf("%s\n",pn);
    	printf("The result is:\n");
    	printf("lexical_error\n");
    }
    system("PAUSE");
	return 0;
	
}
int checks(char a)
{
 	if(a>=48 && a<=57)  return 1; //0~9
	if(a==46)           return 2; //.	
	if(a==69 )          return 3; //E
        if(a==43 || a==45)	return 4; //+ | -
        if(a>=65 && a<=90)  return 6; //A~Z
	return 5;		
 }
 int scanner()
 {	
 	memset(pn,0,50);
 	int c=0,p=0,l=0;
 	int state=1,k,current_char=0;
 	for( k=0;k<strlen(n);k++){
 		while(n[k]==' '||n[k]=='\t'){k++;}                                                                        //去除空格 或 TAB 
 																												  //以下三個優先切割 
		if(state==1&&n[k]=='d'&&n[k+1]=='e'&&n[k+2]=='c'&&n[k+3]=='l'&&n[k+4]=='a'&&n[k+5]=='r'&&n[k+6]=='e'){    //1.保留字declare 
			pn[p]='D';
 			p++;
 			k=k+7;
		}
		while(n[k]==' '||n[k]=='\t'){k++;}
		if(state==1&&n[k]=='I'&&n[k+1]=='f'){                                                                     //2.保留字IF 
			pn[p]='i';
 			p++;
 			k=k+2;
		}
		while(n[k]==' '||n[k]=='\t'){k++;}
		if(state==1&&pn[p-1]=='>' &&checks(n[k])==1){					                                          //3.皆在 > 後的 digit 
			pn[p]='d';
 			p++;
 			k++;
		}
		while(n[k]==' '||n[k]=='\t'){k++;}
     	current_char=checks(n[k]);                                                                                //判斷字元為哪個 
     	if(table[state][current_char]!=-1){                                                                       //跑表  如為 -1 是錯  ,不為錯才轉換state 
     		state=table[state][current_char];
     		if(state==2) c=1;                                                                                     //以進入判斷 number的 DFA 
     		else if(state==8) l=1;																				  //以進入判斷 id    的 DFA  
     	}else {
     		if(checkp(n[k])!=0&&(state==1||state==2||state==4||state==7||state==8)) state=11;                     //checkp(n[k])!=0 表示我們可接受的ter ,如目前state 為可接受的 就轉入11 (可做切割)     
  			else return 0; 
     	}
	 	if(state==11&&c==1){                                                                                      //可切割且之前為跑number 的DFA 則轉換成 N 存入字串 
	 		pn[p]='N';
 			p++;
 			pn[p]=n[k];
 			p++;
 	 		c=0;
 	 		state=1;
 	 	}else if(state==11&&l==1){																				   //可切割且之前為跑id 的DFA 則轉換成 I 存入字串 
	 		pn[p]='I';
 			p++;
 			pn[p]=n[k];
 			p++;
 	 		l=0;
 	 		state=1;
 	 	}else if(state==11){																					   //可接受的TER 存入字串 
 	 		pn[p]=n[k];
 			p++;
			state=1;
	 	}	 	
     }
     if(state==2||state==4||state==7) pn[p]='N';                                                                  //最後再做一次確認 
     else if(state==8) pn[p]='I';
	 else if(c==1||l==1)	return 0; 	
     return 1;
 }
 int checkp(char a)         //可接受TER 
 {
 	if(a=='+')	return 1;
	if(a=='-')	return 2;	
	if(a=='*')	return 3;
        if(a=='/')	return 4;
        if(a=='(')	return 5;
	if(a==')')	return 6;
	if(a=='N')	return 7;
	if(a=='I')	return 8;
	if(a=='D')	return 9;
	if(a==';')	return 10;
	if(a=='=')	return 11;
	if(a=='{')	return 12;
	if(a=='}')	return 13;
	if(a=='i')	return 14;
	if(a=='>')	return 15;
	if(a=='$')	return 16;	
	return 0;
 }
 int checkparserstate(char a) //PARSER 狀態 
 {	
 	if(a=='S')	return 1;
 	if(a=='Z')	return 2;
	if(a=='A')      return 3;	
	if(a=='B')	return 4;
	if(a=='f')	return 5;
        if(a=='C')	return 6;
 	if(a=='E')	return 7;
	if(a=='R')      return 8;	
	if(a=='T')	return 9;
        if(a=='Y')	return 10;
        if(a=='F')	return 11;	
	return 0;	
 }
int  parsertest()
 {
 	int state=1,current_char=0,k,j,q=0;
 	char test=0;
 	pn[strlen(pn)]='$';                                                                   //尾端先加入$ 配合表 
 	for(k=0;k<strlen(pn);k++){
 	 while(test==pn[k]&&checkparserstate(test)==0){
 	 	if(test=='$') return 1;
 	 	test=pop();
 	 	k++;
 	 	q=1;
 	 }
 	 if(q==1){
 	 	state=checkparserstate(test);
 	 	q=0;
 	 }
     current_char=checkp(pn[k]);
     do{
     	if(parsetable[state][current_char].context[0]!='x' && current_char!=0&&state!=0){// 跑表 
     	 	for(j=strlen(parsetable[state][current_char].context)-1;j>=0;j--){           //要重尾端PUSH  , POP 順序才對 
     			push(parsetable[state][current_char].context[j]);                   
     		}
     	test=pop();
     	state=checkparserstate(test);
     	}else{
			return 0;
		}	
    	}while(test!=pn[k]);                                                            //轉換到有出現與字串相同的字元                                                         
     	 if(test=='$') return 1;
		 test=pop();
     	 state=checkparserstate(test);  	
	} 
	 return 0;
 }
void push (char q)
{
	if (s.top == (100 - 1)){
		printf ("Stack is Full\n");
		return;
	}
	else{
		s.top = s.top + 1;
		s.stk[s.top] =q;
		//printf ("PUSH: %c\n",q);
	}
	return;
}
char pop ()
{
	char num;
	if (s.top == - 1){
		//printf ("Stack is Empty\n");
		return ('x');
	}
	else{
		num = s.stk[s.top];
		s.top = s.top - 1;
		//printf (" POP :%c\n",num);
	}
	return(num);
}
void display ()
{
	int i;
	if (s.top == -1){
		printf ("Stack is empty\n");
		return;
	}
	else{
		printf ("\nThe status of the stack is\n");
		for (i = s.top; i >= 0; i--){
			//printf ("%d %d\n", s.stk[i],i);
		}
	}
	printf ("\n");
}
