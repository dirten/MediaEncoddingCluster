@import "ProfileEdit/GeneralView.j"
@import "ProfileEdit/FormatView.j"
@import "ProfileEdit/VideoView.j"
@import "ProfileEdit/AudioView.j"
@import "ProfileEdit/AudioView.j"

ProfileChanged=@"ProfileChanged";

@implementation ProfileEditView : CPView
  {
    //CPTabView tabView;
    var items;
    CPDictionary    views;
    CPDictionary    pdata @accessors(property=data);
    CPView          currentView;


  }
  -(id)setProfileId:(id) id{
    return;
    CPLog.debug("setting profile id:"+ id);
    if(id!="double click to create a new Profile"&&id!=0){
      var request = [CPURLRequest requestWithURL:"/api/v1/profile?id="+id];
      [request setHTTPMethod:"GET"];
      // see important note about CPJSONPConnection above
      var connection = [CPURLConnection connectionWithRequest:request delegate:self];
    }else{
    profileData={};
    profileData.data={};
    profileData.data.format={};
    profileData.data.video={};
    profileData.data.audio={};
    
    [generalView setData:profileData.data];
    [formatView setData:profileData.data.format];
    [videoView setData:profileData.data.video];
    [audioView setData:profileData.data.audio];
  }

}
-(id)initWithFrame:(GCRect)aFrame{
  var self=[super initWithFrame:aFrame];
  CPLog.debug("ProfileEditView INIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  splitview = [[CPSplitView alloc] initWithFrame:aFrame];
  [splitview setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable ];

}

-(id)initWithData:(id)data{
  pdata=data
  CPLog.debug("ProfileEditView INIT12 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  //self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask|CPResizableWindowMask];
  views = [CPDictionary dictionary];  
  //var self=[super init];
  //var self=[super initWithContentRect:CGRectMake(280,50,700,500) styleMask:CPClosableWindowMask|CPResizableWindowMask];
  var self=[super initWithFrame:CGRectMake(0,0,700,500)];
  var contentView=self;//[self contentView];
  items = [CPDictionary dictionaryWithObjects:[ [], [], [], []] forKeys:[@"Audio",@"Video",@"Format",@"General"]];
  CPLog.debug("here"+CPStringFromRect([contentView bounds]));

  splitview = [[CPSplitView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth([contentView bounds]), CGRectGetHeight([contentView bounds]))];
  [splitview setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable ];
  //[splitview setIsPaneSplitter:YES];

  listScrollView = [[CPScrollView alloc] initWithFrame:CGRectMake(0,0,200,CGRectGetHeight([contentView bounds]))];
  [listScrollView setAutohidesScrollers:YES];
  [splitview addSubview:listScrollView];
  mainView = [[CPView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth([contentView bounds]), CGRectGetHeight([contentView bounds]))];
  [splitview addSubview:mainView];

  treeView=[[CPOutlineView alloc] initWithFrame:[[listScrollView contentView] bounds]];
    var textColumn = [[CPTableColumn alloc] initWithIdentifier:@"TextColumn2"];
    [textColumn setWidth:140.0];

    [textColumn setEditable:YES];

    [treeView addTableColumn:textColumn];
    [treeView setOutlineTableColumn:textColumn];
    [treeView setBackgroundColor:[CPColor colorWithRed:221.0/255.0 green:228.0/255.0 blue:235.0/255.0 alpha:1.0]];
    //[self setBackgroundColor:[CPColor colorWithRed:46.0/255.0 green:46.0/255.0 blue:50.0/255.0 alpha:1.0]];
    [treeView setAllowsEmptySelection:NO];
    [treeView setDataSource:self];
    [treeView setDelegate:self];
    [treeView selectRowIndexes:[CPIndexSet indexSetWithIndex:0] byExtendingSelection:NO];




  [listScrollView setDocumentView:treeView];

  [contentView addSubview:splitview];



  generalView = [[GeneralView alloc] initWithFrame:[mainView bounds]];
  [generalView init];
  [self addView:generalView];

  formatView = [[FormatView alloc] initWithFrame:[mainView bounds]] ;
  [formatView init];
  [self addView:formatView];
  
  videoView = [[VideoView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth([mainView bounds]), CGRectGetHeight([mainView bounds])-50)] ;
  [videoView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable ];

  //var listScrollView = [[CPScrollView alloc] initWithFrame:[mainView bounds]];
  //[listScrollView setDocumentView:videoView];
  //[listScrollView setHasHorizontalScroller:NO];
  [self addView:videoView];


  audioView = [[AudioView alloc] initWithFrame:[mainView bounds]] ;
  [audioView init];
  [self addView:audioView];
  
  [mainView addSubview:formatView];
  [mainView addSubview:generalView];
  [mainView addSubview:videoView];
  [mainView addSubview:audioView];

  var data=[pdata objectForKey:@"data"];
  CPLog.debug("Data in ProfileEdit View:"+data);
  var format=[data objectForKey:@"format"];
  var video=[data objectForKey:@"video"];
  var audio=[data objectForKey:@"audio"];

  [generalView setData:data];
  [formatView setData:format];
  [videoView setData:video];
  [audioView setData:audio];
  
  var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-90,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
  [okButton setTitle:@"Save"];
  [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [okButton setTarget:self];
  [okButton setAction:@selector(save:)];
  [contentView addSubview:okButton];

  /*
  var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-180,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
  [cancelButton setTitle:@"Cancel"];
  [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  //[cancelButton setTarget:self];
  //[cancelButton setAction:@selector(close)];

  [contentView addSubview:cancelButton];
  */
  [self setActiveView:@"General"];
  //[self orderFront:self];
  //[self setTitle:"Profile Editor"];

  return self;
}
- (void)save:(id)sender{
  CPLog.debug("SAVING PROFILE");

  CPLog.debug("Data:"+JSON.stringify([pdata toJSON]));
  return;
  //CPLog.debug(JSON.stringify(profileData));
  var url="/api/v1/profile";
  if(profileData.data.id)
    url+="?id="+profileData.data.id;

  var request = [CPURLRequest requestWithURL:url];
  [request setHTTPMethod:"POST"];
  [request setHTTPBody:JSON.stringify(profileData.data)];
  // see important note about CPJSONPConnection above
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("Profile Save Result"+[result rawString]);
  [[CPNotificationCenter defaultCenter]
    postNotificationName:ProfileViewRefresh
    object:self
    userInfo:nil];

  CPLog.debug("tryResult Parsing");
  resultData=[[result rawString] objectFromJSON];
  CPLog.debug("Result Parsed");
  if(resultData.id){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:ProfileChanged
      object:self
      userInfo:resultData.id];
   //[self close];
    var alert=[CPAlert alertWithMessageText:@"Profile saved" defaultButton:@"Ok" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Profile successfull Saved"];
    [alert setAlertStyle:CPInformationalAlertStyle];
    [alert runModal];
  }else{
    var alert=[CPAlert alertWithError:resultData.error.description];
    [alert runModal];
  }


}
/*
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  //CPLog.debug("received json data");
  profileData=[data objectFromJSON];

  [generalView setData:profileData.data];
  [formatView setData:profileData.data.format];
  [videoView setData:profileData.data.video];
  [audioView setData:profileData.data.audio];

}
- (void)connection:(CPURLConnection)aConnection didReceiveResponse:(CPHTTPURLResponse)response
{
  //CPLog.debug(response);
}

- (void)connection:(CPURLConnection)aConnection didFailWithError:(CPString)error
{
  alert(error); //a network error occurred
}
*/




/*!!!!!!!!!!!!!!!!!!!! begin CPOutlineView Delegate !!!!!!!!!!!!!!!!!!!!!!!!!!!!*/


- (id)outlineView:(CPOutlineView)outlineView child:(int)index ofItem:(id)item
{
    //CPLog.debug("outlineView:%@ child:%@ ofItem:%@", outlineView, index, item);

    if (item === nil)
    {
        var keys = [items allKeys];
        //console.log([keys objectAtIndex:index]);
        return [keys objectAtIndex:index];
    }
    else
    {
        var values = [items objectForKey:item];
        //console.log([values objectAtIndex:index]);
        return [values objectAtIndex:index];
    }
}

- (BOOL)outlineView:(CPOutlineView)outlineView isItemExpandable:(id)item
{
    //CPLog.debug("outlineView:%@ isItemExpandable:%@", outlineView, item);

    var values = [items objectForKey:item];
    //console.log(([values count] > 0));
    return ([values count] > 0);
}

- (int)outlineView:(CPOutlineView)outlineView numberOfChildrenOfItem:(id)item
{
    //CPLog.debug("outlineView:%@ numberOfChildrenOfItem:%@", outlineView, item);

    if (item === nil)
    {
        //console.log([items count]);
        return [items count];
    }
    else
    {
        var values = [items objectForKey:item];
        //console.log([values count]);
        return [values count];
    }
}

- (id)outlineView:(CPOutlineView)outlineView objectValueForTableColumn:(CPTableColumn)tableColumn byItem:(id)item
{
    //CPLog.debug("outlineView:%@ objectValueForTableColumn:%@ byItem:%@", outlineView, tableColumn, item);

    //console.log(item);

    return item;
}
- (void)tableViewSelectionDidChange:(CPNotification)aNotification
{
    //CPLog.debug("selection");
}
- (BOOL)selectionShouldChangeInTableView:(CPOutlineView)outlineView
{
    //CPLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    return YES;
}
-(BOOL)outlineView:(CPOutlineView)aTableView shouldSelectItem:(int)rowIndex
{
    CPLog("bla"+rowIndex);
    CPLog.debug("CPOutline view notification")
    //var outlineView = [notification object];
    //var selectedRow = [[outlineView selectedRowIndexes] firstIndex];
    //var item = [outlineView itemAtRow:selectedRow];
    //item=[item substringToIndex:[item length]-1];
    [self setActiveView:rowIndex];
    return YES;
}
- (BOOL)outlineView:(CPOutlineView)aTableView shouldSelectTableColumn:(CPTableColumn)aTableColumn
{
    //CPLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    return YES;
}
- (void)outlineView:(CPOutlineView)outlineView didClickTableColumn:(CPTableColumn)tableColumn
{
    //CPLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}
/*!!!!!!!!!!!!!!!!!!!! end CPOutlineView Delegate !!!!!!!!!!!!!!!!!!!!!!!!!!!!*/




/*!!!!!!!!!!!!!!!!!!!! begin view handling !!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  - (void)addView:(id)view
  {
    [view setHidden:YES];
    //CPLog.debug([view className]+" set hidden");
    [views setObject:view forKey:[view className]];
  }  
- (void)setActiveView:(id)name
  {
    CPLog("setActiveView with name : "+name);
    var view = [views objectForKey:name  + "View"];
    
    [currentView setHidden:YES];
    if (view){
        [view setHidden:NO];
        currentView = view;
      }else{
      CPLog.debug("view with name not found : "+name);
}
  }

/*!!!!!!!!!!!!!!!!!!!! end view handling !!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

@end
