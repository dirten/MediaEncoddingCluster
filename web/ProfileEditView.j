@implementation ProfileEditView :CPWindow
{

}
-(id)init{
    CPLog.debug("ProfileEditView INIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    //self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask|CPResizableWindowMask];
    self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPClosableWindowMask|CPResizableWindowMask];
    //[self setFloatingPanel:YES];
    [self orderFront:self];
    [self setTitle:"Profile Editor"];
    var contentView=[self contentView];
    var tabView = [[CPTabView alloc] initWithFrame: CGRectMake(10, 0, CGRectGetWidth([contentView bounds]) - 20, 200)];
    //var tabView = [[CPTabView alloc] initWithFrame:[contentView]];
    [tabView setTabViewType:CPTopTabsBezelBorder];
    [tabView setAutoresizingMask: CPViewWidthSizable |CPViewHeightSizable  ];
    [contentView addSubview:tabView];
    
    var tabViewItem1 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem1"];
    [tabViewItem1 setLabel:@"General"];
    var view1 = [[CPView alloc] initWithFrame: CGRectMake(0, 0, 100, 100)];
    [tabViewItem1 setView:view1];
    [tabView addTabViewItem:tabViewItem1];
    
    var tabViewItem2 = [[CPTabViewItem alloc] initWithIdentifier:@"tabViewItem2"];
    [tabViewItem2 setLabel:@"Format"];
    var view2 = [[CPView alloc] initWithFrame: CGRectMake(0, 0, 100, 100)];
    [tabViewItem2 setView:view2];
    [tabView addTabViewItem:tabViewItem2];
    return self;
}
@end
