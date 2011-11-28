
@import "NodeView.j"
@import "ProfileEditView.j"
@import "../Categories/CPDictionary+toJSON.j"
@implementation NodeEncoding: NodeView
{
  CPDictionary    data @accessors(property=data);
  id json;
}
-(id)init
{
  self=[super initWithName:@"Encoding" withInputHandle:YES andOutputHandle:YES taskName:@"EncodingTask"];
  json={
    "data":{
      "format":{},
      "video":{},
      "audio":{}
    }
  };
  data=[CPDictionary dictionaryWithJSObject:json recursively:YES];  
  CPLog.debug("Data in Contruct:"+data);
  return self;
}

-(void)drawContentsInView:(id)view inRect:(id)aRect
{
  [super drawContentsInView:view inRect:aRect];
}
-(id)propertyView
{
  var view=[[ProfileEditView alloc] initWithData:data];
  [view setProfileId:0];
  return view;
}
-(id)initWithCoder:(CPCoder)aCoder
{
  CPLog.debug("initWithCoder");
  self = [super initWithCoder:aCoder];
  if(self){
    data=[aCoder decodeObjectForKey:"data"];
    json=[aCoder decodeObjectForKey:"json"];
  }
  return self;
}

-(void)encodeWithCoder:(CPCoder)aCoder
{
  [super encodeWithCoder:aCoder];
  [aCoder encodeObject:data forKey:@"data"];
  [aCoder encodeObject:json forKey:@"json"];
}

@end