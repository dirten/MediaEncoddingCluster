

NoHandle = 0;
InputHandle = 1;
OutputHandle = 2;

@implementation NodeView: CPBox
{
  CPString name;
  CPString    taskName            @accessors(property=taskName);
  CPString    labelText           @accessors(property=labelText);
  CPString    message             @accessors(property=message);
  CPString    taskDescription     @accessors(property=taskDescription);
  CPTextField label
  CPTextField fieldDescription    @accessors(property=fieldDescription);
  
  CPPoint     inHandlePoint;
  CPPoint     outHandlePoint;
  CGPoint     dragLocation;
  CPArray     inputElements;
  CPArray     outputElements;
  BOOL        _drawInputHandle;
  BOOL        _drawOutputHandle;
  CPMenu      menu;
  int         uid;//                 @accessors(property=uid);
  int         status              @accessors(property=status);
  float         progress            @accessors(property=progress);
  CPDictionary          data      @accessors(property=data);


  /*performance test*/
  CPImageView _imageView;
  CPTextField _taskLabelField;
  CPTextField _propLabelField;
  CPTextField _messageField;

}

-(id)initWithName:(CPString)aName withInputHandle:(BOOL)drawInputHandle andOutputHandle:(BOOL)drawOutputHandle taskName:(CPString)aTaskName
{
  _drawInputHandle=drawInputHandle;
  _drawOutputHandle=drawOutputHandle;
  CPLog.debug("NodeVIew initWithName:"+aName)
  self=[super initWithFrame:CGRectMake(0.0,0.0,130.0,100.0)];
  if(self){
    name=aName;
    taskName=aTaskName;
    progress=40.0;
    labelText="";
    [self setCornerRadius:5.0];
    //[self setBorderWidth:3.0];
    [self setBorderType:CPGrooveBorder];
    [[self contentView] setBackgroundColor:[CPColor whiteColor]];
    //[self setBorderColor:[CPColor redColor]];
    [self setPostsFrameChangedNotifications:YES];
    
    label=[CPTextField labelWithTitle:aName];
    //[label setFrameOrigin:CGPointMake(10.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];
    [self addSubview:label];
    var bounds=[self bounds];
    fieldDescription=[CPTextField labelWithTitle:@""];
    //[fieldDescription setBounds:CGRectMake(0.0, 0.0, 110.0, 40.0)];
    [fieldDescription setFrameSize:CGSizeMake(120,40)];

    [fieldDescription setLineBreakMode:CPLineBreakByWordWrapping];
    var label_origin=CPPointMake(bounds.origin.x+15,bounds.origin.y-40);
    [fieldDescription setFrameOrigin:label_origin];
    //[fieldDescription setStringValue:@"objectValue test here"];
    [fieldDescription sizeToFit];

    //[fieldDescription setStringValue:@"test value"];
    //CPLog.debug("ContentRect:"+CPStringFromRect(bounds));
    inputElements=[CPArray array];
    outputElements=[CPArray array];
    inHandlePoint=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    outHandlePoint=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
    json={
      "data":{}
    };
    data=[CPDictionary dictionaryWithJSObject:json recursively:YES];    

    _imageView=[[CPImageView alloc] initWithFrame:CGRectMake(0.0, 0.0, 110.0, 110.0)];
    [_imageView setAlphaValue:0.3];
    [_imageView setImage:[self image]];

    _taskLabelField=[CPTextField labelWithTitle:aName];

    var tmptext=[self frontLabel];
    if(tmptext!=undefined&&tmptext.length>18)
      tmptext=tmptext.substring(0,18)+"...";
    _propLabelField=[CPTextField labelWithTitle:tmptext];


  }
  return self;
}
-(int)uid{
  return uid;
}

-(int)setUid:(int)id{
  uid=id;
}

