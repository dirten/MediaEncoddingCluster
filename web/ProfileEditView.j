@import "View/ProfileEdit/GeneralView.j"
@import "View/ProfileEdit/FormatView.j"
@implementation ProfileEditView :CPWindow
{

}
-(id)setProfileId:(id) id{
    CPLog.debug("setting profile id"+ id);
    var request = [CPURLRequest requestWithURL:"http://localhost:8080/api/v1/profile?id="+id];
    [request setHTTPMethod:"GET"];
    // see important note about CPJSONPConnection above
    var connection = [CPURLConnection connectionWithRequest:request delegate:self];


}

-(id)init{
    CPLog.debug("ProfileEditView INIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    //self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask|CPResizableWindowMask];
    self=[super initWithContentRect:CGRectMake(15,150,625,425) styleMask:CPClosableWindowMask|CPResizableWindowMask];
    //[self setFloatingPanel:YES];
    [self orderFront:self];
    [self setTitle:"Profile Editor"];
    var contentView=[self contentView];
    var tabView = [[CPTabView alloc] initWithFrame: CGRectMake(10, 50, CGRectGetWidth([contentView bounds]) - 20, CGRectGetHeight([contentView bounds])-60)];
    //var tabView = [[CPTabView alloc] initWithFrame:[contentView]];
    //[tabView setTabViewType:CPTopTabsBezelBorder];
    [tabView setAutoresizingMask: CPViewWidthSizable |CPViewHeightSizable  ];
    [contentView addSubview:tabView];
    
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
    [tabViewItem2 setView:formatView];
    [tabView addTabViewItem:tabViewItem2];

    var tabViewItem3 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem3"];
    [tabViewItem3 setLabel:@"Video"];
    var view3 = [[FormatView alloc] initWithFrame:[tabView bounds]] ;
    [view3 init];
    [tabViewItem3 setView:view3];
    [tabView addTabViewItem:tabViewItem3];

    var tabViewItem4 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem4"];
    [tabViewItem4 setLabel:@"Audio"];
    var view4 = [[FormatView alloc] initWithFrame:[tabView bounds]] ;
    [view4 init];
    [tabViewItem4 setView:view4];
    [tabView addTabViewItem:tabViewItem4];

    var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-90,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
    [okButton setTitle:@"Save"];
    [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];

    var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-180,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
    [cancelButton setTitle:@"Cancel"];
    [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
    
    [contentView addSubview:okButton];
    [contentView addSubview:cancelButton];
    [okButton setTarget:self];
    [okButton setAction:@selector(save:)];
    return self;
}
- (void)save:(id)sender{
    CPLog.debug(profileData.testdata);
}
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
    profileData=[data objectFromJSON];
    [formatView setData:profileData];
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
