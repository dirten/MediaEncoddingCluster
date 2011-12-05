


@implementation NodePropertyWindow : CPWindow
{

}
-(id)initWithView:(CGRect)aView
{
  self=[super initWithContentRect:[aView bounds] styleMask:CPClosableWindowMask|CPResizableWindowMask];
  [self setFrameOrigin:CPPointMake(350,100)];
  [self orderFront:self];
  [self setTitle:@"Property"];
  var contentView=[self contentView];

  var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([aView bounds])-90,CGRectGetHeight([aView bounds])-40,80.0,24.0)];
  [cancelButton setTitle:@"Ok"];
  [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [cancelButton setTarget:self];
  [cancelButton setAction:@selector(close)];
  
  [aView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
  [contentView addSubview:aView];
  [aView addSubview:cancelButton];


  return self;
}

-(void)close
{
  CPLog.debug("Close");
  [[CPNotificationCenter defaultCenter]
    postNotificationName:RefreshNodeEditorView
    object:self
    userInfo:nil];
    [super close];
}

@end