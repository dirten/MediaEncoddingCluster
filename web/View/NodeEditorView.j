
@import "NodePropertyWindow.j"

RefreshNodeEditorView=@"RefreshNodeEditorView";
NodeEditorViewChanged=@"NodeEditorViewChanged";

@implementation NodeEditorView: CPView
{
  CPArray elements @accessors(property=elements);
  id currentSelectedElement;
  id targetDropElement;
  id select;
  id selectedPathSource;
  id selectedPathTarget;
  CPPoint currentSelectedHandle;
  CPString _name  @accessors(property=name);
  CGPoint     dragLocation;
  id lastNode;
  BOOL isDrawingLink;
  BOOL movingView;
  BOOL unsavedChanges       @accessors(property=unsavedChanges);
  //int   uidCounter;
 
}

-(id)initWithFrame:(id)aFrame
{
  self=[super initWithFrame:aFrame];
  if(self){
    [self registerForDraggedTypes:[NodeElementDragType]];
    elements=[CPArray array];
    lastNode=nil;
    selectedPathSource=nil;
    selectedPathTarget=nil;
    uidCounter=0;

    movingView=NO;
    [self setBackgroundColor:[CPColor whiteColor]];
  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(refreshNodeEditorView)
    name:RefreshNodeEditorView
    object:nil];
  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(nodeEditorViewChanged)
    name:NodeEditorViewChanged
    object:nil];

    /*
    var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([self bounds])-90,CGRectGetHeight([self bounds])-40,80.0,24.0)];
    [okButton setTitle:@"Save"];
    [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
    [okButton setTarget:self];
    [okButton setAction:@selector(save:)];
    [self addSubview:okButton];
    */
    dragLocation=CPPointMake(0,0);
  }
  return self;
}
-(void)nodeEditorViewChanged
{
  unsavedChanges=YES;
  [self setNeedsDisplay:YES];
}
-(BOOL)hasUnsavedChanges
{
  return unsavedChanges;
}

