
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
-(CPString)frontLabel
{
  return [[data objectForKey:@"data"] objectForKey:@"outfile"];
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
  var bounds=CPRectMake(0,0,500,80);
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
  var fileChooser=[[FileChooser alloc] initWithPath:@"/" andTitle:@"Output File Chooser"];
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
      if(!name=="/")
        name+="/";
      name+=[[chooser selectedItem] name];
    }
    [[data objectForKey:@"data"] setObject:name forKey:@"outfile"];
  }
}

-(id)image
{
    var image = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Torrents-icon.png"] size:CPSizeMake(100, 50)];
  return image;
}

@end