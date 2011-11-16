

GraphicHandleWidth = 10.0;
GraphicHandleHalfWidth = GraphicHandleWidth / 2.0;

LineNoHandle = 0;
LineBeginHandle = 1;
LineEndHandle = 2;

@implementation NodeConnectorView: CPObject
{
  CPString name;
  CGPoint     dragLocation;
  CPPoint     beginHandlePoint;
  CPPoint     endHandlePoint;
  CPPoint     _beginPoint;
  CPPoint     _endPoint;
  int handle;
    BOOL _pointsRight;
    BOOL _pointsDown;

}

- (id)init
{
    self = [super init];
    return self;
}
-(id)initWithName:(CPString)aName
{
  self=[super init];
  if(self){
    name=aName;
    handleUnderMouse=NO;
		[self setBeginPoint:CPPointMake(0.0, 10.0)];
		[self setEndPoint:CPPointMake(50.0, 40.0)];
  }
  return self;
}

-(CPString)name
{
  return name;
}
- (CPPoint)beginPoint {
    return _beginPoint;
}

- (CPPoint)endPoint {
    return _endPoint;
}

- (void)setBeginPoint:(CPPoint)beginPoint 
{
  _beginPoint=beginPoint;
}

- (void)setEndPoint:(NSPoint)endPoint 
{
  _endPoint=endPoint;
}

-(void)setBounds:(CPRect)aRect
{
  _beginPoint = CPPointMake(CGRectGetMinX(aRect), CGRectGetMinY(aRect));
  _endPoint = CPPointMake(CGRectGetMaxX(aRect), CGRectGetMaxY(aRect));
}

-(CPRect)bounds
{
  return CGRectMake(_beginPoint.x,_beginPoint.y,_endPoint.x-_beginPoint.x,_endPoint.y-_beginPoint.y);
}
/*
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
*/
-(void)drawContentsInView:(id)view inRect:(id)aRect
{
  [self drawRect:aRect];
}

- (void)drawRect:(CGRect)aRect
{
  var context = [[CPGraphicsContext currentContext] graphicsPort];
  var path = CGPathCreateMutable();

	CGPathMoveToPoint(path, nil, _beginPoint.x, _beginPoint.y);
	CGPathAddLineToPoint(path, nil, _endPoint.x, _endPoint.y);
	CGPathCloseSubpath(path);    

  if (path)
	{
		CGContextBeginPath(context);
		CGContextAddPath(context, path);
		CGContextClosePath(context);
  	CGContextSetStrokeColor(context, [CPColor blackColor]);
	  CGContextStrokePath(context);
  }
  [self drawHandleInView:view atPoint:_beginPoint];
  [self drawHandleInView:view atPoint:_endPoint];
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

- (CPPoint)handleAtPoint:(CPPoint)aPoint 
{
    // Check a handle-sized rectangle that's centered on the handle point.
    var bounds=[self bounds];
    var pointin=CPPointMake(bounds.origin.x, bounds.origin.y);
    if([self isHandleAtPoint:_beginPoint underPoint:aPoint]){
      CPLog.debug("begin point found");
      return LineBeginHandle;
    }
    //var pointout=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    if([self isHandleAtPoint:_endPoint underPoint:aPoint]){
      CPLog.debug("end point found");
      return LineEndHandle;
    }
    return LineNoHandle;
}

-(id)initWithCoder:(CPCoder)aCoder
{
  name=[aCoder decodeObjectForKey:"name"];
  _beginPoint=[aCoder decodeObjectForKey:"beginPoint"];
  _endPoint=[aCoder decodeObjectForKey:"endPoint"];
  return self;
}

-(void)encodeWithCoder:(CPCoder)aCoder
{
  [aCoder encodeObject:name forKey:@"name"];
  [aCoder encodeObject:_beginPoint forKey:@"beginPoint"];
  [aCoder encodeObject:_endPoint forKey:@"endPoint"];
}

@end