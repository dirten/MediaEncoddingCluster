package hive
import "gmf"
func CreateEncoder(codec * Codec)*gmf.Encoder{
    result:=new(gmf.Encoder)
    result.SetParameter("codecid",codec.Id)
    for i:=0;i<len(codec.Param);i++{
	result.SetParameter(codec.Param[i].Name,codec.Param[i].Value)
    }
    return result
}