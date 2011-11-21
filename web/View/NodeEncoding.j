
@import "NodeView.j"


@implementation NodeEncoding: NodeView
{

}
-(id)init
{
  self=[super initWithName:@"Encoding" withInputHandle:YES andOutputHandle:YES];

  return self;
}

-(void)drawContentsInView:(id)view inRect:(id)aRect
{
  [super drawContentsInView:view inRect:aRect];
}
@end