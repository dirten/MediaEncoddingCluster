@import <AppKit/CPMenuItem.j>
@import "/View/TextBox.j"
@import "/View/SearchBox.j"
@import "/View/ComboBox.j"
@import "/View/CheckBox.j"
@import "/View/Slider.j"
@implementation FormBuilder :CPView
  {
    id _json;
    var nexttop;
    CPDictionary _elements;
    CPDictionary _options;
    var left_bound;
  }
    
  -(void)setData:(id)data
  {
    CPLog.debug("setting data for profile in form builder"+JSON.stringify(data));
    _json=data;
//          _json[data.id]=data.control["default"];

    var keys=[_elements allKeys];
    for(var i=0;i<[keys count];i++){
      CPLog.debug("Looking for Data in json "+[keys objectAtIndex:i])
      if(_json[[keys objectAtIndex:i]]!=undefined){
        CPLog.debug("Data in json found "+[keys objectAtIndex:i])
        var el=[_elements objectForKey:[keys objectAtIndex:i]];
        var op=[_options objectForKey:[keys objectAtIndex:i]];
        if(op.control.items){
          CPLog.debug("Map Key to value for"+[keys objectAtIndex:i]);
          for(var b=0;b<op.control.items.length;b++){
            opitem=op.control.items[b];
            CPLog.debug("Key="+opitem.key+" val="+_json[[keys objectAtIndex:i]]);
            if(opitem.value==_json[[keys objectAtIndex:i]])
              [el setValue:opitem.key];
          }
        }else
        {
          [el setValue:_json[[keys objectAtIndex:i]]];
        }
      }else{
      //var el=[_elements objectForKey:[keys objectAtIndex:i]];
      var op=[_options objectForKey:[keys objectAtIndex:i]];
      _json[[keys objectAtIndex:i]]=op.control.defaults;
      CPLog.debug("setting default "+[keys objectAtIndex:i]);
      }
    }
    //_json.testdata='bla';
  }
  -(id)init
  {
    _elements=[CPDictionary alloc];
    _options=[CPDictionary alloc];
    nexttop=20.0;
    left_bound=145;

    [self setSubviews:[CPArray array]];

  }

  -(void)buildFormView:(id)data
  {
    [self setBounds:CGRectMake(0, 0, 300, 50000)];
    for(a=0;a<data.gui.options.length;a++){
      switch (data.gui.options[a].control.type) {
        case "TextBox":
        [self buildTextBox:data.gui.options[a]];
        break;
        case "SearchBox":
        [self buildSearchBox:data.gui.options[a]];
        break;
        case "ComboBox":
        [self buildComboBox:data.gui.options[a]];
        break;
        case "Slider":
        [self buildSlider:data.gui.options[a]];
        break;
        case "CheckBox":
        [self buildCheckBox:data.gui.options[a]];
        break;
        case "InfoBox":
        [self buildInfoBox:data.gui.options[a]];
        break;
        default:
        //alert("Sie bleiben leider dumm");
        break;
      }
      CPLog.debug(data.gui.options[a].id);
      /*removing all elements from this view*/
      //[self setSubviews:[CPArray array]];
    }
  }
  -(void)buildInfoBox:(id)data
  {
    [_options setObject:data forKey:data.id];
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    field = [TextBox labelWithTitle:data.control.defaults];
    [field setFrameOrigin:CGPointMake(left_bound,nexttop-5.0)];
    [field setDelegate:self];
    [field setIdentifier:data.id];
    [_elements setObject:field forKey:data.id];

    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];

  }
  -(void)buildTextBox:(id)data
  {
    [_options setObject:data forKey:data.id];
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    field = [TextBox textFieldWithStringValue:data.control.defaults placeholder:@"type in "+data.title width:350 ];
    [field setFrameOrigin:CGPointMake(left_bound,nexttop-5.0)];
    [field setDelegate:self];
    [field setIdentifier:data.id];
    [_elements setObject:field forKey:data.id];

    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];
  }
  -(id)buildSearchBox:(id)data
  {
    [_options setObject:data forKey:data.id];
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    field = [SearchBox textFieldWithStringValue:data.control.defaults placeholder:@"type in "+data.title width:350 ];
    [field setFrameOrigin:CGPointMake(left_bound,nexttop-5.0)];
    //[field setDelegate:self];
    [field setIdentifier:data.id];
    [_elements setObject:field forKey:data.id];

    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];
    return field;
  }

  -(id)buildComboBox:(id)data
  {
    [_options setObject:data forKey:data.id];
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    //field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:350 ];
    field = [[ComboBox alloc] initWithFrame:CGRectMake(left_bound+4,nexttop-5.0, 342, 25) pullsDown:NO];
    for(i=0;i<data.control.items.length;i++){
      [field addItemWithTitle:data.control.items[i].key];
      var item = [field itemAtIndex:i];
      [item setTarget:self];
      [item setAction:@selector(itemSelectionChanged:)];
      [item setRepresentedObject:data.id];
      if(data.control.defaults==data.control.items[i].value)
        [field setSelectedIndex:i];
    }
    [_elements setObject:field forKey:data.id];
    //[field setEnabled:NO];
    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];
    return field;
  }

  -(void)buildSlider:(id)data
  {
    [_options setObject:data forKey:data.id];
    CPLog.debug("build slider")
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    field = [[Slider alloc] initWithFrame:CGRectMake(left_bound+4,nexttop-5.0, 342, 25)];
    [field setFrameOrigin:CGPointMake(left_bound+4,nexttop)];

    [field setTarget:self];
    [field setAction:@selector(sliderChanged:)];
    [field setIdentifier:data.id];
    [field setObjectValue:data.control.defaults];
    [field setMinValue:data.control.min];
    [field setMaxValue:data.control.max];
    [_elements setObject:field forKey:data.id];

    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];

  }
  -(void)buildCheckBox:(id)data
  {
    [_options setObject:data forKey:data.id];
    CPLog.debug("buildcheck box")
    label=[CPTextField labelWithTitle:data.title];
    [label setFrameOrigin:CGPointMake(0.0,nexttop)];
    [label setAlignment:CPRightTextAlignment];

    field = [CheckBox checkBoxWithTitle:@"" ];
    [field setFrameOrigin:CGPointMake(left_bound+4,nexttop)];
    [field setTarget:self];
    [field setAction:@selector(checkBoxStateChanged:)];
    [field setIdentifier:data.id];
    [field setObjectValue:data.control.defaults];
    [_elements setObject:field forKey:data.id];

    nexttop+=25.0;
    [self addSubview:label];
    [self addSubview:field];

  }
  -(void)checkBoxStateChanged:(id)sender
  {
    CPLog.debug("checked"+[sender objectValue]);
    _json[[sender identifier]]=[sender objectValue];
  }
  -(void)sliderChanged:(id)sender
  {

    CPLog.debug("changed"+parseInt([sender objectValue]));
    _json[[sender identifier]]=parseInt([sender objectValue]);
  }
  -(void)itemSelectionChanged:(id)sender
  {
    CPLog.debug("selection changed:"+[sender representedObject]);
    if([_options containsKey:[sender representedObject]]){
      CPLog.debug("key found:"+[sender representedObject]);
      var option=[_options objectForKey:[sender representedObject]];

      for(var i=0;i<option.control.items.length;i++){
        var item=option.control.items[i]
        if(item.key==[sender title]){
          if(option.persist!=false)
            _json[[sender representedObject]]=item.value;
          /*enable referenced data fields*/
          if(item.enable&&item.enable.length>0){
            for(var b=0;b<item.enable.length;b++){
              var eitem=item.enable[b];
              CPLog.debug("enable item"+eitem);
            }
          }
          /*setting refernce data fields*/
          if(item.data&&item.data.length>0){
            for(var a=0;a<item.data.length;a++){
              var data=item.data[a];
              if([_elements containsKey:data.option]){
                CPLog.debug("setting data from option "+data.option+" to "+data.value);
                var data_option=[_elements objectForKey:data.option];
                [data_option setObjectValue:data.value];
                _json[data.option]=data.value;
              }
            }
          }
        }
      }
    }else{
    CPLog.error("Key "+[sender representedObject]+" does not exist in dictonary");
  }
}
-(void)controlTextDidChange:(CPNotification)aNotification
{
  CPLog.debug("text changed:"+[[aNotification object] identifier]);
  _json[[[aNotification object] identifier]]=[[aNotification object] objectValue];
}

@end
