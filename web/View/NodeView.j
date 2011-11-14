


@implementation NodeView: CPView
{
  CPString name;
}


-(id)initWithName:(CPString)aName
{
  self=[super initWithFrame:CGRectMake(0.0,0.0,30.0,50.0)];
  if(self){
    name=aName;
    //[self setPostsFrameChangedNofication:YES];
  }
  return self;
}

-(CPString)name
{
  return name;
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

- (CPView) hitTest:		(CPPoint) 	aPoint	 	
{
  CPLog.debug("hit test");
  return nil;
}

- (BOOL) hitTests
{
  return YES;
}
@end
