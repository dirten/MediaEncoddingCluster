@implementation SearchBox :CPSearchField
{
  id _identifier;
}
-(void)setIdentifier:(id)identifier
{
  _identifier=identifier;
}
-(id)identifier
{
  return _identifier;
}
-(void)setValue:(id)value
{
  CPLog.debug("setting textBox Value to "+value);
  [self setObjectValue:value];
}
@end
