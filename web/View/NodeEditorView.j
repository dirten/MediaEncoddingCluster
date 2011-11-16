


@implementation NodeEditorView: CPView
{
  CPArray elements;
}

-(id)initWithFrame:(id)aFrame
{
  self=[super initWithFrame:aFrame];
  if(self){
    [self registerForDraggedTypes:[NodeElementDragType]];
    elements=[CPArray array];
  }
  return self;
}

- (void)performDragOperation:(CPDraggingInfo)aSender
{
  CPLog.debug("drop_dragging:");
  var data = [[aSender draggingPasteboard] dataForType:NodeElementDragType];
  CPLog.debug("drop_dragging:"+data);
  var element=[CPKeyedUnarchiver unarchiveObjectWithData:data];
  CPLog.debug("Element:"+[element name]);
  CPLog.debug(CPStringFromPoint([aSender draggingLocation]));
  var location=[self convertPoint:[aSender draggingLocation] fromView:nil];
  //[element setFrameOrigin:[aSender draggingLocation]];
  var frameOrigin=[aSender draggingLocation];
  var bounds = [element bounds];
  [element setBounds:CGRectMake(frameOrigin.x,frameOrigin.y,bounds.size.width,bounds.size.height)];
  
  CPLog.debug("Origin"+CPStringFromPoint([element frameOrigin]));
  //[self addSubview:element];
  [elements addObject:element];
  [self setNeedsDisplay:YES];
  
}   
- (void) mouseDragged:		(CPEvent) 	anEvent	 {
  CPLog.debug("mouseDragged");
}

- (void)draggingEntered:(CPDraggingInfo)aSender
{
  CPLog.debug("draggingEntered");
}

- (void)draggingExited:(CPDraggingInfo)aSender
{
  CPLog.debug("draggingExited");
}
- (void)drawRect:(CPRect)rect 
{
  var context = [[CPGraphicsContext currentContext] graphicsPort];
  
  CPLog.debug("DrawRect12");
  var graphicCount = [elements count];
  for (var index = graphicCount - 1; index>=0; index--) 
	{
    var element = [elements objectAtIndex:index];
    CPLog.debug("ElementIndex:"+index+" Element:"+element);
    CGContextSaveGState(context);
		[element drawContentsInView:self inRect:rect];
    
		CGContextRestoreGState(context);
  }
}
/*
 - (CPView) hitTest:		(CPPoint) 	aPoint	 	
 {
 CPLog.debug("hit test x:"+aPoint.x+" y:"+aPoint.y);
 return nil;
 }
 
 - (BOOL) hitTests
 {
 return NO;
 }
 */
@end

