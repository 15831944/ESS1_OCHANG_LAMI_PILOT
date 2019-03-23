extern "C"
{
int _stdcall pIO_init();                 //초기화 
long _stdcall pGet_in(int);              //X전체  입력값을 읽어들인다. 
int _stdcall pGet_in_bit(int,int);       //X 입력값을 읽어들인다.(비트별로)
int _stdcall pSet_out(int, long);        //Y 전체 출력을 내보낸다. 
int _stdcall  pSet_out_bit(int, short);  //Y 출력을 내보낸다.
int _stdcall  pReset_out_bit(int, short);//Y 출력을 내보낸다.(비트별로)
long _stdcall pGet_out(int);             //Y 출력값을 읽어들인다. 
int _stdcall pGet_out_bit(int,short);    //Y 출력값을 읽어들인다.(비트별로)
int _stdcall pIO_end();                  //종료
}