


@implementation InputWindow : CPAlert
{
  TextBox field;
}
-(id)initWithTitle:(CPString)aTitle andText:(CPString)aText
{
  self=[super init];
  [self setTitle:aTitle];
  [self setMessageText:aText];
  [self addButtonWithTitle:"Cancel"];
  [self addButtonWithTitle:"Ok"];
  [self setAlertStyle:CPInformationalAlertStyle];
  
  //[self setDelegate:self];
  field = [TextBox textFieldWithStringValue:@"" placeholder:@"type in file location" width:280];
  [field setFrameOrigin:CGPointMake(80.0,40.0)];
  //[field setDelegate:self];
  [field setIdentifier:@"infile"];

  [self runModal];
  [[self._window contentView] addSubview:field];
  CPLog.debug("after modal");
  return self;
}
-(CPString)inputText
{
  return [field stringValue];
}
@end