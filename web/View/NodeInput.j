
@import "NodeView.j"

@implementation NodeInput: NodeView
{
 CPDictionary    data @accessors(property=data);
 id json;

}
-(id)init
{
  self=[super initWithName:@"Input" withInputHandle:NO andOutputHandle:YES taskName:@"InputTask"];
  json={
    "data":{
      "infile":""
    }
  };
  data=[CPDictionary dictionaryWithJSObject:json recursively:YES];  
  return self;
}

-(id)propertyView
{
  var bounds=CPRectMake(0,0,450,100);
  var view=[[CPView alloc] initWithFrame:bounds];
  [view setBounds:bounds];
  //[_options setObject:data forKey:data.id];
  label=[CPTextField labelWithTitle:@"Input File:"];
  [label setFrameOrigin:CGPointMake(5.0,10.0)];
  [label setAlignment:CPRightTextAlignment];
  //var pdata=[data objectForKey:@"data"];
  var field = [TextBox textFieldWithStringValue:@"" placeholder:@"type in file location" width:350];
  controller=[[CPObjectController alloc] init];
  [controller bind:@"contentObject" toObject:self withKeyPath:@"data" options:nil];    
  [field 
     bind:CPValueBinding 
     toObject:controller 
     withKeyPath:@"selection.data.infile" 
     options:nil
  ];
  [field setFrameOrigin:CGPointMake(60.0,5.0)];
  [field setDelegate:self];
  [field setIdentifier:@"infile"];

  [view addSubview:label];
  [view addSubview:field];

  //CPLog.debug("orig bounds:"+CPStringFromRect(bounds));
  //CPLog.debug("orig View with bounds:"+CPStringFromRect([view bounds]));
  return view;
}
-(id)image
{
    var image = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Downloads-alt-icon.png"] size:CPSizeMake(100, 50)];
  return image;
}
-(id)properties
{
  return nil;
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