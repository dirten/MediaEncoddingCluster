


@implementation NodeEditorView: CPView
{
  CPArray elements;
  id currentSelectedElement;
  id targetDropElement;
  id select;
  CPPoint currentSelectedHandle;

  CGPoint     dragLocation;
  id lastNode;
  BOOL isDrawingLink;
 
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
  /*
  if(lastNode){
    [lastNode addTarget:element];
  }else
    lastNode=element;
    */
  [self setNeedsDisplay:YES];
  
}
   
- (void) mouseDown:		(CPEvent) 	anEvent	 {
  currentSelectedElement=[self graphicUnderPoint:[anEvent locationInWindow]];
  currentSelectedHandle=[self handleUnderPoint:[anEvent locationInWindow]];
  if([select class]!=CPNull){
      [select setBorderWidth:1.0];
      select=[CPNull null];
  }

  if([currentSelectedElement class]!=CPNull){
    CPLog.debug("selected graphic:"+CPStringFromRect([currentSelectedElement bounds]));

    select=currentSelectedElement;
    [select setBorderWidth:3.0];
   }

  if(currentSelectedHandle!=nil){
    CPLog.debug("currentSelectedHandle:"+CPStringFromPoint(currentSelectedHandle));
    isDrawingLink=YES;
  }
  [self setNeedsDisplay:YES];
 
  dragLocation = [anEvent locationInWindow];
}
   
- (void) mouseDragged:		(CPEvent) 	anEvent	 {
    //[select setBorderWidth:1.0];

//  CPLog.debug("mouseDragged:"+CPStringFromPoint([anEvent locationInWindow]));
  
  if(currentSelectedHandle){
    CPLog.debug("Which Handle12:"+currentSelectedHandle);
    
  }else
  /**
  * this is used for moving the complete node object
  */
  if([currentSelectedElement class]!=CPNull){
    var location = [anEvent locationInWindow],
          originBounds = [currentSelectedElement bounds];
    var bounds=CGRectMake(location.x+(originBounds.origin.x-dragLocation.x),location.y+(originBounds.origin.y-dragLocation.y),originBounds.size.width,originBounds.size.height);
    //CPLog.debug("NewBounds:"+CPStringFromRect([currentSelectedElement bounds]));
    [currentSelectedElement setBounds:bounds];
  } 
  var targetDropHandle=[self handleUnderPoint:[anEvent locationInWindow]];
  if(targetDropHandle!=nil){
    targetDropElement=[self graphicUnderPoint:[anEvent locationInWindow]];
    CPLog.debug("Target Drop Handle Here:"+targetDropElement);
    CPLog.debug("Source Element:"+currentSelectedElement);
  }else{
    CPLog.debug("noop");
    targetDropElement=[CPNull null];
  }
  dragLocation = [anEvent locationInWindow];
  [self setNeedsDisplay:YES];
}

- (void) mouseUp:		(CPEvent) 	anEvent	 {
//  CPLog.debug("mouseUp:"+[anEvent locationInWindow]);
    CPLog.debug("Drop Target Drop Handle Here:"+targetDropElement);
    CPLog.debug("Drop Source Element:"+currentSelectedElement);
    if(isDrawingLink&&[targetDropElement class]!=CPNull&&currentSelectedElement!=targetDropElement){
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


-(void)drawLinkFrom:(CPPoint)startPoint to:(CPPoint)endPoint withColor:(CPColor)inlineColor
{
  var p0=CPMakePoint(startPoint.x,startPoint.y);
  var p3=CPMakePoint(endPoint.x,endPoint.y);

  var p1=CPMakePoint(startPoint.x+treshold((endPoint.x-startPoint.x)/2,50),startPoint.y);
  var p2=CPMakePoint(endPoint.x-treshold((endPoint.x-startPoint.x)/2,50),endPoint.y);
  
  
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


  CPLog.debug("PathCount:"+path._path.count);
}

-(BOOL)checkPathAtPoint:(CPPoint)point from:(CPPoint)from to:(CPPoint)to
{
    var isContentsUnderPoint = NO;

  var bounds=CPMakeRect(from.x,from.y,to.x-from.x,  to.y-from.y);
  CPLog.debug("Rect Link : "+CPStringFromRect(bounds));
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
  return isContentsUnderPoint;
}
- (BOOL)acceptsFirstResponder 
{ 
    return YES; 
} 

- (void)keyDown:(CPEvent)anEvent 
{ 
  if([anEvent keyCode]==46&&[select class]!=CPNull){
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
    }
  }
//    alert([anEvent keyCode]); 
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
        var color=[CPColor grayColor];
        if([self checkPathAtPoint:dragLocation from:startPoint to:endPoint]){
          CPLog.debug("path found");
          var color=[CPColor greenColor];
        }
        [self drawLinkFrom:startPoint to:endPoint withColor:color];
      }
    }
		CGContextRestoreGState(context);
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
