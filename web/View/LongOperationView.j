@implementation LongOperationView : CPWindow
{

}

-(id)initWithFrame:(CPRect)aFrame
{
  self=[super initWithContentRect:CGRectMake(0,0,200,155) styleMask:CPClosableWindowMask|CPResizableWindowMask];
 [self orderFront:nil];

  return self;
}
@end