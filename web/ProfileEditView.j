@import "View/ProfileEdit/GeneralView.j"
@import "View/ProfileEdit/FormatView.j"
@implementation ProfileEditView :CPWindow
{

}
-(id)setProfileId:(id) id{
    CPLog.debug("setting profile id"+ id.test);
    id.test="bla return";

}

-(id)init{
    CPLog.debug("ProfileEditView INIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    //self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask|CPResizableWindowMask];
    self=[super initWithContentRect:CGRectMake(15,150,625,425) styleMask:CPClosableWindowMask|CPResizableWindowMask];
    //[self setFloatingPanel:YES];
    [self orderFront:self];
    [self setTitle:"Profile Editor"];
    var contentView=[self contentView];
    var tabView = [[CPTabView alloc] initWithFrame: CGRectMake(10, 10, CGRectGetWidth([contentView bounds]) - 20, CGRectGetHeight([contentView bounds])-60)];
    //var tabView = [[CPTabView alloc] initWithFrame:[contentView]];
    //[tabView setTabViewType:CPTopTabsBezelBorder];
    [tabView setAutoresizingMask: CPViewWidthSizable |CPViewHeightSizable  ];
    [contentView addSubview:tabView];
    
    var tabViewItem1 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem1"];
    [tabViewItem1 setLabel:@"General"];
    var view1 = [[GeneralView alloc] initWithFrame:[tabView bounds]];
    [view1 init];
    [tabViewItem1 setView:view1];
    [tabView addTabViewItem:tabViewItem1];
    
    var tabViewItem2 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem2"];
    [tabViewItem2 setLabel:@"Format"];
    var view2 = [[FormatView alloc] initWithFrame: CGRectMake(0, 0, 100, 100)] ;
    [tabViewItem2 setView:view2];
    [tabView addTabViewItem:tabViewItem2];

    var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-90,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
    [okButton setTitle:@"Save"];
    [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];

    var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-180,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
    [cancelButton setTitle:@"Cancel"];
    [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
    
    [contentView addSubview:okButton];
    [contentView addSubview:cancelButton];
    return self;
}
@end
