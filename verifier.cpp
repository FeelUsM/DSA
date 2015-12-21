#include <stdlib.h>
#include <fstream>
using std::fstream;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include "common.h"
/* Функция выполняет проверку подписи */
bool prov_pod(int len, const WORD *g1, const WORD * HH1, const WORD *okl, const WORD *R1, const WORD *S1);

bool verbose;
int main(int argc, const char * argv[]){
	verbose = argc==1;
	char infilename[1000], outfilename[1000];
	WORD Y[LEN1], GG[LEN1], X3[LEN1], X4[LEN1], HH[LEN1], R1[LEN1], S1[LEN1];
	if(verbose){
		//прочитать имя входного/выходного файла
		cout<<"Введите имя входного файла"<<endl;
		cin>>infilename;
		cout<<"Введите имя выходного файла"<<endl;
		cin>>outfilename;
		//вывести пример ключа
		cout<<"введите открытый ключ, например такой:"<<endl;
		cout<<hex_mas(10,XX)<<endl;
		//заполнить YY - открытый ключ
		cin>>hex_mas(10,Y);
			cout<<"прочитано: "<<endl;
			cout<<hex_mas(10,Y)<<endl;
		mod_p(Y,LEN1,QQ,LEN1);
	}
	else
		;//разобрать параметры
	
	{//	прочитать R, S
		fstream in_f(infilename,std::fstream::in);
		if(!in_f){
			cerr <<"не могу открыть файл "<<outfilename<<", останов"<<endl;
			exit(2);
		}
		char s[1000];
		in_f.getline(s,1000);
		if(verbose)		cout<<"прочитана компонента R:"<<endl<<s<<endl;
		{	stringstream str((string)s);	str>>hex_mas(10,R1);	}
		in_f.getline(s,1000);
		if(verbose)		cout<<"прочитана компонента S:"<<endl<<s<<endl;
		{	stringstream str(s);	str>>hex_mas(10,S1);	}
	}
	
	{//скопировать файл
		FILE * in_f=fopen(infilename,"r"), * out_f=fopen(outfilename,"w");
		if(!out_f){
			cerr <<"не могу открыть файл "<<outfilename<<", останов"<<endl;
			exit(2);
		}
		if(!in_f){
			cerr <<"не могу открыть файл "<<infilename<<", останов"<<endl;
			exit(2);
		}
		//cout<<"копирование"<<endl;
		char arr[10000];
		fgets(arr,10000,in_f);
		//cout<<"игнорируем: "<<arr<<endl;
		fgets(arr,10000,in_f);
		//cout<<"игнорируем: "<<arr<<endl;
		while(!feof(in_f)){
			fgets(arr,10000,in_f);
			//cout<<"копируем: "<<arr<<endl;
			fputs(arr,out_f);
		}
		fclose(in_f);
		fclose(out_f);
		if(verbose)
			cout<<"подписанный файл создан"<<endl;
	}
	
	{//посчитать хеш
		FILE * in_f=fopen(outfilename,"r");
		if(!in_f){
			cerr <<"не могу открыть файл "<<infilename<<", останов"<<endl;
			exit(2);
		}
		file_hash(in_f,HH,verbose);
		if(verbose){
			cout<<"хэш входного файла = "<<endl;
			cout<<hex_mas(10,HH)<<endl;
		}
		fclose(in_f);
	}
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
	if(prov_pod(LEN1, GG, HH, Y, R1, S1)){
		if(verbose)
			cout<<endl<<"Подпись верна так как вычисленное значение V равно подписанному значению R"<<endl;
#ifdef MSC_VER
		if(verbose){
			cout <<endl <<endl <<"нажмите любую клавишу... "<<endl;
			getchar();
			cout<<endl;
		}
#endif
		return 0;
	}
	else{
		if(verbose)
			cout<<"\n\nчто-то пошло не так\n";
#ifdef MSC_VER
		if(verbose){
			cout <<endl <<endl <<"нажмите любую клавишу... "<<endl;
			getchar();
			cout<<endl;
		}
#endif
		return 1;
	}
	
}

/* Функция выполняет проверку подписи */
bool prov_pod(int len, const WORD *g1, const WORD * HH1, const WORD *okl, const WORD *R1, const WORD *S1)
{
	WORD M[LEN1],X4[2*LEN1],X5[2*LEN1],X6[LEN1];
	WORD AA[LEN1],BB[LEN1],V[LEN1];

	if(verbose)	cout <<endl<<"Вычисление компонент A,B,V ";
	/* 2) Вычисление А */
	obr(len,S1,QQ,M);
	umn(HH1,len,M,len,X5,2*len);
	mod_p(X5,2*len,QQ,len);
	memcpy(AA,X5,len*sizeof(WORD));

	if(verbose)	cout<<"\nA="<<hex_mas(len-1,AA);

	/* 3) Вычисление В */
	umn(R1,len,M,len,X5,2*len);
	mod_p(X5,2*len,QQ,len);
	memcpy(BB,X5,len*sizeof(WORD));

	if(verbose)	cout<<("\nB=")<<hex_mas(len-1,BB);

	/* 4) Вычисление V */
	step_mod(PP,g1,len,AA,len,X5);
	step_mod(PP,okl,len,BB,len,X6);
	umn(X5,len,X6,len,X4,2*len);
	mod_p(X4,2*len,PP,len);
	mod_p(X4,len,QQ,len);
	memcpy(V,X4,len*sizeof(WORD));

	if(verbose)	cout<<"\nV="<<hex_mas(len-1,V);

	return (cmp(len,V,R1)==-1);
}
