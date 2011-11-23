


@implementation NodePropertyWindow : CPWindow
{

}
-(id)initWithFrame:(CGRect)aRect
{
  self=[super initWithContentRect:aRect styleMask:CPClosableWindowMask|CPResizableWindowMask];
  [self setFrameOrigin:CPPointMake(350,100)];
  [self orderFront:self];
  return self;
}

@end