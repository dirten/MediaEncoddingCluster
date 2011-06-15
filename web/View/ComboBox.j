@implementation ComboBox :CPPopUpButton
{
  id _identifier;
}
-(void)setIdentifier:(id)identifier
{
  CPLog.debug("setting identifier to "+identifier);
  _identifier=identifier;
}
-(id)identifier
{
  return _identifier;
}

@end