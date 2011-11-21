
@import "NodeView.j"

@implementation NodeOutput: NodeView
{

}
-(id)init
{
  self=[super initWithName:@"Output" withInputHandle:YES andOutputHandle:NO];
  return self;
}
@end