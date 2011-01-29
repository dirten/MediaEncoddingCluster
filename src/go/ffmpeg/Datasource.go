package gmf



type DataSource struct{
  Locator MediaLocator;
  ctx * FormatContext
  Valid bool
}


func (src * DataSource) Connect() bool{
  src.Valid=false
  src.ctx=avformat_alloc_context();
  result:=av_open_input_file(src.ctx, src.Locator.Filename, nil,0,nil);
  if(result!=0){
    return src.Valid
  }
  result=av_find_stream_info(src.ctx)
  if(result<0){
    return src.Valid
  }
  src.Valid=true
  return src.Valid;
}

func (src * DataSource) Disconnect() bool{
  if(src.Valid){
    av_close_input_file(src.ctx);
  }
  return true;
}

func (src * DataSource) GetContentType() string{
  return "video/ffmpeg";
}
