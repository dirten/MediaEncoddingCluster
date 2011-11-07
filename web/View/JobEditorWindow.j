

@implementation JobEditorWindow:CPWindow


-(id)init{
  self=[super initWithContentRect:CGRectMake(280,50,700,500) styleMask:CPClosableWindowMask|CPResizableWindowMask];
  [self orderFront:self];
  [self setTitle:"Job Editor"];

  return self;
}

@end
 