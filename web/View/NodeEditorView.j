


@implementation NodeEditorView: CPView
{

}

-(id)initWithFrame:(id)aFrame
{
  self=[super initWithFrame:aFrame];
  if(self){
    [self registerForDraggedTypes:[NodeElementDragType]];
  }
  return self;
}
- (void)performDragOperation:(CPDraggingInfo)aSender
{
    var data = [[aSender draggingPasteboard] dataForType:NodeElementDragType];
    CPLog.debug("drop_dragging:"+data);
    var element=[CPKeyedUnarchiver unarchiveObjectWithData:data];
    CPLog.debug("Element:"+[element name]);

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


- (CPView) hitTest:		(CPPoint) 	aPoint	 	
{
  CPLog.debug("hit test x:"+aPoint.x+" y:"+aPoint.y);
  return nil;
}

- (BOOL) hitTests
{
  return YES;
}
@end

@implementation NodeItemView : CPView
{
    CPTextField textField;
}

- (void)setRepresentedObject:(id)anObject
{
    if (!textField)
    {
        textField = [[CPTextField alloc] initWithFrame:CGRectMake(50.0, 15.0, 145.0, 20.0)];
        [textField setFont:[CPFont boldSystemFontOfSize:12.0]];
        [self addSubview:textField];
    }
    [textField setStringValue:[anObject name]];
}

- (void)setSelected:(BOOL)isSelected
{
    [self setBackgroundColor:isSelected ? [CPColor blueColor] : nil];
    [textField setTextColor:isSelected ? [CPColor whiteColor] : [CPColor blackColor]];
}

@end
