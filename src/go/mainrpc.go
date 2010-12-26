package main;

import "fmt"
import "flag"
import "rpc"
import "net"
import "log"
import "http"
import "os"
import "exec"
//import "bytes"

var processMap map[string]*Command = make(map[string]*Command)
type Command struct{
    name string
    command string
    args []string
    dir string
    cmd *exec.Cmd
}
var processChannel=make(chan *Command)

type ProcessStarter int

func (p*ProcessStarter)Stop(com *Command, reply*string) os.Error {
    return nil
}

func (p*ProcessStarter)Start(com *Command, reply*string) os.Error {
    fmt.Println(""+com.command)
    //args := []string{com.command, "-c", "ls"} 
    go func(){
        //args := []string{com.command, "-c", "ls"}
        prc, err := exec.Run(com.command, com.args, nil,com.dir,exec.DevNull, exec.Pipe, exec.PassThrough)
        fmt.Println("ProcessStarted ")
        //pid, err := os.ForkExec(com.command, com.args, nil,com.dir,nil)
        if err != nil {
            fmt.Println("error start process")
        }
        com.cmd=prc
        processChannel<-com
        prc.Close()
        fmt.Println("Process Stopped ")
    }()
    return nil
}

func server(){
    ps:=new(ProcessStarter)
    rpc.Register(ps)
    rpc.HandleHTTP()
    l, e := net.Listen("tcp", ":1234")
    if e != nil {
	log.Exit("listen error:", e)
    }
    go http.Serve(l, nil)
}

func client(host string, port int){
    client, err := rpc.DialHTTP("tcp", host + ":"+fmt.Sprintf("%d",port))
    if err != nil {
        log.Exit("dialing:", err)
    }

    var reply string
    err = client.Call("ProcessStarter.Start", processMap["server"], &reply)
    if err != nil {
	log.Exit("error:", err)
    }
//    fmt.Printf("Arith: %d*%d=%d", args.A, args.B, *reply)

}

func processListener(pMap map[string]*Command){
    for true {
        command:=<-processChannel        
        fmt.Printf("ProcessStarted pid=%d\n", command.cmd.Pid)
        _, ok:=pMap["serv"]
        if(!ok){
            pMap["server"]=command
            fmt.Printf("setze server %s:", command.name)
        }
        
    }
}

func main(){
    processMap["server"]=&Command{name:"serverprocess",command:"mhive",dir:"/home/HoelscJ/devel/mec/install/bin",args:[]string{"mhive","-r"} }
    processMap["client"]=&Command{name:"clientprocess",command:"mhive",dir:"/home/HoelscJ/devel/mec/install/bin",args:[]string{"mhive","-i"} }
    processMap["web"]=   &Command{name:"webprocess",command:"mhive",dir:"/home/HoelscJ/devel/mec/install/bin",args:[]string{"mhive","-w"} }

    flag.Parse()
    if(flag.Arg(0)=="s") {
	fmt.Println("starting server")
        go processListener(processMap)
	server();
	<-make(chan int)
    }else{
	fmt.Println("starting client")
	client(flag.Arg(0), 1234)
    }
}




