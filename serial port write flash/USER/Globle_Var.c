#include "Globle_Var.h"



/*

�ֿ��СΪ8178*32=261696�ֽ�
flash��СΪ0~1fffffh
1fffffh=2097151d
2097151-261696=1835455
1835455d=1c01bfh
1c01bfh+1h=1c01c0h

����.fon�ļ������������ֽ�

1C01C0h-2=1C01BEh

��flash���261698�ֽڷ��ֿ�

*/
uint32_t GWriteAddr=0x1C01BE;

