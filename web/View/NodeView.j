

NoHandle = 0;
InputHandle = 1;
OutputHandle = 2;

@implementation NodeView: CPBox
{
  CPString name;
  CPTextField label
  CPPoint     inHandlePoint;
  CPPoint     outHandlePoint;
  CGPoint     dragLocation;
  CPArray     inputElements;
  CPArray     outputElements;
  BOOL        _drawInputHandle;
  BOOL        _drawOutputHandle;
  CPMenu      menu;
}

-(id)initWithName:(CPString)aName withInputHandle:(BOOL)drawInputHandle andOutputHandle:(BOOL)drawOutputHandle
{
  _drawInputHandle=drawInputHandle;
  _drawOutputHandle=drawOutputHandle;
  CPLog.debug("NodeVIew initWithName:"+aName)
  self=[super initWithFrame:CGRectMake(0.0,0.0,130.0,100.0)];
  if(self){
    name=aName;
    [self setCornerRadius:5.0];
    //[self setBorderWidth:3.0];
    [self setBorderType:CPGrooveBorder];
    [self setBackgroundColor:[CPColor whiteColor]];
    [self setPostsFrameChangedNotifications:YES];
    
    label=[CPTextField labelWithTitle:aName];
    //[label setFrameOrigin:CGPointMake(10.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];
    [self addSubview:label];
    var bounds=[self bounds];
    //CPLog.debug("ContentRect:"+CPStringFromRect(bounds));
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
  [outputElements addObject:target];
}

-(void)menuForNodeItem
{
    menu = [[CPMenu alloc] initWithTitle:"Encoding Menu"],
    menuItems = ["Stop Encoding", "Delete Encoding"],
    menuActions = [@selector(stop:), @selector(delete:)],
    //isOpen = displayedIssuesKey === "openIssues",
    count = menuItems.length,
    i = 0,
    numberOfSelectedIssues=1;

    for (; i < count; i++)
    {
        var title = menuItems[i],
        newMenuItem = [[CPMenuItem alloc] initWithTitle:title action:menuActions[i] keyEquivalent:nil];

        [newMenuItem setTarget:self];

        switch (title)
        {
          case "Stop Encoding":
            if (numberOfSelectedIssues > 1)
                [newMenuItem setTitle:"Stop (" + numberOfSelectedIssues + ") Encodings" ];
                break;

    	    case "Delete Encoding":
            if (numberOfSelectedIssues > 1)
              [newMenuItem setTitle:"Delete (" + numberOfSelectedIssues + ") Encodings"];
        		break;

          case "Tag":
            var shouldTag = numberOfSelectedIssues === 1;
            break;
          case "Comment":
            [newMenuItem setEnabled:(numberOfSelectedIssues === 1)];
            break;
            // we want a seperator so just skip it for now
          case "View On GitHub":
            [newMenuItem setEnabled:(numberOfSelectedIssues === 1)];
            continue;
            break;
          }
          [menu addItem:newMenuItem];
        }
    return menu;
}

-(void)addSource:(id)source
{
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
  //CPLog.debug("-(void)drawContentsInView:(id)view inRect:(id)aRect:"+CPStringFromRect(aRect));
  //[label drawRect:aRect];
  var context = [[CPGraphicsContext currentContext] graphicsPort];
  var bounds=[self bounds];
  //CPLog.debug("ContentRect:"+CPStringFromRect(bounds));
  inHandlePoint=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
  outHandlePoint=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
   
	var path = CGPathCreateMutable();
	
	CGPathMoveToPoint(path, nil, inHandlePoint.x+5, inHandlePoint.y-20);
  //[path setLineWidth:0];
	CGPathAddLineToPoint(path, nil, outHandlePoint.x-5, outHandlePoint.y-20);
	CGPathCloseSubpath(path);
  CGContextBeginPath(context);
	CGContextAddPath(context, path);
	CGContextClosePath(context);
  CGContextSetStrokeColor(context, [CPColor blackColor]);
	CGContextStrokePath(context);

  var label=[CPTextField labelWithTitle:name];
  var label_origin=CPPointMake(inHandlePoint.x+15,inHandlePoint.y-40);
  [label setFrameOrigin:label_origin];
  [label setTextColor:[CPColor blackColor]];
  [view addSubview:label];
  //[label drawRect:aRect];
  [label setNeedsDisplay:YES];
  var base_value=30.0;
  var ind=[[CPProgressIndicator alloc] initWithFrame:CGRectMake(0,3.5, CGRectGetWidth([self bounds])-4,15)];    
  var progress_origin=CPPointMake(inHandlePoint.x+7,inHandlePoint.y+30);
    [view addSubview:ind];
    [ind startAnimation:nil];
    [ind setDoubleValue:base_value];
    [ind setFrameOrigin:progress_origin];

  [self drawRect:aRect];
  if(_drawInputHandle)
    [self drawHandleInView:view atPoint:inHandlePoint];
  if(_drawOutputHandle)
    [self drawHandleInView:view atPoint:outHandlePoint];
}

- (void)drawHandleInView:(CPView)view atPoint:(CPPoint)point 
{

//  var ctx = document.getElementById('canvas').getContext('2d'); 
	var context = [[CPGraphicsContext currentContext] graphicsPort];
   /*context.shadowOffsetX = 2; 
   context.shadowOffsetY = 2; 
   context.shadowBlur = 2; 
   context.shadowColor = "rgba(0, 0, 0, 0.5)"; 
   context.font = "12px Times New Roman"; 
   context.fillStyle = "Black"; 
   context.fillText(name, inHandlePoint.x+15, inHandlePoint.y-30); 
	*/
    // Figure out a rectangle that's centered on the point but lined up with device pixels.
    var x = point.x - GraphicHandleHalfWidth;
    var y = point.y - GraphicHandleHalfWidth;
    var width = GraphicHandleWidth;
    var height = GraphicHandleWidth;
	  var handleBounds = CGRectMake(x, y, width, height);
    //CPLog.debug("Handle @ x:"+x+" y:"+y+" w:"+width+" h:"+height);
    // Draw the shadow of the handle.
    var handleShadowBounds = CGRectOffset(handleBounds, 1.0, 1.0);
	  CGContextSetFillColor(context, [CPColor shadowColor]);
    CGContextFillRect(context, handleShadowBounds);

    // Draw the handle itself.
	  CGContextSetFillColor(context, [CPColor darkGrayColor]);
    CGContextFillRect(context, handleBounds);
}
/*
- (void)drawRect:(CGRect)aRect
{
    [super drawRect:aRect];
    
    var label=[CPTextField labelWithTitle:name];
    [label setFrameOrigin:CGPointMake(14.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];

    [self addSubview:label];
    
}
*/
- (BOOL)isHandleAtPoint:(CPPoint)handlePoint underPoint:(CPPoint)point 
{
    // Check a handle-sized rectangle that's centered on the handle point.
    var x = handlePoint.x - GraphicHandleHalfWidth;
    var y = handlePoint.y - GraphicHandleHalfWidth;
    var width = GraphicHandleWidth;
    var height = GraphicHandleWidth;
    var handleBounds = CGRectMake(x, y, width, height);
    //CPLog.debug("Handle @ x:"+x+" y:"+y+" w:"+width+" h:"+height);
    //CPLog.debug("Point @ x:"+point.x+" y:"+point.y);

    return CGRectContainsPoint(handleBounds, point);
}

- (CPPoint)handleAtPoint:(CPPoint)aPoint 
{
    // Check a handle-sized rectangle that's centered on the handle point.
    //var bounds=[self bounds];
    //var pointin=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));

    if(_drawInputHandle && [self isHandleAtPoint:inHandlePoint underPoint:aPoint]){
      //CPLog.debug("begin point found");
      return inHandlePoint;
    }
    //var pointout=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    if(_drawOutputHandle && [self isHandleAtPoint:outHandlePoint underPoint:aPoint]){
      //CPLog.debug("end point found");
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
    _drawInputHandle=[aCoder decodeObjectForKey:"drawInputHandle"];
    _drawOutputHandle=[aCoder decodeObjectForKey:"drawOutputHandle"];
    //inHandlePoint=[aCoder decodeObjectForKey:"inHandlePoint"];
    //outHandlePoint=[aCoder decodeObjectForKey:"outHandlePoint"];
    [self initWithName:name withInputHandle:_drawInputHandle andOutputHandle:_drawOutputHandle];
  }
  return self;
}

-(void)encodeWithCoder:(CPCoder)aCoder
{
  [super encodeWithCoder:aCoder];
  [aCoder encodeObject:name forKey:@"name"];
  [aCoder encodeObject:_drawInputHandle forKey:@"drawInputHandle"];
  [aCoder encodeObject:_drawOutputHandle forKey:@"drawOutputHandle"];
  //[aCoder encodeObject:inHandlePoint forKey:@"inHandlePoint"];
  //[aCoder encodeObject:outHandlePoint forKey:@"outHandlePoint"];
}
-(void)openPropertyWindow
{

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
function CGBitmapGraphicsContextCreate() 
{ 
     var DOMElement = document.createElement("canvas"), 
         context = DOMElement.getContext("2d"); 
     context.DOMElement = DOMElement; 
     return context; 
} 
