package mytest

//enum TestEnum {
//	NONE=-1,
//	ZERO,
//	ONE,
//	TWO
//};
//typedef struct tests{
//	enum TestEnum enumvar;
//}tests;
//void test_int_p(int * size);
//void test_char_p(char * size);
//#include <string.h>
//#include <inttypes.h>
//void test_int_p(int * size){
//	*size=3;
//}
//void test_char_p(char * data){
//	memcpy(data,"new text here",13);
//}
//void test_uint8t_p(uint8_t * data){
//	memcpy(data,"new text here",13);
//}
import "C"
import "unsafe"
import "fmt"
func test_int_p(){
    println("test_int_p")
    var d=0
    println(d)
    C.test_int_p((*C.int)(unsafe.Pointer(&d)))
    println(d)
}
func test_char_p(){
    println("test_char_p")
    var d=make([]byte, 20)
    fmt.Printf("data %s\n",d)
    C.test_char_p((*C.char)(unsafe.Pointer(&d[0])))
    fmt.Printf("data %s\n",d)
//    println(d)
}
func test_char_p_by_p(data []byte){
    println("test_char_p")
    //var d=make([]byte, 20)
    fmt.Printf("data %s\n",data)
    C.test_char_p((*C.char)(unsafe.Pointer(&data[0])))
    fmt.Printf("data %s\n",data)
//    println(d)
}
func test_enum(){
    var teststruct * C.tests=new(C.tests)
    teststruct.enumvar=C.NONE
}
func test_uint8t_p_by_p(data []byte){
    println("test_char_p")
    //var d=make([]byte, 20)
    fmt.Printf("data %s\n",data)
    C.test_uint8t_p((*C.uint8_t)(unsafe.Pointer(&data[0])))
    fmt.Printf("data %s\n",data)
//    println(d)
}
//export exportTest
func exportTest(data string){
    //MySetFinalizer()
}