-(CPString)frontLabel
{
  return @"test";
}
-(id)image
{
  return nil;
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

-(id)menuForNodeItem
{
  return [self menuItems:["No Context Menu for this Item"] forActions:[@selector(noop)]];
}
-(id)menuItems:(id)items forActions:(id)actions
{
  //return nil;
  menu = [[CPMenu alloc] initWithTitle:"Null Menu"],
  menuItems = items;//["No Context Menu for this Item"],
  //menuItems = [],
  menuActions = actions;//[@selector(noop)],
  //menuActions = [],
  //isOpen = displayedIssuesKey === "openIssues",
  count = menuItems.length,
  i = 0,
  numberOfSelectedIssues=1;
  
  for (; i < count; i++)
  {
    var title = menuItems[i],
    newMenuItem = [[CPMenuItem alloc] initWithTitle:title action:menuActions[i] keyEquivalent:nil];
    [newMenuItem setEnabled:menuActions[i]!=nil];
    [newMenuItem setTarget:self];    
    [menu addItem:newMenuItem];
  }
  return menu;
}
-(void)noop{}


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
  CPLog.debug("-(void)drawContentsInView:(id)view inRect:(id)aRect:"+CPStringFromRect(aRect)+" progress="+progress);
  //[label drawRect:aRect];
  var context = [[CPGraphicsContext currentContext] graphicsPort];
  var bounds=[self bounds];
  
  //[self setBackgroundColor:[CPColor whiteColor]];
  //[[self contentView] setBackgroundColor:[CPColor whiteColor]];
  
  //CPLog.debug("ContentRect:"+CPStringFromRect(bounds));
  inHandlePoint=CPPointMake(bounds.origin.x-GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
  outHandlePoint=CPPointMake(bounds.origin.x+bounds.size.width+GraphicHandleHalfWidth, bounds.origin.y+(bounds.size.height/2));
  
	var path = CGPathCreateMutable();
	
	CGPathMoveToPoint(path, nil, inHandlePoint.x+5, inHandlePoint.y-20);
  //[path setLineWidth:1];
	CGPathAddLineToPoint(path, nil, outHandlePoint.x-5, outHandlePoint.y-20);
	CGPathCloseSubpath(path);
  CGContextBeginPath(context);
	CGContextAddPath(context, path);
	CGContextClosePath(context);
  CGContextSetStrokeColor(context, [CPColor blackColor]);
	CGContextStrokePath(context);
  
  //var label=[CPTextField labelWithTitle:name];
  var label_origin=CPPointMake(inHandlePoint.x+15,inHandlePoint.y-40);
  [_taskLabelField setFrameOrigin:label_origin];
  [_taskLabelField setTextColor:[CPColor blackColor]];
  //[label setNeedsDisplay:YES];
  [view addSubview:_taskLabelField];
  
  
  var tmptext=[self frontLabel];
  if(tmptext!=undefined&&tmptext.length>18)
    tmptext=tmptext.substring(0,18)+"...";
  [_propLabelField setStringValue:tmptext];
  
  
  var label_origin=CPPointMake(inHandlePoint.x+8,inHandlePoint.y-20);
  [_propLabelField setFrameOrigin:label_origin];
  [_propLabelField setTextColor:[CPColor blackColor]];
  [_propLabelField sizeToFit];

  //[label setNeedsDisplay:YES];
  [view addSubview:_propLabelField];
  
  //var imageView=[[CPImageView alloc] initWithFrame:CGRectMake(0.0, 0.0, 110.0, 110.0)];
  var img_origin=CPPointMake(inHandlePoint.x+15,inHandlePoint.y-55);
  
  [_imageView setFrameOrigin:img_origin];
  [view addSubview:_imageView];
  
  //[view addSubview:fieldDescription];
  //[label drawRect:aRect];
  var base_value=progress;
  if(parseInt(base_value)==base_value&&status==2){
    var ind=[[CPProgressIndicator alloc] initWithFrame:CGRectMake(0,3.5, CGRectGetWidth([self bounds])-4,15)];    
    var progress_origin=CPPointMake(inHandlePoint.x+7,inHandlePoint.y+33);
    [view addSubview:ind];
    [ind startAnimation:nil];
    [ind setDoubleValue:base_value];
    [ind setFrameOrigin:progress_origin];
  }
  if(status!=7){
    var l=parseInt(base_value)==base_value?base_value+"%":base_value;
    var label=[CPTextField labelWithTitle:l];
    var l_origin=CPPointMake(inHandlePoint.x+7,inHandlePoint.y+33);

    [label setFrameOrigin:l_origin];
    [label setTextColor:[CPColor darkGrayColor]];
    [view addSubview:label];
  }else{
    [self setBorderColor:[CPColor redColor]];
    [self setBorderWidth:3.0];
  }
  if(status==2){
    [self setBorderColor:[CPColor blueColor]];
  }
  if(status==4){
    [self setBorderColor:[CPColor greenColor]];
  }
   //fieldDescription=[CPTextField labelWithTitle:labelText];
   //fieldDescription=[CPTextField labelWithTitle:@"test daa"];
   var l_origin=CPPointMake(inHandlePoint.x+7,inHandlePoint.y-7);
   //var l_bounds=CPRectMake(inHandlePoint.x+7,inHandlePoint.y-20, 50,10);
   //[fieldDescription setBounds:l_bounds];
   [fieldDescription setFrameOrigin:l_origin];
   //[fieldDescription setEditable:YES];
   //[fieldDescription setPlaceholderString:@"click to enter label"];
   [fieldDescription setTextColor:[CPColor darkGrayColor]];
   [fieldDescription setStringValue:message];
   [fieldDescription sizeToFit];

   [view addSubview:fieldDescription];
   
  [self drawRect:aRect];
  if(_drawInputHandle)
    [self drawHandleInView:view atPoint:inHandlePoint];
  if(_drawOutputHandle)
    [self drawHandleInView:view atPoint:outHandlePoint];
  //[self drawRect:aRect];
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
    taskName=[aCoder decodeObjectForKey:"taskName"];
    //inHandlePoint=[aCoder decodeObjectForKey:"inHandlePoint"];
    //outHandlePoint=[aCoder decodeObjectForKey:"outHandlePoint"];
    [self initWithName:name withInputHandle:_drawInputHandle andOutputHandle:_drawOutputHandle taskName:taskName];
  }
  return self;
}

-(void)encodeWithCoder:(CPCoder)aCoder
{
  [super encodeWithCoder:aCoder];
  [aCoder encodeObject:name forKey:@"name"];
  [aCoder encodeObject:_drawInputHandle forKey:@"drawInputHandle"];
  [aCoder encodeObject:_drawOutputHandle forKey:@"drawOutputHandle"];
  [aCoder encodeObject:taskName forKey:@"taskName"];
  
  //[aCoder encodeObject:inHandlePoint forKey:@"inHandlePoint"];
  //[aCoder encodeObject:outHandlePoint forKey:@"outHandlePoint"];
}
-(id)propertyView
{
  return nil;
}
-(id)properties
{
  return nil;
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
/*
 function CGBitmapGraphicsContextCreate() 
 { 
 var DOMElement = document.createElement("canvas"), 
 context = DOMElement.getContext("2d"); 
 context.DOMElement = DOMElement; 
 return context; 
 } 
 */