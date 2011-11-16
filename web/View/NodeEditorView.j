


@implementation NodeEditorView: CPView
{
  CPArray elements;
  id currentSelectedElement;
  CPPoint currentSelectedHandle;

  CGPoint     dragLocation;
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
  
  //CPLog.debug("Origin"+CPStringFromPoint([element frameOrigin]));
  //[self addSubview:element];
  [elements addObject:element];
  [self setNeedsDisplay:YES];
  
}
   
- (void) mouseDown:		(CPEvent) 	anEvent	 {
  CPLog.debug("mouseDown:"+CPStringFromPoint([anEvent locationInWindow]));
  currentSelectedElement=[self graphicUnderPoint:[anEvent locationInWindow]];
  //currentSelectedHandle=[self handleUnderPoint:[anEvent locationInWindow]];
  if([currentSelectedElement class]!=CPNull){
    CPLog.debug("selected graphic:"+CPStringFromRect([currentSelectedElement bounds]));
  }

  if(currentSelectedHandle!=nil){
    CPLog.debug("currentSelectedHandle:"+CPStringFromPoint([currentSelectedHandle]));
  }
  dragLocation = [anEvent locationInWindow];
}
   
- (void) mouseDragged:		(CPEvent) 	anEvent	 {
  CPLog.debug("mouseDragged:"+CPStringFromPoint([anEvent locationInWindow]));
  if([currentSelectedElement class]!=CPNull){
    var location = [anEvent locationInWindow],
          originBounds = [currentSelectedElement bounds];
    var bounds=CGRectMake(location.x+(originBounds.origin.x-dragLocation.x),location.y+(originBounds.origin.y-dragLocation.y),originBounds.size.width,originBounds.size.height);
    CPLog.debug("NewBounds:"+CPStringFromRect([currentSelectedElement bounds]));
    [currentSelectedElement setBounds:bounds];
    dragLocation = location;
  }else if(currentSelectedHandle){
    CPLog.debug("here:"+JSON.stringify(currentSelectedHandle));
    //CPLog.debug("Drawing Connector from point:"+CPStringFromPoint(currentSelectedHandle)+" to point "+CPStringFromPoint(location));
    //[self drawConnectorInView:self fromPoint:currentSelectedHandle toPoint:[anEvent locationInWindow]];
  }
  [self setNeedsDisplay:YES];
}
- (void) mouseUp:		(CPEvent) 	anEvent	 {
  CPLog.debug("mouseUp:"+[anEvent locationInWindow]);
  currentSelectedElement=[CPNull null];
  currentSelectedHandle=nil;
}

- (void)draggingEntered:(CPDraggingInfo)aSender
{
  CPLog.debug("draggingEntered");
}

- (void)draggingExited:(CPDraggingInfo)aSender
{
  CPLog.debug("draggingExited");
}

-(id)graphicUnderPoint:(CPRect)aPoint{
  var graphicCount = [elements count];
  var selected=[CPNull null];
  for (var index = graphicCount - 1; index>=0; index--) 
	{
    var element = [elements objectAtIndex:index];
    CPLog.debug("ElementIndex:"+index+" Element:"+element);
		if (CPRectContainsPoint([element bounds], aPoint)) 
		{
      CPLog.debug("under point");
      selected=element;
    }
  }
  return selected;
}

-(CPPoint)handleUnderPoint:(CPRect)aPoint{

  var graphicCount = [elements count];
  for (var index = graphicCount - 1; index>=0; index--) 
	{
    var element = [elements objectAtIndex:index];
    //if([element class]!=NodeConnectorView)continue;
    CPLog.debug("ElementIndex:"+index+" Element:"+element);
    var handle=[element handleAtPoint:aPoint];
    if(handle){
      CPLog.debug("Result Point:"+CPStringFromPoint(handle));
      return handle;
    }
  }
  CPLog.debug("return nil");
  return nil;
}

-(void)drawConnectorInView:(CPView)view fromPoint:(CPPoint)startPoint toPoint:(CPPoint)endPoint
{

  var context = [[CPGraphicsContext currentContext] graphicsPort];

  var path = CGPathCreateMutable();
	//var endPoint = [self endPoint];

	CGPathMoveToPoint(path, nil, startPoint.x, startPoint.y);
	CGPathAddLineToPoint(path, nil, endPoint.x, endPoint.y);
	CGPathCloseSubpath(path);    
  CPLog.debug("GraphicsContext:"+context);
    if (path)
	{
		CGContextBeginPath(context);
		CGContextAddPath(context, path);
		CGContextClosePath(context);
    /*
		if ([self isDrawingFill]) 
		{
			CGContextSetFillColor(context, _fillColor);
		    CGContextFillPath(context);
		}*/
		
			CGContextSetStrokeColor(context, [CPColor blackColor]);
		    CGContextStrokePath(context);
  }

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
  CPLog.debug("DrawRect12-End");
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

