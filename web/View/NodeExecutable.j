
@import "NodeView.j"

@implementation NodeExecutable: NodeView
{
 CPDictionary    data @accessors(property=data);
}
-(id)init
{
  self=[super initWithName:@"Executable" withInputHandle:YES andOutputHandle:YES];
  return self;
}
@end