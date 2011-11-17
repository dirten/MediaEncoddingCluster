

NoHandle = 0;
InputHandle = 1;
OutputHandle = 2;

@implementation NodeView: CPBox
{
  CPString name;
  CPPoint     inHandlePoint;
  CPPoint     outHandlePoint;
  CGPoint     dragLocation;
  CPArray     inputElements;
  CPArray     outputElements;
}

-(id)init
{
  CPLog.debug("NodeVIew init")
//self=[super init];
  
  if(self){
    //[self setCornerRadius:3.0];
//    [self setBorderWidth:3.0];
    //[self setBorderType:CPGrooveBorder];
    //[self setPostsFrameChangedNotifications:YES];
    //var label=[CPTextField labelWithTitle:aName];
    //[label setFrameOrigin:CGPointMake(10.5,2.5)];
    //[label setTextColor:[CPColor darkGrayColor]];
    //[self addSubview:label];
    var bounds=[self bounds];
    inputElements=[CPArray array];
    outputElements=[CPArray array];
    inHandlePoint=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    outHandlePoint=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
  }
  //return self
}
-(id)initWithName:(CPString)aName
{
  CPLog.debug("NodeVIew initWithName")
  self=[super initWithFrame:CGRectMake(0.0,0.0,130.0,50.0)];
  if(self){
    name=aName;
    [self setCornerRadius:3.0];
    //[self setBorderWidth:3.0];
    [self setBorderType:CPGrooveBorder];
    [self setPostsFrameChangedNotifications:YES];
    var label=[CPTextField labelWithTitle:aName];
    [label setFrameOrigin:CGPointMake(10.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];
    [self addSubview:label];
    var bounds=[self bounds];
    inputElements=[CPArray array];
    outputElements=[CPArray array];
    inHandlePoint=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    outHandlePoint=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
  }
  return self;
}

-(CPString)name
{
  return name;
}

-(CPArray)inputElements
{
  return inputElements;
}

-(CPArray)outputElements
{
  return outputElements;
}

-(CPPoint)outHandlePoint
{
  return outHandlePoint;
}
-(CPPoint)inHandlePoint
{
  return inHandlePoint;
}
-(void)addTarget:(id)target
{
  CPLog.debug("AddTarget")
  [outputElements addObject:target];
}
-(void)addSource:(id)source
{
  CPLog.debug("AddSource")
  [inputElements addObject:source];
}

/*
- (void)mouseDown:(CPEvent)anEvent
{
    
    dragLocation = [anEvent locationInWindow];
    
}
- (void)mouseDragged:(CPEvent)anEvent
{
    var location = [anEvent locationInWindow],
        origin = [self frame].origin;
    
    [self setFrameOrigin:CGPointMake(origin.x + location.x - dragLocation.x, origin.y + location.y - dragLocation.y)];

    dragLocation = location;
}
*/
-(void)drawContentsInView:(id)view inRect:(id)aRect
{
  CPLog.debug("RectSelf:"+CPStringFromRect([self bounds]));
  [super drawRect:aRect];
  var bounds=[self bounds];
  inHandlePoint=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
  outHandlePoint=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));

  CPLog.debug("InHandlePoint:"+CPStringFromPoint([self inHandlePoint]));
  [self drawHandleInView:view atPoint:inHandlePoint];
  [self drawHandleInView:view atPoint:outHandlePoint];
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

- (void)drawRect:(CGRect)aRect
{
    [super drawRect:aRect];
    /*
    var label=[CPTextField labelWithTitle:name];
    [label setFrameOrigin:CGPointMake(14.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];

    [self addSubview:label];
    */
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
    //var bounds=[self bounds];
    //var pointin=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    if([self isHandleAtPoint:inHandlePoint underPoint:aPoint]){
      CPLog.debug("begin point found");
      return inHandlePoint;
    }
    //var pointout=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    if([self isHandleAtPoint:outHandlePoint underPoint:aPoint]){
      CPLog.debug("end point found");
      return outHandlePoint;
    }
    return nil;
}

-(id)initWithCoder:(CPCoder)aCoder
{
  CPLog.debug("initWithCoder");
  self = [super initWithCoder:aCoder];
  if(self){
    name=[aCoder decodeObjectForKey:"name"];
    name=[aCoder decodeObjectForKey:"inHandlePoint"];
    name=[aCoder decodeObjectForKey:"outHandlePoint"];
    [self init];
  }
  return self;
}

-(void)encodeWithCoder:(CPCoder)aCoder
{
  [super encodeWithCoder:aCoder];
  [aCoder encodeObject:name forKey:@"name"];
  [aCoder encodeObject:inHandlePoint forKey:@"inHandlePoint"];
  [aCoder encodeObject:outHandlePoint forKey:@"outHandlePoint"];
}
/*
- (CPView) hitTest:		(CPPoint) 	aPoint	 	
{
  CPLog.debug("hit test");
  return nil;
}

- (BOOL) hitTests
{
  return YES;
}
*/
@end
