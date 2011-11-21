
@import "NodeView.j"

@implementation NodeExecutable: NodeView
{

}
-(id)init
{
  self=[super initWithName:@"Executable" withInputHandle:YES andOutputHandle:YES];
  return self;
}
@end