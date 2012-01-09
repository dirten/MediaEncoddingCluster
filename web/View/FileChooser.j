
@implementation Node : CPObject
{
    CPString name;
    CPString path;
    BOOL isDirectory;
    id value;
    CPArray children;
}

+(id)withData:(id)data
{
  var a = [[self alloc] init];
  a.children=[CPArray array];
  a.name = data.name;
  a.path = data.path;
  a.isDirectory = data.directory;
  return a;
}

+ (id)withValue:(id)aValue
{
    var a = [[self alloc] init];
    a.value = aValue;
    return a;
}

- (id)value
{
    return value;
}
- (BOOL)isDirectory
{
    return isDirectory;
}

- (id)path
{
    return path;
}

- (id)name
{
    return name;
}
-(void)clearChild
{
  children=[CPArray array];
}
-(id)sort:(id)obj
{
  //CPLog.debug("Sort:"+obj);
  return name>[obj name];
}

-(void)addChild:(id)node
{
  [children addObject:node];
  //CPLog.debug("add Node"+[children count]);
}

- (CPArray)children
{
    return children;//[children sortedArrayUsingSelector:@selector(sort:)];
}

@end


@implementation FileChooser:CPWindow
{
  Node rootNode;
  CPBrowser browser;
  SEL _selector;
  id _target;
}

-(id)initWithPath:(CGString)path andTitle:(CPString)title
{
  var bounds=CPRectMake(0,0,650,400);
  self=[super initWithContentRect:bounds styleMask:CPClosableWindowMask|CPResizableWindowMask];

  //[self setFrameOrigin:CPPointMake(350,100)];
  [self center];
  [self orderFront:self];
  [self setTitle:title];
  var contentView=[self contentView];

  var data={
    path:path,
    name:"root",
    isDirectory:true
  };
  rootNode=[Node withData:data];
  [self loadChildren:rootNode];
  CPLog.debug("Children count"+[[rootNode children] count]);


  browser = [[CPBrowser alloc] initWithFrame:CGRectMake(0,0,CGRectGetWidth(bounds),CGRectGetHeight(bounds)-40)];
  //[browser setWidth:300 ofColumn:1];
  [browser setDelegate:self];
  [browser setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
  [browser setTarget:self];
  //[browser setNextResponder:nil];
  [browser setAction:@selector(browserClicked:)];
  [browser setDoubleAction:@selector(dblClicked:)];

  var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth(bounds)-90,CGRectGetHeight(bounds)-40,80.0,24.0)];
  [cancelButton setTitle:@"Cancel"];
  [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [cancelButton setTarget:self];
  [cancelButton setAction:@selector(cancel)];

  var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth(bounds)-190,CGRectGetHeight(bounds)-40,80.0,24.0)];
  [okButton setTitle:@"Ok"];
  [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [okButton setTarget:self];
  [okButton setAction:@selector(close)];
  
  //[aView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
  [contentView addSubview:browser];
  [contentView addSubview:cancelButton];
  [contentView addSubview:okButton];
  //[aView addSubview:cancelButton];
  return self;
}
  -(void)setTarget:(id)target
  {
    _target=target;
  }
  - (void)setAction:(SEL)selector
  {
    _selector=selector
  }

- (id)browser:(id)aBrowser numberOfChildrenOfItem:(id)anItem
{
  CPLog.debug("- (id)browser:(id)aBrowser numberOfChildrenOfItem:(id)anItem:"+anItem);
    if (anItem === nil)
        return [[rootNode children] count];
    [self loadChildren:anItem];
    return [[anItem children] count];
}

- (id)browser:(id)aBrowser child:(int)index ofItem:(id)anItem
{
    if (!anItem)
        return [[rootNode children] objectAtIndex:index];

    return [[anItem children] objectAtIndex:index];
}

- (id)browser:(id)aBrowser imageValueForItem:(id)anItem
{
  if([anItem isDirectory]){
    return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Folder-folder-open-icon.png"] size:CGSizeMake(16, 16)];
  }else{
    if([[anItem name] hasSuffix:@".avi"])
      return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Folder-my-videos-icon.png"] size:CGSizeMake(16, 16)];  
    if([[anItem name] hasSuffix:@".ts"])
      return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Folder-my-videos-icon.png"] size:CGSizeMake(16, 16)];  
    if([[anItem name] hasSuffix:@".mp4"])
      return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Folder-my-videos-icon.png"] size:CGSizeMake(16, 16)];  
    if([[anItem name] hasSuffix:@".mkv"])
      return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Folder-my-videos-icon.png"] size:CGSizeMake(16, 16)];  
    if([[anItem name] hasSuffix:@".vob"])
      return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Folder-my-videos-icon.png"] size:CGSizeMake(16, 16)];  
    if([[anItem name] hasSuffix:@".mpg"])
      return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"Folder-my-videos-icon.png"] size:CGSizeMake(16, 16)];  
    return [[CPImage alloc] initWithContentsOfFile:[[CPBundle mainBundle] pathForResource:"System-help-icon.png"] size:CGSizeMake(16, 16)];  
  }
}

- (id)browser:(id)aBrowser objectValueForItem:(id)anItem
{
  //CPLog.debug("- (id)browser:(id)aBrowser objectValueForItem:(id)anItem"+anItem);
    return [anItem name];
}

- (id)browser:(id)aBrowser isLeafItem:(id)anItem
{
  //[self loadChildren:anItem];
    return ![anItem isDirectory];
}

- (void)browserClicked:(id)aBrowser
{
  if(![[browser selectedItem] isDirectory]){
    console.log("selected column: " + [aBrowser selectedColumn] + " row: " + [aBrowser selectedRowInColumn:[aBrowser selectedColumn]]);
    //[browser addColumn];
  }
}

- (void)dblClicked:(id)sender
{
  [self close];
    //alert("DOUBLE");
}

-(void)close
{
  if(_target&&_selector)
    [_target performSelector:_selector withObject:self];
  [super close];
}
-(void)cancel
{
  [super close];
}
-(void) setAllowsMultipleSelection:(BOOL)a
{
  [browser  setAllowsMultipleSelection:a];
}

-(CPArray)selectedItems
{
  return [[browser selectedItems] allObjects];
}

-(CPArray)selectedItem
{
  return [browser selectedItem];
}

-(void)loadChildren:(id)node
{
  //if([node isDirectory]){
  [node clearChild];
  var path="/api/v1/file"+[node path];
  var response=[CPHTTPURLResponse alloc];
  var error;
  var raw_data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:response];
  //CPLog.debug("raw_data:"+[raw_data rawString]);
  
  var json=[raw_data JSONObject];
  if(json.data){
    var arr=json.data.sort(callbackFunc);
    for(a=0;a<arr.length;a++){
      var newNode=[Node withData:arr[a]];
      //CPLog.debug("new Node"+[newNode name]);
      [node addChild:newNode];
    }
  }
  CPLog.debug("OlfNode:"+[[node children] count]);
  //}
}

@end

function callbackFunc(a,b){
  if(a.name == b.name){
    return 0;
  }

  return (a.name < b.name) ? -1 : 1;
}