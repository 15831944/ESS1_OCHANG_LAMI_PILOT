extern "C"
{
int _stdcall pIO_init();                 //�ʱ�ȭ 
long _stdcall pGet_in(int);              //X��ü  �Է°��� �о���δ�. 
int _stdcall pGet_in_bit(int,int);       //X �Է°��� �о���δ�.(��Ʈ����)
int _stdcall pSet_out(int, long);        //Y ��ü ����� ��������. 
int _stdcall  pSet_out_bit(int, short);  //Y ����� ��������.
int _stdcall  pReset_out_bit(int, short);//Y ����� ��������.(��Ʈ����)
long _stdcall pGet_out(int);             //Y ��°��� �о���δ�. 
int _stdcall pGet_out_bit(int,short);    //Y ��°��� �о���δ�.(��Ʈ����)
int _stdcall pIO_end();                  //����
}