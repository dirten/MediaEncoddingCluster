
@import "NodeView.j"

@implementation NodeExecutable: NodeView
{
 CPDictionary    data @accessors(property=data);
}
-(id)init
{
  self=[super initWithName:@"Executable" withInputHandle:YES andOutputHandle:YES taskName:@"ExecutableTask"];
  json={
    "data":{
      "executable":""
    }
  };
  data=[CPDictionary dictionaryWithJSObject:json recursively:YES];  
  return self;
}
-(id)image
{
  var image = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Developer-icon.png"] size:CPSizeMake(100, 50)];
  return image;
}
-(CPString)frontLabel
{
  return [[data objectForKey:@"data"] objectForKey:@"executable"];
}
-(id)menuForNodeItem
{
  return [self menuItems:["Properties..."] forActions:[@selector(open:)]];
}

-(void)open:(id)aSender
{
  CPLog.debug("open clicked:"+aSender);
  var view=[self propertyView];
  if(!view)return;
  CPLog.debug("View Bounds:"+CPStringFromRect([view bounds]));
  var propertyWindow=[[NodePropertyWindow alloc] initWithView:view] ;
  [[propertyWindow contentView] addSubview:view];
  CPLog.debug("open property window");

}

-(id)propertyView
{
  var bounds=CPRectMake(0,0,450,100);
  var view=[[CPView alloc] initWithFrame:bounds];
  [view setBounds:bounds];
  //[_options setObject:data forKey:data.id];
  label=[CPTextField labelWithTitle:@"Executable:"];
  [label setFrameOrigin:CGPointMake(5.0,10.0)];
  [label setAlignment:CPRightTextAlignment];
  //var pdata=[data objectForKey:@"data"];
  var field = [TextBox textFieldWithStringValue:@"" placeholder:@"type in executable location" width:350];
  controller=[[CPObjectController alloc] init];
  [controller bind:@"contentObject" toObject:self withKeyPath:@"data" options:nil];    
  [field 
     bind:CPValueBinding 
     toObject:controller 
     withKeyPath:@"selection.data.executable" 
     options:nil
  ];
  [field setFrameOrigin:CGPointMake(60.0,5.0)];
  [field setDelegate:self];
  [field setIdentifier:@"executable"];

  [view addSubview:label];
  [view addSubview:field];

  //CPLog.debug("orig bounds:"+CPStringFromRect(bounds));
  //CPLog.debug("orig View with bounds:"+CPStringFromRect([view bounds]));
 [[CPNotificationCenter defaultCenter]
    postNotificationName:NodeEditorViewChanged
    object:self
    userInfo:nil];
  [[CPRunLoop currentRunLoop] performSelectors]; 

  return view;
}
@end