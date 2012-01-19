
@import <AppKit/CPPopover.j>
@import "NodeView.j"
@import "ProfileEditView.j"
@import "../Categories/CPDictionary+toJSON.j"
@import "ProfileSelector.j"

@implementation NodeEncoding: NodeView
{
  CPDictionary    data @accessors(property=data);
  id json;
  CPPopover   popoverView;
  CPViewController viewController;
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
  [self setTaskDescription:@"The Encoding Type will process the Source File from the workspace. It should define the target File Format end Encoding options."];
  //popoverView=[[CPPopover alloc] init];
  //viewController=[[CPViewController alloc] init];
  //[popoverView setContentViewController:viewController];
  //[self setLabelText:@""];
  return self;
}

-(CPString)frontLabel
{
  return [[data objectForKey:@"data"] objectForKey:@"name"];
}

-(void)setData:(id)ndata
{
  data=ndata;
  //[self setLabelText:[[data objectForKey:@"data"] objectForKey:@"name"]];
  //[self setLabelText:@"name"];
  CPLog.debug("Data in setData:"+[data objectForKey:@"name"]);
}

-(void)drawContentsInView:(id)view inRect:(id)aRect
{
  [super drawContentsInView:view inRect:aRect];
}
-(id)propertyView
{
  [[CPNotificationCenter defaultCenter]
            postNotificationName:StartWaitingSpinner
            object:self
            userInfo:nil];
  //[[TNGrowlCenter defaultCenter] pushNotificationWithTitle:@"Loading Property View." message:"please wait!"];
  //[[CPRunLoop currentRunLoop] performSelectors]; 
/* 
[[CPNotificationCenter defaultCenter]
    postNotificationName:NodeEditorViewChanged
    object:self
    userInfo:nil];
*/
  [[CPRunLoop currentRunLoop] performSelectors]; 

  var view=[[ProfileEditView alloc] initWithData:data];
  [view setProfileId:0];
  return view;
}

-(id)menuForNodeItem
{
//  return [self menuItems:["Load existing Profile ...","Properties"] forActions:[@selector(loadProfile:),@selector(open:)]];
  
  //return nil;
  menu = [[CPMenu alloc] initWithTitle:"Null Menu"],
  profileMenu = [[CPMenu alloc] initWithTitle:"Profiles"],
  profileMenuItem = [[CPMenuItem alloc] initWithTitle:"Load existing Profile ..." action:nil keyEquivalent:nil],
  menuItems = ["No Context Menu for this Item"],
  //menuItems = [],
  menuActions = [nil],
  //menuActions = [],
  //isOpen = displayedIssuesKey === "openIssues",
  count = menuItems.length,
  i = 0,
  //[menu addItem:profileMenu];
  numberOfSelectedIssues=1;
  var request = [CPURLRequest requestWithURL:@"/api/v1/profile"];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("NodeEncoding Load Result"+[result rawString]);
  var json=[[result rawString] objectFromJSON];
  [profileMenuItem setSubmenu:profileMenu];
  [menu addItem:profileMenuItem];
  
  for (; i < json.data.length; i++)
  {
    var newMenuItem = [[CPMenuItem alloc] initWithTitle:json.data[i].name action:@selector(loadProfile:) keyEquivalent:nil];
    
    [newMenuItem setRepresentedObject:json.data[i].id];
    [newMenuItem setTarget:self];

    [profileMenu addItem:newMenuItem];  
  }
  var newMenuItem = [[CPMenuItem alloc] initWithTitle:@"Properties..." action:@selector(open:) keyEquivalent:nil];
  //[newMenuItem setEnabled:YES];
  [newMenuItem setTarget:self];
  
  [menu addItem:newMenuItem];  
  return menu;
}

}

-(id)image
{
  var image = [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Enhanced-Labs-icon.png"] size:CPSizeMake(100, 50)];
  return image;
}

-(void)loadProfile:(id)sender
{
  var profileId=[sender representedObject];
  var request = [CPURLRequest requestWithURL:@"/api/v1/profile?id="+profileId];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("NodeEncoding Load Profile Result"+[result rawString]);
  var json=[[result rawString] objectFromJSON];
  data=[CPDictionary dictionaryWithJSObject:json recursively:YES];  

  CPLog.debug("LoadProfile:"+[sender title]);
  //[self setLabelText:[[data objectForKey:@"data"] objectForKey:@"name"]];
  
  //[self setNeedsDisplay:YES];
 [[CPNotificationCenter defaultCenter]
    postNotificationName:RefreshNodeEditorView
    object:self
    userInfo:nil];
  [[TNGrowlCenter defaultCenter] pushNotificationWithTitle:@"Profile successful loaded." message:"Profile "+[[data objectForKey:@"data"] objectForKey:@"name"]+" successful loaded."];
 [[CPNotificationCenter defaultCenter]
    postNotificationName:NodeEditorViewChanged
    object:self
    userInfo:nil];
  [[CPRunLoop currentRunLoop] performSelectors]; 
  //[popoverView showRelativeToRect:nil ofView:self preferredEdge:CPRectMake(100,100,200,200)];
}

-(void)open:(id)aSender
{
  [[TNGrowlCenter defaultCenter] pushNotificationWithTitle:@"Loading Property View." message:"please wait!"];
  CPLog.debug("open clicked:"+aSender);
  [CPTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(_open) userInfo:nil repeats:NO];

}
-(void)_open
{
  var view=[self propertyView];
  if(!view)return;
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