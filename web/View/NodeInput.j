
@import "NodeView.j"
@import "FileChooser.j"
@implementation NodeInput: NodeView
{
 CPDictionary    data @accessors(property=data);
 id json;
  //TextBox field;
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
  [self setTaskDescription:@"The Input Type will copy the Source File into the workspace. It should define a local File on the Server."];
  return self;
}
-(CPString)frontLabel
{
  return [[data objectForKey:@"data"] objectForKey:@"infile"];
}
-(id)propertyView
{
  var bounds=CPRectMake(0,0,500,80);
  var view=[[CPView alloc] initWithFrame:bounds];
  [view setBounds:bounds];
  //[_options setObject:data forKey:data.id];
  label=[CPTextField labelWithTitle:@"Input File:"];
  [label setFrameOrigin:CGPointMake(5.0,10.0)];
  [label setAlignment:CPRightTextAlignment];
  //var pdata=[data objectForKey:@"data"];
  //var field = [TextBox textFieldWithStringValue:@"" placeholder:@"type in file location" width:350];
  var field = [CPTextField textFieldWithStringValue:@"" placeholder:@"type in file location" width:CGRectGetWidth([view bounds])-90];
  
  //var field = [[LPMultiLineTextField alloc] initWithFrame:CGRectMake(0,0,350,50)];// textFieldWithStringValue:@"" placeholder:@"type in file location" width:350];
  /*
  var field = [[LPMultiLineTextField alloc] initWithFrame:CGRectMake(0.0, 0.0, 350, 129.0)];
  [field setTheme:[CPTheme defaultTheme]];
  [field setPlaceholderString:@"type in file location"];
  [field setBordered:YES];
  [field setBezeled:YES];
  [field setEditable:YES];
  [field sizeToFit];
  */
  

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
  //[field setIdentifier:@"infile"];

  var selectButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([view bounds])-90,7.0,80.0,24.0)];
  [selectButton setTitle:@"Select File"];
  //[selectButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [selectButton setTarget:self];
  [selectButton setAction:@selector(openBrowser)];


  [view addSubview:label];
  [view addSubview:field];
  [view addSubview:selectButton];

  //CPLog.debug("orig bounds:"+CPStringFromRect(bounds));
  //CPLog.debug("orig View with bounds:"+CPStringFromRect([view bounds]));
 [[CPNotificationCenter defaultCenter]
    postNotificationName:NodeEditorViewChanged
    object:self
    userInfo:nil];
  [[CPRunLoop currentRunLoop] performSelectors]; 

  return view;
}

-(void)openBrowser{
  CPLog.debug("open browser");
  var fileChooser=[[FileChooser alloc] initWithPath:@"/" andTitle:@"Input File Chooser"];
  [fileChooser setAllowsMultipleSelection:NO];
  [fileChooser setTarget:self];
  [fileChooser setAction:@selector(fileSelected:)];

}

-(void)fileSelected:(id)chooser
{
  CPLog.debug("Chooser="+[[chooser selectedItem] path]+"/"+[[chooser selectedItem] name]);
  if([[chooser selectedItems] count]){
    var name=[[chooser selectedItem] path];
    if(![[chooser selectedItem] isDirectory]){
      if(name!="/")
        name+="/";
      name+=[[chooser selectedItem] name];
    }
    [[data objectForKey:@"data"] setObject:name forKey:@"infile"];
  }
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