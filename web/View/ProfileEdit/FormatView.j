@implementation FormatView :CPScrollView
  {
    id _json;
    var nexttop;
    CPDictionary _elements;
  }

  -(id)init
  {
     nexttop=20.0;
    CPLog.debug("format view init");
    [self setAutohidesScrollers:YES];
    var mainBundle = [CPBundle mainBundle];
    var path = [mainBundle pathForResource:@"encoder.video.libx264.gui"];
    CPLog.debug(path);
    _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];

    [self buildFormView:[[_data rawString] objectFromJSON]];
  }

  -(void)buildFormView:(id)data
  {
    for(a=0;a<data.gui.options.length;a++){
      switch (data.gui.options[a].control.type) {
        case "TextBox":
        [self buildTextBox:data.gui.options[a]];
        break;
        case "ComboBox":
        [self buildComboBox:data.gui.options[a]];
        break;
        case "Slider":
        [self buildSlider:data.gui.options[a]];
        break;
        default:
        alert("Sie bleiben leider dumm");
        break;
      }
      CPLog.debug(data.gui.options[a].id);
      /*removing all elements from this view*/
      //[self setSubviews:[CPArray array]];
    }
  }

  -(void)buildTextBox:(id)data
  {
    CPLog.debug("build textbox")
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:350 ];
    [field setFrameOrigin:CGPointMake(125.0,nexttop-5.0)];

    [_elements setObject:field forKey:data.id];

    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];
  }

  -(void)buildComboBox:(id)data
  {
    CPLog.debug("build combobox")
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    //field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:350 ];
    field = [[CPPopUpButton alloc] initWithFrame:CGRectMake(127.0,nexttop-5.0, 345, 25) pullsDown:NO];
    for(i=0;i<data.control.items.length;i++){
      [field addItemWithTitle:data.control.items[i].key];
      if(data.control.default==data.control.items[i].value)
        [field setSelectedIndex:i];
    }
    [_elements setObject:field forKey:data.id];
    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];

  }

  -(void)buildSlider:(id)data
  {
    CPLog.debug("build slider")

  }

@end
