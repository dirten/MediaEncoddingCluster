


@implementation NodePropertyWindow : CPWindow
{

}
-(id)initWithFrame:(CGRect)aRect
{
  self=[super initWithContentRect:aRect styleMask:CPClosableWindowMask|CPResizableWindowMask];
  [self orderFront:self];
  return self;
}

@end