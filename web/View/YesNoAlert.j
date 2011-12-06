

@implementation YesNoAlert: CPAlert
{
  SEL yesAct;
  id  yesObj;
  SEL noAct;
  id  noObj;
  id target;
}

-(id)initWithLabel:(CPString)aLabel question:(CPString)aQuestion
{
  return [self initWithLabel:aLabel question:aQuestion yesLabel:@"Yes" noLabel:@"No" target:nil yesAction:nil yesObject:nil noAction:nil noObject:nil];
}

-(id)initWithLabel:(CPString)aLabel question:(CPString)aQuestion yesLabel:(CPString)yesLabel noLabel:(CPString)noLabel target:(id)aTarget yesAction:(SEL)yesAction yesObject:(id)yesObject
{
  return [self initWithLabel:aLabel question:aQuestion yesLabel:yesLabel noLabel:noLabel target:aTarget yesAction:yesAction yesObject:yesObject noAction:nil noObject:nil];
}

-(id)initWithLabel:(CPString)aLabel question:(CPString)aQuestion yesLabel:(CPString)yesLabel noLabel:(CPString)noLabel target:(id)aTarget yesAction:(SEL)yesAction yesObject:(id)yesObject noAction:(SEL)noAction noObject:(id)noObject
{
  self=[super init];
  [self setTitle:aLabel];
  [self setMessageText:aQuestion];
  [self setAlertStyle:CPWarningAlertStyle];
  [self addButtonWithTitle:noLabel];
  [self addButtonWithTitle:yesLabel];
  [self setDelegate:self];
  yesAct=yesAction;
  noAct=noAction;
  yesObj=yesObject;
  noObj=noObject;
  target=aTarget;
  [self runModal];
  return self;
}

- (void)alertDidEnd:(CPAlert)anAlert returnCode:(int)tag
{
  if (tag === 1){
    CPLog.debug("send yes action = "+yesAct+" to target "+target);
    if(yesAct && target)
      [target performSelector:yesAct withObject:yesObj];
      //[CPApp sendAction:yesAct to:target from:self];
  }else{
    CPLog.debug("send no action = "+noAct+" to target "+target);
    if(noAct && target)
      [target performSelector:noAct withObject:noObj];
      //[CPApp sendAction:noAct to:target from:self];
  }
}
@end