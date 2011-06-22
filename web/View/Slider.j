@implementation Slider :CPView /*CPSlider*/
  {
    id _identifier;
    id _slider;
    id _label;
    SEL _selector;
    id _target;
  }
  -(id)initWithFrame:(CGRect)aFrame
  {
    self=[super initWithFrame:aFrame];
    CPLog.debug("Init Slider!!!!!!!!!!!!!!!!!!!!!!"+self+"---"+aFrame);

    _slider=[[CPSlider alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth([self bounds])-65, CGRectGetHeight([self bounds]))];

    _label=[[CPTextField alloc] initWithFrame:CGRectMake(CGRectGetWidth([self bounds])-60, 0, 60, CGRectGetHeight([self bounds]))];
    [_label setBordered:YES];
    [_label setBezeled:YES];
    [_label setEditable:NO];
    //_label=[CPTextField roundedTextFieldWithStringValue:@"0" placeholder:@"" width:4];
    //[_label setFrameOrigin:CGPointMake(CGRectGetWidth([self bounds])-40,0)];


    [self addSubview:_slider];
    [self addSubview:_label];
    return self;
  }
  -(void)setTarget:(id)target
  {
    _target=target;
    [_slider setTarget:self];
  }
  - (void)setAction:(SEL)selector
  {
    _selector=selector
    [_slider setAction:@selector(sliderChanged:)];
  }
  - (void)setObjectValue:(id)aValue
  {
    [self setValue:aValue];
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
    [_slider setObjectValue:value];
     [_label setObjectValue:parseInt(value)];
  }
  -(void)sliderChanged:(id)sender
{
  [_label setObjectValue:parseInt([sender objectValue])];
  [self sendAction:_selector to:_target];
}

  - (void)sendAction:(SEL)anAction to:(id)anObject
  {
    //[self _reverseSetBinding];
    [CPApp sendAction:anAction to:anObject from:self];
  }
  - (id) objectValue
  {
    return [_slider objectValue];
}
-(void)setMaxValue:(float)val
{
  [_slider setMaxValue:val];
}
-(void)setMinValue:(float)val
{
  [_slider setMinValue:val];
}
@end
