
@import "NodeView.j"

@implementation NodeInput: NodeView
{

}
-(id)init
{
  self=[super initWithName:@"Input" withInputHandle:NO andOutputHandle:YES];

  return self;
}

-(id)propertyView
{
  var bounds=CPRectMake(400,300,300,300);
  var view=[[CPView alloc] initWithFrame:bounds];
  [view setBounds:bounds];
  CPLog.debug("orig bounds:"+CPStringFromRect(bounds));
  CPLog.debug("orig View with bounds:"+CPStringFromRect([view bounds]));
  return view;
}
@end