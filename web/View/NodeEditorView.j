


@implementation NodeEditorView: CPView
{
  CPArray elements;
  id currentSelectedElement;
  CPPoint currentSelectedHandle;

  CGPoint     dragLocation;
  id lastNode;
}

-(id)initWithFrame:(id)aFrame
{
  self=[super initWithFrame:aFrame];
  if(self){
    [self registerForDraggedTypes:[NodeElementDragType]];
    elements=[CPArray array];
    lastNode=nil;
  }
  return self;
}

- (void)performDragOperation:(CPDraggingInfo)aSender
{
  var data = [[aSender draggingPasteboard] dataForType:NodeElementDragType];
  var element=[CPKeyedUnarchiver unarchiveObjectWithData:data];
  var frameOrigin=[aSender draggingLocation];  
  var bounds = [element bounds];

  [element setBounds:CGRectMake(frameOrigin.x,frameOrigin.y,bounds.size.width,bounds.size.height)];
  [elements addObject:element];
  
  if(lastNode){
    [lastNode addTarget:element];
  }else
    lastNode=element;
  [self setNeedsDisplay:YES];
  
}
   
- (void) mouseDown:		(CPEvent) 	anEvent	 {
  currentSelectedElement=[self graphicUnderPoint:[anEvent locationInWindow]];
  currentSelectedHandle=[self handleUnderPoint:[anEvent locationInWindow]];

  if([currentSelectedElement class]!=CPNull){
    CPLog.debug("selected graphic:"+CPStringFromRect([currentSelectedElement bounds]));
  }

  if(currentSelectedHandle!=nil){
    CPLog.debug("currentSelectedHandle:"+CPStringFromPoint([currentSelectedHandle]));
  }
  dragLocation = [anEvent locationInWindow];
}
   
- (void) mouseDragged:		(CPEvent) 	anEvent	 {
//  CPLog.debug("mouseDragged:"+CPStringFromPoint([anEvent locationInWindow]));
  /*
  if(currentSelectedHandle){
    CPLog.debug("Which Handle12:"+currentSelectedHandle);
    var location = [anEvent locationInWindow],
          originBounds = [currentSelectedElement bounds];

    var bx=originBounds.origin.x;
    var by=originBounds.origin.y;

    var ex=originBounds.size.width;
    var ey=originBounds.size.height;

    if(currentSelectedHandle==LineBeginHandle){
      ex+=(dragLocation.x-location.x);
      ey+=(dragLocation.y-location.y);
      bx=location.x+(originBounds.origin.x-dragLocation.x);
      by=location.y+(originBounds.origin.y-dragLocation.y);
    }
    
    if(currentSelectedHandle==LineEndHandle){
      ex-=(dragLocation.x-location.x);
      ey-=(dragLocation.y-location.y);
    }

    var bounds=CGRectMake(bx,by,ex,ey);
    [currentSelectedElement setBounds:bounds];
    dragLocation = location;
    if([self handleUnderPoint:location]){
      CPLog.debug("handle under point");
    }
  }else*/
  /**
  * this is used for moving the complete node object
  */
  if([currentSelectedElement class]!=CPNull){
    var location = [anEvent locationInWindow],
          originBounds = [currentSelectedElement bounds];
    var bounds=CGRectMake(location.x+(originBounds.origin.x-dragLocation.x),location.y+(originBounds.origin.y-dragLocation.y),originBounds.size.width,originBounds.size.height);
    //CPLog.debug("NewBounds:"+CPStringFromRect([currentSelectedElement bounds]));
    [currentSelectedElement setBounds:bounds];
    dragLocation = location;
  } 
  [self setNeedsDisplay:YES];
}

- (void) mouseUp:		(CPEvent) 	anEvent	 {
//  CPLog.debug("mouseUp:"+[anEvent locationInWindow]);
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


-(void)drawLinkFrom:(CPPoint)startPoint to:(CPPoint)endPoint
{
  var p0=CPMakePoint(startPoint.x,startPoint.y);
  var p3=CPMakePoint(endPoint.x,endPoint.y);

  var p1=CPMakePoint(startPoint.x+treshold((endPoint.x-startPoint.x)/2,50),startPoint.y);
  var p2=CPMakePoint(endPoint.x-treshold((endPoint.x-startPoint.x)/2,50),endPoint.y);
  
  var inlineColor=[CPColor grayColor];
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

}

- (void)drawRect:(CPRect)rect 
{
  var context = [[CPGraphicsContext currentContext] graphicsPort];  
  var graphicCount = [elements count];
  for (var index = graphicCount - 1; index>=0; index--) 
	{
    var element = [elements objectAtIndex:index];
    CGContextSaveGState(context);
		[element drawContentsInView:self inRect:rect];

    {/*this scope is for handle drawing*/
      var targets=[element outputElements];
      var targetCount = [targets count];
      for (var index2 = targetCount - 1; index2>=0; index2--) 
      {
        var target = [targets objectAtIndex:index2];
        var startPoint=[element outHandlePoint];  
        var endPoint=[target inHandlePoint];
        [self drawLinkFrom:startPoint to:endPoint];
      }
    }
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
treshold = function(x,tr){
  return (x>0)?((x>tr)?x:tr):-x+tr;
}
