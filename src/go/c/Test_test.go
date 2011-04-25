package mytest



import "testing"
import "fmt"

func TestIntPointer(t*testing.T){
    println("Testing int Pointer")
    test_int_p()
}

func TestCharPointer(t*testing.T){
    println("Testing char Pointer")
    test_char_p()
}
func TestCharPointerByPointer(t*testing.T){
    println("Testing char Pointer")
    data:=make([]byte, 20)
    test_char_p_by_p(data)
    fmt.Printf("data received:%s\n",data)
}
func TestUint8tPointerByPointer(t*testing.T){
    println("Testing uint8t Pointer")
    data:=make([]byte, 20)
    test_uint8t_p_by_p(data)
    fmt.Printf("data received:%s\n",data)
}
func TestMalloc(t*testing.T){
    println("Testing malloc")
	test_mal()

}

