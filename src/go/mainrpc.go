package main;

import "fmt"
import "flag"
import "rpc"
import "net"
import "log"
import "http"
import "os"
import "exec"
import "path"
//import "syscall"
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
	fmt.Printf("try stopping process\n")
        data, ok:=processMap[com.name]
        if(!ok){
            fmt.Printf("server unknown:%s", com.name)
        }else{
	    if(data.cmd!=nil&&data.cmd.Pid>0){
		fmt.Printf("kill pid server %d:", data.cmd.Pid)
		var pid string=fmt.Sprintf("%d", data.cmd.Pid)
		println(pid)
		args := []string{com.command, "--stop", pid}
    		env := []string{"DYLD_LIBRARY_PATH=/Users/jholscher/Documents/bripper/install/lib/"}
		_, err := exec.Run(com.command, args, env,com.dir,exec.DevNull, exec.Pipe, exec.PassThrough)
		if err != nil {
        	    fmt.Println("error stop process")
    		}else{
        	    //fmt.Println("process stopped")
        	}

//		syscall.Kill(data.cmd.Pid, 15)
	    }else{
		fmt.Printf("failed to stop the process, because one of the following data is nil or lower 0 [%s,%d]", data.cmd, data.cmd.Pid)
	    }
	}

    return nil
}

func (p*ProcessStarter)Start(com *Command, reply*string) os.Error {
    fmt.Println(""+com.command)
    //args := []string{com.command, "-c", "ls"} 
    go func(){
        //args := []string{com.command, "-c", "ls"}
        env := []string{"DYLD_LIBRARY_PATH=/Users/jholscher/Documents/bripper/install/lib/"}
        prc, err := exec.Run(com.command, com.args, env,com.dir,exec.DevNull, exec.Pipe, exec.PassThrough)
        fmt.Println("ProcessStarted ")
        //pid, err := os.ForkExec(com.command, com.args, nil,com.dir,nil)
        if err != nil {
            fmt.Println("error start process")
        }else{
    	    com.cmd=prc
    	    pid:=com.cmd.Pid
    	    processListener(com)
    	    prc.Close()
    	    fmt.Printf("Process Stopped with pid=%d\n", pid)
    	    com.cmd=nil;
	}
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
func stopclient(host string, port int){
    client, err := rpc.DialHTTP("tcp", host + ":"+fmt.Sprintf("%d",port))
    if err != nil {
        log.Exit("dialing:", err)
    }

    var reply string
    err = client.Call("ProcessStarter.Stop", processMap["server"], &reply)
    if err != nil {
	log.Exit("error:", err)
    }
//    fmt.Printf("Arith: %d*%d=%d", args.A, args.B, *reply)

}

func processListener(command *Command){
//    for true {
//        command:=<-processChannel        
        fmt.Printf("ProcessStarted pid=%d\n", command.cmd.Pid)
        data, ok:=processMap["server"]
        if(!ok){
            processMap["server"]=command
            fmt.Printf("setze server %s:", command.name)
        }else{
	    if(data.cmd==nil){
		fmt.Printf("setze data.cmd server %s:", data.cmd)
		data.cmd=command.cmd
	    }else{
		fmt.Printf("last pid server %d:", data.cmd.Pid)
	    }
	}
//    }
}

func main(){
    mypath,_:=os.Getwd()
    println(mypath+":"+os.Args[0])
    d, _ := path.Split(os.Args[0])
    os.Chdir(d)
    fmt.Println(os.Getwd()) 
    mypath,_=os.Getwd()
    println(mypath+":"+os.Args[0])

    processMap["server"]=&Command{name:"server",command:"mhive",dir:mypath,args:[]string{"mhive","-r"} }
    processMap["client"]=&Command{name:"client",command:"mhive",dir:mypath,args:[]string{"mhive","-i"} }
    processMap["web"]=   &Command{name:"web",command:"mhive",dir:mypath,args:[]string{"mhive","-w"} }

    flag.Parse()
    if(flag.Arg(0)=="s") {
	fmt.Println("starting server")
        //go processListener(processMap)
	server();
	<-make(chan int)
    }else if(flag.Arg(0)=="stop"){
	fmt.Println("stopping client")
	stopclient("localhost", 1234)
    }else{
	fmt.Println("starting client")
	client(flag.Arg(0), 1234)
    }
}