-(void)refreshNodeEditorView
{
  CPLog.debug("Refresh Signal received");
 [self setNeedsDisplay:YES];
}
-(void)clearElements
{
  [[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
  elements=[CPArray array];
  unsavedChanges=NO;
  [self setNeedsDisplay:YES];
}
- (void)performDragOperation:(CPDraggingInfo)aSender
{
  var data = [[aSender draggingPasteboard] dataForType:NodeElementDragType];
  var element=[CPKeyedUnarchiver unarchiveObjectWithData:data];
  
  var frameOrigin=[self convertPoint:[aSender draggingLocation] fromView:nil];  
  //var frameOrigin = [self convertPoint:[_window convertGlobalToBase:aGlobalLocation] fromView:nil]; 
  var bounds = [element bounds];

  [element setBounds:CGRectMake(frameOrigin.x-(bounds.size.width/2),frameOrigin.y-(bounds.size.height/2),bounds.size.width,bounds.size.height)];
  [element setUid:[elements count]+1];
  [element setProgress:@" "];

  [elements addObject:element];
  //CPLog.debug("UidCounter:"+[element uid]);
  /*
  if(lastNode){
    [lastNode addTarget:element];
  }else
    lastNode=element;
    */
  [self nodeEditorViewChanged];
  [self setNeedsDisplay:YES];
  
}

- (CPMenu)menuForEvent:(CPEvent)anEvent
{
  CPLog.debug("menu Button:"+[anEvent buttonNumber]);
  var point=[self convertPoint:[anEvent locationInWindow] fromView:nil];
  currentSelectedElement=[self graphicUnderPoint:point];
  if([currentSelectedElement class]!=CPNull){
    CPLog.debug("selected graphic:"+CPStringFromRect([currentSelectedElement bounds]));
    var menu=[currentSelectedElement menuForNodeItem];
    CPLog.debug("Menu:"+menu);
    select=currentSelectedElement;
    [select setBorderWidth:3.0];
    [select setBorderColor:[CPColor greenColor]];
    currentSelectedElement=[CPNull null];
    select=[CPNull null];
    movingView=NO;  
    return menu;
   }
}
/*
- (void)rightMouseDown:(CPEvent)anEvent	
{
    CPLog.debug("Mouse Button:"+[anEvent buttonNumber]);
  currentSelectedElement=[self graphicUnderPoint:[anEvent locationInWindow]];
  if([currentSelectedElement class]!=CPNull){
    CPLog.debug("selected graphic:"+CPStringFromRect([currentSelectedElement bounds]));
    var menu=[currentSelectedElement menuForNodeItem];
    CPLog.debug("Menu:"+menu);

    select=currentSelectedElement;
    [select setBorderWidth:3.0];
   }
}*/
- (void) mouseDown:		(CPEvent) 	anEvent	 {
  var clickcount=[anEvent clickCount];
  CPLog.debug("Click count:"+clickcount);
  
  var point=[self convertPoint:[anEvent locationInWindow] fromView:nil];
  currentSelectedElement=[self graphicUnderPoint:point];
  if(clickcount==2&&[currentSelectedElement class]!=CPNull){
    var view=[currentSelectedElement propertyView];
    if(!view)return;
    CPLog.debug("View Bounds:"+CPStringFromRect([view bounds]));
    var propertyWindow=[[NodePropertyWindow alloc] initWithView:view] ;
    [[propertyWindow contentView] addSubview:view];
    CPLog.debug("open property window");
    currentSelectedElement=[CPNull null];
    return;
  }
  currentSelectedHandle=[self handleUnderPoint:point];
  if([select class]!=CPNull){
      [select setBorderWidth:1.0];
      [select setBorderColor:[CPColor blackColor]];
      select=[CPNull null];
  }

  if([currentSelectedElement class]!=CPNull){
    CPLog.debug("selected graphic:"+CPStringFromRect([currentSelectedElement bounds]));
    CPLog.debug("Uid:"+[currentSelectedElement uid]);

    select=currentSelectedElement;
    [select setBorderWidth:3.0];
    [select setBorderColor:[CPColor greenColor]];
   }

  if(currentSelectedHandle!=nil){
    CPLog.debug("currentSelectedHandle:"+CPStringFromPoint(currentSelectedHandle));
    isDrawingLink=YES;
  }
  targetDropElement=[CPNull null];
  if([currentSelectedElement class]==CPNull&&currentSelectedHandle==nil){
    movingView=YES;
    //[CPCursor dragCopyCursor];
  }else{
    movingView=NO;  
  }
  [self setNeedsDisplay:YES];
 
  dragLocation = point;
}
   
- (void) mouseDragged:		(CPEvent) 	anEvent	 {
  var point=[self convertPoint:[anEvent locationInWindow] fromView:nil];
  [self nodeEditorViewChanged];

    //[select setBorderWidth:1.0];

//  CPLog.debug("mouseDragged:"+CPStringFromPoint([anEvent locationInWindow]));
  
  if(currentSelectedHandle){
    //CPLog.debug("Which Handle12:"+currentSelectedHandle);
    
  }else
  /**
  * this is used for moving the complete node object
  */
  if([currentSelectedElement class]!=CPNull){
    var location = point,
          originBounds = [currentSelectedElement bounds];
    var bounds=CGRectMake(location.x+(originBounds.origin.x-dragLocation.x),location.y+(originBounds.origin.y-dragLocation.y),originBounds.size.width,originBounds.size.height);
    //CPLog.debug("NewBounds:"+CPStringFromRect([currentSelectedElement bounds]));
    [currentSelectedElement setBounds:bounds];
  } 
  var targetDropHandle=[self handleUnderPoint:point];
  if(targetDropHandle!=nil){
    targetDropElement=[self graphicUnderPoint:point];
    //CPLog.debug("Target Drop Handle Here:"+targetDropElement);
    //CPLog.debug("Source Element:"+currentSelectedElement);
  }else{
    //CPLog.debug("noop");
    targetDropElement=[CPNull null];
  }
  if(movingView){
    var graphicCount = [elements count];
    for (var index = graphicCount - 1; index>=0; index--) 
    {
      var element = [elements objectAtIndex:index];
      var location = point,
          originBounds = [element bounds];
      var bounds=CGRectMake(location.x+(originBounds.origin.x-dragLocation.x),location.y+(originBounds.origin.y-dragLocation.y),originBounds.size.width,originBounds.size.height);
      //CPLog.debug("NewBounds:"+CPStringFromRect([element bounds]));
      [element setBounds:bounds];
    }

    CPLog.debug("moving all");
  }
  dragLocation = point;
  [self setNeedsDisplay:YES];
}

- (void) mouseUp:		(CPEvent) 	anEvent	 {
//  CPLog.debug("mouseUp:"+[anEvent locationInWindow]);
    //CPLog.debug("Drop Source Element:"+currentSelectedElement);
    if(isDrawingLink&&[targetDropElement class]!=CPNull&&[currentSelectedElement class]!=CPNull&&currentSelectedElement!=targetDropElement){
      //CPLog.debug("Drop Target Drop Handle Here:"+targetDropElement);
      [currentSelectedElement addTarget:targetDropElement];
      [targetDropElement addSource:currentSelectedElement];
    }

  currentSelectedElement=[CPNull null];
  currentSelectedHandle=nil;
  targetDropElement=nil;
  isDrawingLink=NO;
  [self setNeedsDisplay:YES];
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
    //CPLog.debug("ElementIndex:"+index+" Element:"+element);
		if (CPRectContainsPoint([element bounds], aPoint)) 
		{
      //CPLog.debug("under point");
      selected=element;
    }else{
      var handle=[element handleAtPoint:aPoint];
	    if(handle){
	      selected=element;
	    }
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
    //CPLog.debug("ElementIndex:"+index+" Element:"+element);
    var handle=[element handleAtPoint:aPoint];
    if(handle){
      //CPLog.debug("Result Point:"+CPStringFromPoint(handle));
      return handle;
    }
  }
  //CPLog.debug("return nil");
  return nil;
}


-(void)drawLinkFrom:(CPPoint)startPoint to:(CPPoint)endPoint withColor:(CPColor)inlineColor
{
  var p0=CPMakePoint(startPoint.x,startPoint.y);
  var p3=CPMakePoint(endPoint.x,endPoint.y);

  var p1=CPMakePoint(startPoint.x+treshold((endPoint.x-startPoint.x)/2,50),startPoint.y);
  var p2=CPMakePoint(endPoint.x-treshold((endPoint.x-startPoint.x)/2,50),endPoint.y);
  //CPLog.debug("Handle Start:"+CPStringFromPoint(p0));
  //CPLog.debug("Handle End:"+CPStringFromPoint(p3));
  
  var outlineColor=[CPColor blackColor];

  var path=[CPBezierPath bezierPath];
  [path setLineWidth:0];
  [outlineColor set];
  [path appendBezierPathWithOvalInRect:CPMakeRect(startPoint.x-2.5,startPoint.y-2.5,5,5)];
  [path fill];
  
  path=[CPBezierPath bezierPath];
  [path setLineWidth:0];
  [inlineColor set];
  [path appendBezierPathWithOvalInRect:CPMakeRect(startPoint.x-1.5,startPoint.y-1.5,3,3)];
  [path fill];

  path=[CPBezierPath bezierPath];
  [path setLineWidth:0];
  [outlineColor set];
  [path appendBezierPathWithOvalInRect:CPMakeRect(endPoint.x-2.5,endPoint.y-2.5,5,5)];
  [path fill];

  path=[CPBezierPath bezierPath];
  [path setLineWidth:0];
  [inlineColor set];
  [path appendBezierPathWithOvalInRect:CPMakeRect(endPoint.x-1.5,endPoint.y-1.5,3,3)];
  [path fill];

  path=[CPBezierPath bezierPath];
  [path setLineWidth:5];
  [path moveToPoint:p0];
  [path curveToPoint:p3 controlPoint1:p1 controlPoint2:p2];
  [outlineColor set];
  [path stroke];

  path=[CPBezierPath bezierPath];
  [path setLineWidth:3];
  [path moveToPoint:p0];
  [path curveToPoint:p3 controlPoint1:p1 controlPoint2:p2];
  [inlineColor set];
  [path stroke];


  //CPLog.debug("PathCount:"+path._path.count);
}

-(BOOL)checkPathAtPoint:(CPPoint)point from:(CPPoint)from to:(CPPoint)to
{
    var isContentsUnderPoint = NO;
  var startPoint=CPPointMake(from.x>to.x?to.x:from.x, from.y>to.y?to.y:from.y);
  var bounds=CPMakeRect(startPoint.x,startPoint.y,Math.abs(to.x-from.x),  Math.abs(to.y-from.y)+10);
  //CPLog.debug("Click  Point : "+CPStringFromPoint(point));
  //CPLog.debug("Rect Link : "+CPStringFromRect(bounds));
  
  var acceptableDistance = 20.0;
 if (CGRectContainsPoint(bounds, point)) 
	{
    var xDelta = to.x - from.x;
    if (xDelta == 0.0 && Math.abs(point.x - from.x) <= acceptableDistance) 
		{
		    isContentsUnderPoint = YES;
		}
		else 
		{
		    // Do a weak approximation of distance to the line segment.
		    var slope = (to.y - from.y) / xDelta;
		    if (Math.abs(((point.x - from.x) * slope) - (point.y - from.y)) <= acceptableDistance)
			{
				isContentsUnderPoint = YES;
		    }
		}

    //return YES;
  }
  //CPLog.debug("Selected Link : "+isContentsUnderPoint);

  return isContentsUnderPoint;
}

- (BOOL)becomeFirstResponder
 {
     return YES;
 }

- (BOOL)acceptsFirstResponder 
{ 
    return YES; 
} 

- (void)keyDown:(CPEvent)anEvent 
{ 
  if([anEvent keyCode]==46){
    CPLog.debug("delete key pressed");
    var graphicCount = [elements count];
    for (var index = graphicCount - 1; index>=0; index--) 
    {
      var element = [elements objectAtIndex:index];
      if(element==select){
        [elements removeObject:element];
        var sources=[element inputElements];
        var sourcesCount = [sources count];
        for (var index2 = sourcesCount - 1; index2>=0; index2--) 
        {
          var source = [sources objectAtIndex:index2];
          [[source outputElements] removeObject:element];
        }
        //alert("remove");
        [self setNeedsDisplay:YES];
      }
      if(element==selectedPathSource){
        //CPLog.debug("selected Path found");
        //[elements removeObject:element];
        var targets=[element outputElements];
        var targetsCount = [targets count];
        for (var index2 = targetsCount - 1; index2>=0; index2--) 
        {
          var target = [targets objectAtIndex:index2];
          if(target==selectedPathTarget){
            [[element outputElements] removeObject:target];            
            [[target inputElements] removeObject:element];            
          }
        }
        //alert("remove");
        [self setNeedsDisplay:YES];
      }
    }
  }
//    alert([anEvent keyCode]); 
} 

- (void)drawRect:(CPRect)rect 
{
  [super drawRect:rect];
  //CPLog.debug("drawRect");
  
 [[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
  var tmpname=_name;
  if([self hasUnsavedChanges])
    tmpname=_name+" * ";
  var label=[CPTextField labelWithTitle:tmpname];
  //[label setBounds:CPRectMake(10.0,10.0,200.0,100.0)];  
  [label setFrameOrigin:CPPointMake(10,10)];
  [label setFrameSize:CGSizeMake(550, 100)];
  [label setTextColor:[CPColor blackColor]];
  [label setFont:[CPFont boldSystemFontOfSize:34]];
  [label setAlphaValue:0.3];

  //[label setEditable:YES];
  //[[self contentView] addSubview:label];
  [self addSubview:label];
  var graphicCount = [elements count];
  if(graphicCount == 0)return;

  var context = [[CPGraphicsContext currentContext] graphicsPort];  
  
  for (var index = graphicCount - 1; index>=0; index--) 
	{
    var element = [elements objectAtIndex:index];
    CGContextSaveGState(context);
		[element drawContentsInView:self inRect:rect];

    if(NO){/*this scope is for handle drawing*/
      var targets=[element outputElements];
      var targetCount = [targets count];
      
      for (var index2 = targetCount - 1; index2>=0; index2--) 
      {
        var target = [targets objectAtIndex:index2];
        CPLog.debug("Source UID:"+[element uid]);
        CPLog.debug("Target UID:"+[target uid]);
        var startPoint=[element outHandlePoint];  
        var endPoint=[target inHandlePoint];
        var color=[CPColor grayColor];
        if([self checkPathAtPoint:dragLocation from:startPoint to:endPoint]){
          var color=[CPColor greenColor];
        }
        [self drawLinkFrom:startPoint to:endPoint withColor:color];
      }
    }
		CGContextRestoreGState(context);
  }
  for (var index = graphicCount - 1; index>=0; index--) 
	{
    var element = [elements objectAtIndex:index];
    //CGContextSaveGState(context);
		//[element drawContentsInView:self inRect:rect];

    {/*this scope is for handle drawing*/
      var targets=[element outputElements];
      var targetCount = [targets count];
      
      for (var index2 = targetCount - 1; index2>=0; index2--) 
      {
        var target = [targets objectAtIndex:index2];
        //CPLog.debug("Source UID:"+[element uid]);
        //CPLog.debug("Target UID:"+[target uid]);
        var startPoint=[element outHandlePoint];  
        var endPoint=[target inHandlePoint];
        var color=[CPColor grayColor];
        if([self checkPathAtPoint:dragLocation from:startPoint to:endPoint]){
          var color=[CPColor greenColor];
          selectedPathSource=element;
          selectedPathTarget=target;
          
          //CPLog.debug("selectedPath:"+selectedPath);
        }
        [self drawLinkFrom:startPoint to:endPoint withColor:color];
      }
    }
		//CGContextRestoreGState(context);
  }

  if(isDrawingLink){
    var startPoint=[currentSelectedElement outHandlePoint];
    [self drawLinkFrom:startPoint to:dragLocation withColor:[CPColor greenColor]];
  }
}
/*
 - (CPView) hitTest:		(CPPoint) 	aPoint	 	
 {
 //CPLog.debug("hit test x:"+aPoint.x+" y:"+aPoint.y);
  var element=[self graphicUnderPoint:aPoint];
  if([element class]!=CPNull){
    CPLog.debug("hit test on element");
  }
 return self;
 }
 
 - (BOOL) hitTests
 {
  return NO;
 }
*/


@end
treshold = function(x,tr){
  return (x>0)?((x>tr)?x:tr):-x+tr;
}
