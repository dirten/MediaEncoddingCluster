package main

import "flag"
import "os"
import "bytes"
import "xml"
import "strings"

type Param struct{
    Name string "attr"
    Value string "attr"
}

type Codec struct{
    Type string "attr"
    Id string "attr"
    Parameter []Param
}

type Format struct{
    Id string "attr"
    Parameter []Param
}

type Preset struct{
    XMLName xml.Name "preset"
    Format Format
    Codec []Codec
}

func main(){
    flag.Parse()
    buff := bytes.NewBuffer (make ([]byte, 0));
    buff.ReadFrom (os.Stdin);
//    println(num)
//    println(buff.String())    
    dataReader := strings.NewReader(buff.String())
    preset:=new(Preset)
    err:=xml.Unmarshal(dataReader, preset)
    if(err != nil){
        println(err)
    }
    println("data parsed without error")
    println(preset.Codec[0].Id)
}

