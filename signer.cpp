#include "common.h"
 /* Функция выполняет вычисление подписи */
void form_pod(int len, const WORD *g1, const WORD * HH, const WORD *skl, WORD *R1, WORD *S1);

bool verbose;
int main(int argc, const char * argv[]){
	
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

	cout<<endl<<"-Подпись сообщения-"<<endl
		<<"Компонента R = "<<hex_mas(len,R1);

	/* 3) Вычисление S компоненты подписи */

	umn(XX,len,R1,len,X5,2*len);
	mod_p(X5,2*len,QQ,LEN1);
	plus(len,X5,HH,X6);
	mod_p(X6,len,QQ,LEN1);
	obr(len,skl,QQ,M);
	umn(X6,len,M,len,X5,2*len);
	mod_p(X5,2*len,QQ,LEN1);
	memcpy(S1,X5,len*sizeof(WORD));

	cout<<endl
		<<"Компонента S = "<<hex_mas(len,S1)<<endl;
	return;

}
