
@import "NodeView.j"

@implementation NodeOutput: NodeView
{
 CPDictionary    data @accessors(property=data);
}
-(id)init
{
  self=[super initWithName:@"Output" withInputHandle:YES andOutputHandle:NO];
  return self;
}
@end