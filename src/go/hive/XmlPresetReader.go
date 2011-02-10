package hive

import "io/ioutil"
import "xml"
import "strings"
import "os"
import "fmt"

type Param struct{
    Name string "attr"
    Value string "attr"
}

type Codec struct{
    Type string "attr"
    Id string "attr"
    Param []Param
}

type Format struct{
    Id string "attr"
    Param []Param
}

type Preset struct{
    XMLName xml.Name "preset"
    Format Format
    Codec []Codec
}

func LoadPreset(file string)(*Preset, os.Error){
    buffer,err:=ioutil.ReadFile(file)
    if(err != nil){
	return nil,err
    }
    dataReader := strings.NewReader(string(buffer))
    preset:=new(Preset)
    err=xml.Unmarshal(dataReader, preset)
    if(err != nil){
	return nil,err
    }
    println("data parsed without error")
    fmt.Println(preset)
    return preset,nil
}

