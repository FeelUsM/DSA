#include <stdlib.h>
#include <time.h>
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <fstream>
using std::fstream;
#include "common.h"

 /* Функция выполняет вычисление подписи */
void form_pod(int len, const WORD *g1, const WORD * HH, const WORD *skl, WORD *R1, WORD *S1);

bool verbose;
int main(int argc, const char * argv[]){
	verbose = argc==1;
	char infilename[1000], outfilename[1000];
	if(verbose){
		//прочитать имя входного/выходного файла
		cout<<"Введите имя входного файла"<<endl;
		cin>>infilename;
		cout<<"Введите имя выходного файла"<<endl;
		cin>>outfilename;
		//вывести пример закрытого ключа
		cout<<"введите фиксированную часть закрытого ключа, например такой:"<<endl;
		cout<<hex_mas(10,XX)<<endl;
		//заполнить XX - 1я часть закрытого ключа
		cin>>hex_mas(10,XX);
			cout<<"прочитано: "<<endl;
			cout<<hex_mas(10,XX)<<endl;
		mod_p(XX,LEN1,QQ,LEN1);
		//прочесть или случайно сгенерировать k - 2я часть закрытого ключа
		cout<<"введите случайную часть закрытого ключа, \nесли не укажите, она будет сгенерирована случайно"<<endl;
		char s[100];
		cin.getline(s,100);
		cin.getline(s,100);
		if(*s==0){
			srand(time(0));
			for(int i=0; i<10; i++)
				k[i]=(WORD)rand();
			cout<<"сгенерировано: "<<endl;
		}
		else{
			stringstream str((string)s);
			str>>hex_mas(10,k);
			cout<<"прочитано: "<<endl;
		}
		mod_p(k,LEN1,QQ,LEN1);
		cout<<hex_mas(10,k)<<endl;
	}
	//Вычисление открытого ключа по секретному ключу
	WORD Y[LEN1], GG[LEN1], X3[LEN1], X4[LEN1], HH[LEN1], R1[LEN1], S1[LEN1];
	step_mod(PP,GG,LEN1,XX,LEN1,Y);
	if(verbose)
		cout<<endl << "Открытый ключ Y=G^x(mod P) = "<<endl;
	cout<<hex_mas(10,Y)<<endl;
	// вычисление G=Hnach^[(P-1)/Q](mod P)
	minus(LEN1,PP,ODIN,X3);
	Div(LEN1,X3,QQ,X4);
	step_mod(PP,Hnach,LEN1,X4,LEN1,GG);
	if(verbose){
		cout<< "вычисление G=Hnach^[(P-1)/Q](mod P)"<<endl;
		cout << "G = "<<hex_mas(LEN1,GG)<<endl ;
		//проверка модуля Р: G^Q=1(mod P)
		cout<< "проверка модуля Р: G^Q=1(mod P)"<<endl;
		step_mod(PP,GG,LEN1,QQ,LEN1,X3);
		cout<<"G^Q = "<<hex_mas(LEN1,X3)<<((cmp(11,X3,ODIN))?" - OK":" - что-то пошло не так")<<endl<<endl;
	}


	FILE * in_f = fopen(infilename,"r");
	if(!in_f){
		cerr <<"не могу открыть файл "<<infilename<<", останов"<<endl;
		exit(2);
	}
	file_hash(in_f,HH,verbose);
	if(verbose){
		cout<<"хэш входного файла = "<<endl;
		cout<<hex_mas(10,HH)<<endl;
	}
	form_pod(LEN1, GG, HH, k, R1, S1);
	if(verbose){
		cout<<endl<<"-Подпись сообщения-"<<endl
			<<"Компонента R = "<<hex_mas(10,R1)<<endl
			<<"Компонента S = "<<hex_mas(10,S1)<<endl;
	}
	fclose(in_f);
	//записать в файл
	{
		fstream out_f(outfilename,std::fstream::out);
		if(!out_f)
		if(!out_f){
			cerr <<"не могу открыть файл "<<outfilename<<", останов"<<endl;
			exit(2);
		}
		out_f<<hex_mas(10,R1)<<endl<<hex_mas(10,S1)<<endl;
	}
	{
		FILE * in_f=fopen(infilename,"r"), * out_f=fopen(outfilename,"a");
		if(!out_f){
			cerr <<"не могу открыть файл "<<outfilename<<", останов"<<endl;
			exit(2);
		}
		if(!in_f){
			cerr <<"не могу открыть файл "<<infilename<<", останов"<<endl;
			exit(2);
		}
		char arr[10000];
		while(!feof(in_f)){
			fgets(arr,10000,in_f);
			fputs(arr,out_f);
		}
		fclose(in_f);
		fclose(out_f);
		if(verbose)
			cout<<"подписанный файл создан"<<endl;
	}
	if(verbose)
		cout<<endl << "Открытый ключ Y=G^x(mod P) = "<<endl
			<<hex_mas(10,Y)<<endl;
	
#ifdef MSC_VER
	if(verbose){
		cout <<endl <<endl <<"нажмите любую клавишу... "<<endl;
		getchar();
		cout<<endl;
	}
#endif
}

 /* Функция выполняет вычисление подписи */
void form_pod(int len, const WORD *g1, const WORD * HH, const WORD *skl, WORD *R1, WORD *S1)
{
    WORD M[LEN1],X5[2*LEN1],X6[LEN1];
	/* 2) Вычисление R компонеты подписи */

	step_mod(PP,g1,len,skl,len,R1);
	mod_p(R1,len,QQ,len);

	/* 3) Вычисление S компоненты подписи */

	umn(XX,len,R1,len,X5,2*len);
	mod_p(X5,2*len,QQ,LEN1);
	plus(len,X5,HH,X6);
	mod_p(X6,len,QQ,LEN1);
	obr(len,skl,QQ,M);
	umn(X6,len,M,len,X5,2*len);
	mod_p(X5,2*len,QQ,LEN1);
	memcpy(S1,X5,len*sizeof(WORD));

	return;
}
