#include "runtime.h"
#include "malloc.h"
#include "type.h"

#line 9 "test.goc"
static uint32 
fastrand1 ( void ) 
{ 
static uint32 x = 0x49f6428aUL; 
#line 14 "test.goc"
x += x; 
if ( x & 0x80000000L ) 
x ^= 0x88888eefUL; 
return x; 
} 
void
mytest·MySetFinalizer()
{
#line 20 "test.goc"


}
void main·main()
{

}
void main·init()
{

}
