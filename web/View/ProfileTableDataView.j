@import <AppKit/CPView.j>

@implementation ProfileTableDataView : CPView
{
    id _data;
    CPDictionary _elements;
    CPDictionary _options;

}

- (void)setObjectValue:(id)aValue
{
    _data=aValue;
    [self setNeedsDisplay:YES];
    //[self drawRectself:""];
}

- (void)drawRect:(CGRect)aRect
{
    [[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    /*
    var bounds = [self bounds],
        //maxWidth = (bounds.size.width - 2) * value,
        context = [[CPGraphicsContext currentContext] graphicsPort],
        rectToDraw = CGRectMake(2, 2, 5, bounds.size.height - 4),
        transform = CGAffineTransformMakeTranslation(6, 0);
    */

    //var label=[CPTextField labelWithTitle:_data.control.type];
    //[label setFrameOrigin:CGPointMake(10.5,2.5)];
    //[label setTextColor:[CPColor darkGrayColor]];

    //[self addSubview:label];
    //return;

      switch (_data.gui.control.type) {
        case "TextBox":
        [self buildTextBox:_data];
        break;
        case "SearchBox":
        [self buildSearchBox:_data];
        break;
        case "ComboBox":
        [self buildComboBox:_data];
        break;
        case "Slider":
        [self buildSlider:_data];
        break;
        case "CheckBox":
        [self buildCheckBox:_data];
        break;
        case "InfoBox":
        [self buildInfoBox:_data];
        break;
        default:
        break;
      }

}
  -(void)buildInfoBox:(id)data
  {

    field = [TextBox labelWithTitle:data.gui.control.defaults];
    [field setDelegate:self];
    [field setIdentifier:data.gui.id];
    [self addSubview:field];

  }
  -(void)buildTextBox:(id)data
  {
    field = [TextBox textFieldWithStringValue:data.gui.control.defaults placeholder:@"type in "+data.gui.title width:350 ];
    [field setDelegate:self];
    [field setIdentifier:data.gui.id];
    [self addSubview:field];
  }
  -(id)buildSearchBox:(id)data
  {
    field = [SearchBox textFieldWithStringValue:data.gui.control.defaults placeholder:@"type in "+data.gui.title width:350 ];
    [field setIdentifier:data.gui.id];
    [self addSubview:field];
    //return field;
  }

  -(id)buildComboBox:(id)data
  {
    field = [[ComboBox alloc] initWithFrame:CGRectMake(0,0, 342, 25) pullsDown:NO];
    for(i=0;i<data.gui.control.items.length;i++){
      [field addItemWithTitle:data.gui.control.items[i].key];
      var item = [field itemAtIndex:i];
      //[item setTarget:self];
      //[item setAction:@selector(itemSelectionChanged:)];
      [item setRepresentedObject:data.gui.id];
      if(data.gui.control.defaults==data.gui.control.items[i].value)
        [field setSelectedIndex:i];
    }
    //[_elements setObject:field forKey:data.id];
    //[field setEnabled:NO];
    //nexttop+=25.0;
    //[self addSubview:label];
    [self addSubview:field];
    return field;
  }

  -(void)buildSlider:(id)data
  {
    //[_options setObject:data forKey:data.id];
    //CPLog.debug("build slider")
    //label=[CPTextField labelWithTitle:data.title];
    //[label setFrameOrigin:CGPointMake(0.0,nexttop)];
    //[label setAlignment:CPRightTextAlignment];

    field = [[Slider alloc] initWithFrame:CGRectMake(0,0, 342, 25)];
    //[field setFrameOrigin:CGPointMake(left_bound+4,nexttop)];

    //[field setTarget:self];
    //[field setAction:@selector(sliderChanged:)];
    [field setIdentifier:data.gui.id];
    //[field setObjectValue:data.gui.control.defaults];
    [field setValue:1000];
    //[field setValue:data.data[data.gui.id]];
    [field setMinValue:data.gui.control.min];
    [field setMaxValue:data.gui.control.max];
    //[_elements setObject:field forKey:data.id];

    //nexttop+=25.0;
    //[self addSubview:label];
    [self addSubview:field];

  }
  -(void)buildCheckBox:(id)data
  {
    //[_options setObject:data forKey:data.id];
    //CPLog.debug("buildcheck box")
    //label=[CPTextField labelWithTitle:data.title];
    //[label setFrameOrigin:CGPointMake(0.0,nexttop)];
    //[label setAlignment:CPRightTextAlignment];

    field = [CheckBox checkBoxWithTitle:@"" ];
    //[field setFrameOrigin:CGPointMake(0,0)];
    [field setTarget:self];
    [field setAction:@selector(checkBoxStateChanged:)];
    [field setIdentifier:data.gui.id];
    [field setObjectValue:data.gui.control.defaults];
    //[_elements setObject:field forKey:data.id];

    //nexttop+=25.0;
    //[self addSubview:label];
    [self addSubview:field];

  }

@end
