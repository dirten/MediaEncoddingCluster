package main;

import "fmt"
import "net"
import "bytes"
import "flag"
import "time"
//import "hive/net"

func sendUdp(){
    for i:=0; i < 10; i++ {
    if laddr, err := net.ResolveUDPAddr("239.255.0.1:6000"); err !=nil {
	fmt.Println(err)
    }else{
	conn, _ := net.ListenPacket("udp4", "0.0.0.0:0")
	fmt.Println(laddr)
	fmt.Println("ListenPacket:")
	//socket := conn.(*net.UDPConn)
	buf := bytes.NewBufferString("test message")
	message:=buf.Bytes()
	if n, err := conn.WriteTo(message, laddr); err!=nil{
	//if n, err := socket.WriteToUDP(message, laddr); err!=nil{
	    fmt.Println("error write udp")
	}else{	
	    fmt.Println("write udp")
	    fmt.Println(n)

	}
    }
    time.Sleep(1*1000000000)
    }
}
func recvUdp(){
    if conn, err := net.ListenPacket("udp4", "0.0.0.0:6000"); err!=nil{
	fmt.Println("error openning receiver Listener")
	fmt.Println(err)
    }else{
	//socket := conn.(*net.UDPConn)
	answerBytes := make([]byte, 1024)
	if n, _, err := conn.ReadFrom(answerBytes); err!=nil {
	    fmt.Println("erro read from Udp")
	}else{
	    fmt.Println("read socket succeed")
	    fmt.Println(n)
	    fmt.Println(answerBytes)
	    answer := string(answerBytes[0:n])
	    fmt.Println(answer)
	}
    }
    fmt.Println("exit recv udp")
}
func recv(a chan string){
    grus:=<-a
    fmt.Println(grus);
}

var port=flag.Int("port", 6000, "Auto Discovery Port number")

//var dim = flag.Int("dim", 16, "board dimensions")

func main(){
    flag.Parse();
    fmt.Println("starte test")
    //go recvUdp()
    //time.Sleep(1*1000000000)
    sendUdp()
    time.Sleep(50*1000000000)
}

