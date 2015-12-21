#include "common.h"
/* Функция выполняет проверку подписи */
void prov_pod(int len, const WORD *g1, const WORD * HH1, const WORD *okl, const WORD *R1, const WORD *S1);

bool verbose;
int main(int argc, const char * argv[]){
	verbose = argc==1;
	char infilename[1000], outfilename[1000];
	if(verbose){
		//прочитать имя входного/выходного файла
		//вывести пример открытого ключа
		//заполнить YY - открытый ключ
	}
	
#ifdef MSC_VER
	if(verbose){
		cout <<endl <<endl <<"нажмите любую клавишу... "<<endl;
		getchar();
		cout<<endl;
	}
#endif
}

/* Функция выполняет проверку подписи */
void prov_pod(int len, const WORD *g1, const WORD * HH1, const WORD *okl, const WORD *R1, const WORD *S1)
{
	WORD M[LEN1],X4[2*LEN1],X5[2*LEN1],X6[LEN1];
	WORD AA[LEN1],BB[LEN1],V[LEN1];

	cout <<endl<<"Вычисление компонент A,B,V ";
	/* 2) Вычисление А */
	obr(len,S1,QQ,M);
	umn(HH1,len,M,len,X5,2*len);
	mod_p(X5,2*len,QQ,len);
	memcpy(AA,X5,len*sizeof(WORD));

	cout<<"\nA="<<hex_mas(len,AA);

	/* 3) Вычисление В */
	umn(R1,len,M,len,X5,2*len);
	mod_p(X5,2*len,QQ,len);
	memcpy(BB,X5,len*sizeof(WORD));

	cout<<("\nB=")<<hex_mas(len,BB);

	/* 4) Вычисление V */
	step_mod(PP,g1,len,AA,len,X5);
	step_mod(PP,okl,len,BB,len,X6);
	umn(X5,len,X6,len,X4,2*len);
	mod_p(X4,2*len,PP,len);
	mod_p(X4,len,QQ,len);
	memcpy(V,X4,len*sizeof(WORD));

	cout<<"\nV="<<hex_mas(len,V);

	if(cmp(len,V,R1)==-1)
		cout<<endl<<endl<<"Подпись верна так как вычисленное значение V равно подписанному значению R"<<endl;
	else
		cout<<"\n\nчто-то пошло не так\n";
	return;
}
