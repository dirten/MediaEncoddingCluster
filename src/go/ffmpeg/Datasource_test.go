package gmf


import "testing"


func TestDatasourceConnect(t*testing.T){
    println("start func TestDatasourceConnect(t*testing.T){")
    loc:=MediaLocator{Filename:"/media/video/ChocolateFactory.ts"}
    source:=DataSource{Locator:loc}
    source.Connect()
    
    source.Disconnect()
    println("end func TestDatasourceConnect(t*testing.T){")
    
}

