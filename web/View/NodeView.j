


@implementation NodeView: CPBox
{
  CPString name;
  CGPoint     dragLocation;
}


-(id)initWithName:(CPString)aName
{
  self=[super initWithFrame:CGRectMake(0.0,0.0,130.0,50.0)];
  if(self){
    name=aName;
    [self setCornerRadius:3.0];
//    [self setBorderWidth:3.0];
    [self setBorderType:CPGrooveBorder];
    [self setPostsFrameChangedNotifications:YES];
    var label=[CPTextField labelWithTitle:aName];
    [label setFrameOrigin:CGPointMake(10.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];
    [self addSubview:label];
  }
  return self;
}

-(CPString)name
{
  return name;
}

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

-(void)drawContentsInView:(id)view inRect:(id)aRect
{
  CPLog.debug("RectSelf:"+CPStringFromRect([self bounds]));
    [super drawRect:aRect];

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
