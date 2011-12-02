


@implementation NodePropertyWindow : CPWindow
{

}
-(id)initWithFrame:(CGRect)aRect
{
  self=[super initWithContentRect:aRect styleMask:CPClosableWindowMask|CPResizableWindowMask];
  [self setFrameOrigin:CPPointMake(350,100)];
  var contentView=[self contentView];
  var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-90,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
  [cancelButton setTitle:@"Close"];
  [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [cancelButton setTarget:self];
  [cancelButton setAction:@selector(close)];
  
  //[contentView addSubview:okButton];
  [contentView addSubview:cancelButton];

  [self orderFront:nil];
  return self;
}

@end