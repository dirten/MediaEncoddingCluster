@implementation Slider :CPSlider
{
  id _identifier;
}
-(void)setIdentifier:(id)identifier
{
  //CPLog.debug("setting identifier to "+identifier);
  _identifier=identifier;
}
-(id)identifier
{
  return _identifier;
}
-(void)setValue:(id)value
{
  CPLog.debug("setting sliderBox Value to "+value);
  [self setObjectValue:value];
}

@end
