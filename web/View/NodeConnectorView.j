

GraphicHandleWidth = 10.0;
GraphicHandleHalfWidth = GraphicHandleWidth / 2.0;

LineNoHandle = 0;
LineBeginHandle = 1;
LineEndHandle = 2;

@implementation NodeConnectorView: CPBox
{
  CPString name;
  CGPoint     dragLocation;
  CPPoint     beginHandlePoint;
  CPPoint     endHandlePoint;
  int handle;
    BOOL _pointsRight;
    BOOL _pointsDown;

}


-(id)initWithName:(CPString)aName
{
  self=[super initWithFrame:CGRectMake(0.0,0.0,230.0,150.0)];
  if(self){
    name=aName;
    [self setPostsFrameChangedNotifications:YES];
    var label=[CPTextField labelWithTitle:aName];
    [label setFrameOrigin:CGPointMake(10.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];
    [self addSubview:label];
    handleUnderMouse=NO;
		//[self setBeginPoint:CPPointMake(0.0, 10.0)];
		//[self setEndPoint:CPPointMake(50.0, 40.0)];
  }
  return self;
}

-(CPString)name
{
  return name;
}
- (CPPoint)beginPoint {
    // Convert from our odd storage format to something natural.
    var bounds = [self bounds];
    var x = _pointsRight ? CGRectGetMinX(bounds) : CGRectGetMaxX(bounds);
    var y = _pointsDown ? CGRectGetMinY(bounds) : CGRectGetMaxY(bounds);
    return CPPointMake(x, y);
}

- (CPPoint)endPoint {
    // Convert from our odd storage format to something natural.
    var bounds = [self bounds];
    var x = _pointsRight ? CGRectGetMaxX(bounds) : CGRectGetMinX(bounds);
    var y = _pointsDown ? CGRectGetMaxY(bounds) : CGRectGetMinY(bounds);
    return CPPointMake(x, y);
}

- (void)mouseDown:(CPEvent)anEvent
{
  var bounds=[self bounds];
    var beginPoint = CPPointMake(CGRectGetMinX(bounds), CGRectGetMinY(bounds));
   	var endPoint = CPPointMake(CGRectGetMaxX(bounds), CGRectGetMaxY(bounds));
    var curPoint = [self convertPoint:[event locationInWindow] fromView:nil];
    handle=LineNoHandle;
    if([self isHandleAtPoint:beginPoint underPoint:curPoint]){
      CPLog.debug("BeginPoint");
      handle=LineBeginHandle;
    }else if([self isHandleAtPoint:endPoint underPoint:curPoint]){
      handle=LineEndHandle;
      CPLog.debug("EndPoint");
    }
    dragLocation = [anEvent locationInWindow];
    
}
+ (CPArray)boundsWithBeginPoint:(CPPoint)beginPoint endPoint:(CPPoint)endPoint 
{
    // Convert the begin and end points of the line to its bounds and flags specifying the direction in which it points.
    var pointsRight = beginPoint.x < endPoint.x;
    var pointsDown = beginPoint.y < endPoint.y;
    var xPosition = pointsRight ? beginPoint.x : endPoint.x;
    var yPosition = pointsDown ? beginPoint.y : endPoint.y;
    var width = Math.abs(endPoint.x - beginPoint.x);
    var height = Math.abs(endPoint.y - beginPoint.y);

    return [CPArray arrayWithObjects:CPRectMake(xPosition, yPosition, width, height), pointsRight, pointsDown, nil];    
}

- (void)setBeginPoint:(CPPoint)beginPoint 
{
    // It's easiest to compute the results of setting these points together.
	var array = [[self class] boundsWithBeginPoint:beginPoint endPoint:[self endPoint]];
	
    [self setBounds:[array objectAtIndex:0]];
	_pointsRight = [array objectAtIndex:1];
	_pointsDown = [array objectAtIndex:2];
}

- (void)setEndPoint:(NSPoint)endPoint {
    
    // It's easiest to compute the results of setting these points together.
    var array = [[self class] boundsWithBeginPoint:[self beginPoint] endPoint:endPoint];

    [self setBounds:[array objectAtIndex:0]];
	_pointsRight = [array objectAtIndex:1];
	_pointsDown = [array objectAtIndex:2];
}
- (void)mouseDragged:(CPEvent)anEvent
{
    var location = [anEvent locationInWindow],
        origin = [self frame].origin;
    if(!handleUnderMouse){
      //[self setFrameOrigin:CGPointMake(origin.x + location.x - dragLocation.x, origin.y + location.y - dragLocation.y)];
    }
    var handleLocation = [self convertPoint:[event locationInWindow] fromView:nil];

    if(handle==LineBeginHandle){
      
      var bounds = [self bounds];
      bounds.size.width = CGRectGetMaxX(bounds) - bounds.origin.x + location.x - dragLocation.x;
      bounds.origin.x = bounds.origin.x + location.x - dragLocation.x;

      bounds.size.height = CGRectGetMaxY(bounds) - bounds.origin.y + location.y - dragLocation.y;
      bounds.origin.y = bounds.origin.y + location.y - dragLocation.y;
      [self setBounds:bounds];
      [self setNeedsDisplay:YES];
      
      //[self setBeginPoint:handleLocation];
      //[_resizedGraphic resizeByMovingHandle:_resizedHandle toPoint:handleLocation];
    }
    if(handle==LineEndHandle){
      //[self setEndPoint:handleLocation];
    }

    dragLocation = location;
}
- (void)mouseUp:(CPEvent)anEvent
{
      handleUnderMouse=NO;
}
- (void)drawRect:(CGRect)aRect
{
    //[[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    [super drawRect:aRect];
    var context = [[CPGraphicsContext currentContext] graphicsPort];

  var path = CGPathCreateMutable();
  var bounds=[self bounds];
	beginHandlePoint = CPPointMake(CGRectGetMinX(bounds), CGRectGetMinY(bounds));
	endHandlePoint = CPPointMake(CGRectGetMaxX(bounds), CGRectGetMaxY(bounds));
	//var endPoint = [self endPoint];

	CGPathMoveToPoint(path, nil, beginHandlePoint.x, beginHandlePoint.y);
	CGPathAddLineToPoint(path, nil, endHandlePoint.x, endHandlePoint.y);
	CGPathCloseSubpath(path);    

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
    [self drawHandleInView:view atPoint:beginHandlePoint];
    [self drawHandleInView:view atPoint:endHandlePoint];

}

- (void)drawHandleInView:(CPView)view atPoint:(CPPoint)point 
{
	var context = [[CPGraphicsContext currentContext] graphicsPort];
	
    // Figure out a rectangle that's centered on the point but lined up with device pixels.
    var x = point.x - GraphicHandleHalfWidth;
    var y = point.y - GraphicHandleHalfWidth;
    var width = GraphicHandleWidth;
    var height = GraphicHandleWidth;
	  var handleBounds = CGRectMake(x, y, width, height);
    CPLog.debug("Handle @ x:"+x+" y:"+y+" w:"+width+" h:"+height);
    // Draw the shadow of the handle.
    var handleShadowBounds = CGRectOffset(handleBounds, 1.0, 1.0);
	  CGContextSetFillColor(context, [CPColor shadowColor]);
    CGContextFillRect(context, handleShadowBounds);

    // Draw the handle itself.
	CGContextSetFillColor(context, [CPColor darkGrayColor]);
    CGContextFillRect(context, handleBounds);
}

- (BOOL)isHandleAtPoint:(CPPoint)handlePoint underPoint:(CPPoint)point 
{
    // Check a handle-sized rectangle that's centered on the handle point.
    var x = handlePoint.x - GraphicHandleHalfWidth;
    var y = handlePoint.y - GraphicHandleHalfWidth;
    var width = GraphicHandleWidth;
    var height = GraphicHandleWidth;
    var handleBounds = CGRectMake(x, y, width, height);
    CPLog.debug("Handle @ x:"+x+" y:"+y+" w:"+width+" h:"+height);
    CPLog.debug("Point @ x:"+point.x+" y:"+point.y);

    return CGRectContainsPoint(handleBounds, point);
}


-(id)initWithCoder:(CPCoder)aCoder
{
  CPLog.debug("initWithCoder");
  self = [super initWithCoder:aCoder];
  if(self){
    name=[aCoder decodeObjectForKey:"name"];
  }
  return self;
}

-(void)encodeWithCoder:(CPCoder)aCoder
{
  [super encodeWithCoder:aCoder];
  [aCoder encodeObject:name forKey:@"name"];
}

@end