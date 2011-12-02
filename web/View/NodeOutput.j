
@import "NodeView.j"

@implementation NodeOutput: NodeView
{
 CPDictionary    data @accessors(property=data);
}
-(id)init
{
  self=[super initWithName:@"Output" withInputHandle:YES andOutputHandle:NO taskName:@"OutputTask"];
  json={
    "data":{
      "outfile":""
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
  label=[CPTextField labelWithTitle:@"Output File:"];
  [label setFrameOrigin:CGPointMake(5.0,10.0)];
  [label setAlignment:CPRightTextAlignment];
  //var pdata=[data objectForKey:@"data"];
  var field = [TextBox textFieldWithStringValue:@"" placeholder:@"type in file location" width:350];
  controller=[[CPObjectController alloc] init];
  [controller bind:@"contentObject" toObject:self withKeyPath:@"data" options:nil];    
  [field 
     bind:CPValueBinding 
     toObject:controller 
     withKeyPath:@"selection.data.outfile" 
     options:nil
  ];
  [field setFrameOrigin:CGPointMake(60.0,5.0)];
  [field setDelegate:self];
  [field setIdentifier:@"outfile"];

  [view addSubview:label];
  [view addSubview:field];

  //CPLog.debug("orig bounds:"+CPStringFromRect(bounds));
  //CPLog.debug("orig View with bounds:"+CPStringFromRect([view bounds]));
  return view;
}
-(id)image
{
    var image = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Torrents-icon.png"] size:CPSizeMake(100, 50)];
  return image;
}

@end