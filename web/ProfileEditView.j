@import "View/ProfileEdit/GeneralView.j"
@import "View/ProfileEdit/FormatView.j"
@import "View/ProfileEdit/VideoView.j"
@import "View/ProfileEdit/AudioView.j"
ProfileChanged=@"ProfileChanged";

@implementation ProfileEditViewOld :CPWindow
  {
    CPTabView tabView;
  }
  -(id)setProfileId:(id) id{
    
    CPLog.debug("setting profile id"+ id);
    if(id!="double click to create a new Profile"){
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

-(id)init{
  CPLog.debug("ProfileEditView INIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  //self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask|CPResizableWindowMask];
  self=[super initWithContentRect:CGRectMake(280,50,625,600) styleMask:CPClosableWindowMask|CPResizableWindowMask];

  [self orderFront:self];
  [self setTitle:"Profile Editor"];
  var contentView=[self contentView];
  
  tabView = [[CPTabView alloc] initWithFrame: CGRectMake(10, 10, CGRectGetWidth([contentView bounds]) - 20, CGRectGetHeight([contentView bounds])-100)];
  //var tabView = [[CPTabView alloc] initWithFrame:[contentView]];
  //[tabView setTabViewType:CPTopTabsBezelBorder];
  [tabView setAutoresizingMask: CPViewWidthSizable |CPViewHeightSizable  ];
  [contentView addSubview:tabView];
  [tabView setBackgroundColor:[CPColor whiteColor]];

  var tabViewItem1 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem1"];
  [tabViewItem1 setLabel:@"General"];
  generalView = [[GeneralView alloc] initWithFrame:[tabView bounds]];
  [generalView init];
  [tabViewItem1 setView:generalView];
  [tabView addTabViewItem:tabViewItem1];

  var tabViewItem2 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem2"];
  [tabViewItem2 setLabel:@"Format"];
  formatView = [[FormatView alloc] initWithFrame:[tabView bounds]] ;
  [formatView init];
  //var listScrollView = [[CPScrollView alloc] initWithFrame:[tabView bounds]];
  //[listScrollView setDocumentView:formatView];
  //[listScrollView setHasHorizontalScroller:NO];
  [tabViewItem2 setView:formatView ];
  [tabView addTabViewItem:tabViewItem2];

  var tabViewItem3 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem3"];
  [tabViewItem3 setLabel:@"Video"];
  //videoView = [[VideoView alloc] initWithFrame:[contentView bounds]] ;
  videoView = [[VideoView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth([tabView bounds]) , CGRectGetHeight([tabView bounds])+1000)] ;
  [videoView init];
  var listScrollView = [[CPScrollView alloc] initWithFrame:[tabView bounds]];
  [listScrollView setDocumentView:videoView];
  [listScrollView setHasHorizontalScroller:NO];
  [tabViewItem3 setView:listScrollView];
  //[tabViewItem3 setView:videoView];
  [tabView addTabViewItem:tabViewItem3];

  var tabViewItem4 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem4"];
  [tabViewItem4 setLabel:@"Audio"];
  audioView = [[AudioView alloc] initWithFrame:[tabView bounds]] ;
  [audioView init];
  [tabViewItem4 setView:audioView];
  [tabView addTabViewItem:tabViewItem4];

  var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-90,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
  [okButton setTitle:@"Save"];
  [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];

  var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-180,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
  [cancelButton setTitle:@"Cancel"];
  [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [cancelButton setTarget:self];
  [cancelButton setAction:@selector(close)];
  [contentView addSubview:okButton];
  [contentView addSubview:cancelButton];
  [okButton setTarget:self];
  [okButton setAction:@selector(save:)];
  return self;
}
- (void)save:(id)sender{
  CPLog.debug(JSON.stringify(profileData));
  var url="/api/v1/profile";
  if(profileData.data.id)
    url+="?id="+profileData.data.id;

  var request = [CPURLRequest requestWithURL:url];
  [request setHTTPMethod:"POST"];
  [request setHTTPBody:JSON.stringify(profileData.data)];
  // see important note about CPJSONPConnection above
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug([result rawString]);
  [[CPNotificationCenter defaultCenter]
    postNotificationName:ProfileViewRefresh
    object:self
    userInfo:nil];
  resultData=[[result rawString] objectFromJSON];
  if(resultData.id){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:ProfileChanged
      object:self
      userInfo:resultData.id];
  }
  [self close];

}
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  CPLog.debug("received json data");
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

@end
