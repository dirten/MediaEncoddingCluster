package gmf



type DataSink struct{
  Locator MediaLocator;
  ctx * FormatContext
  Valid bool
}

func (src * DataSink) Connect() bool{
  src.Valid=false
  src.ctx=avformat_alloc_context();
  format:=av_guess_format(src.Locator.Format,src.Locator.Filename)
  src.ctx.ctx.oformat=(*_Ctype_struct_AVOutputFormat)(format.format)

  result:=url_fopen(src.ctx, src.Locator.Filename);

  if(result!=0){
    return src.Valid
  }
  src.Valid=true
  return src.Valid;
}

func (src * DataSink) Disconnect() bool{
  if(src.Valid){
    url_fclose(src.ctx);
  }
  av_free(src.ctx)
  return true;
}
