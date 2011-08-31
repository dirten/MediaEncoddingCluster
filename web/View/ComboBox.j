@implementation ComboBox :CPPopUpButton
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
    //CPLog.debug("setting comboBox Value to "+value);
    var itemarray=[self itemArray];
    for(var a=0;a<[itemarray count];a++){
      menuitem=[itemarray objectAtIndex:a];
      if(value==[menuitem title])
        [self setSelectedIndex:a];
    }

  }
  
@end