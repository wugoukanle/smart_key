#include "Globle_Var.h"



/*

字库大小为8178*32=261696字节
flash大小为0~1fffffh
1fffffh=2097151d
2097151-261696=1835455
1835455d=1c01bfh
1c01bfh+1h=1c01c0h

由于.fon文件最后多了两个字节

1C01C0h-2=1C01BEh

即flash最后261698字节放字库

*/
uint32_t GWriteAddr=0x1C01BE;

