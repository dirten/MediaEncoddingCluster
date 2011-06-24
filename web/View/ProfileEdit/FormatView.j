@import <AppKit/CPMenuItem.j>
@import "/View/FormBuilder.j"
@implementation FormatView :CPView
  {
    id _json;
    var nexttop;
    id option;
    CPDictionary _elements;
    CPDictionary _options;
    FormBuilder _form;
  }
  -(void)setData:(id)data
  {
    _json=data;
    [self loadFormatUI:data.id];
    [_form setData:data];
    [self setBounds:[_form bounds]];
  }
  -(id)init
  {
    nexttop=20.0;
    //CPLog.debug("format view init");
    _elements=[CPDictionary alloc];
    _options=[CPDictionary alloc];
    //[self setAutohidesScrollers:YES];
    _form=[[FormBuilder alloc] initWithFrame:CGRectMake(10, 30, CGRectGetWidth([self bounds]) , CGRectGetHeight([self bounds]))];
    option={"id":"id", "type":"string","title":"File Format","group":"picture_settings","unit":"",
                "control":{
                    "type":"ComboBox",
                    "default":"0"
                }
            };
    codecstxt = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:@"/api/v1/format"] returningResponse:nil];
    codecs=[[codecstxt rawString] objectFromJSON];
    
    var items=[{"key":"please select a file format","value":"0"}];
    for(var a=0;a<codecs.data.length;a++){
      //if(codecs.data[a].name.length>0)
        items.push({"key":codecs.data[a].longname,"value":codecs.data[a].name});
    }
    option.control.items=items;
    [self addSubview:_form];
    return self;
  }
  -(void)formatSelectionChanged:(id)sender
  {
    CPLog.debug("selection changed");
    [self loadFormatUI:[sender title]];
    [_form itemSelectionChanged:sender];
    [_form setData:_json];
  }

-(void)loadFormatUI:(id)codecid
  {
    //var path = [[CPBundle mainBundle] pathForResource:@"encoder.video.libx264.gui"];
    var codec=codecid;
    CPLog.debug("set codec UI id"+codecid);
    for(var i=0;i<codecs.data.length;i++){
      //CPLog.debug(codecs.data[i].longname)
      if(codecs.data[i].longname==codecid){
        codec=codecs.data[i].name;
        CPLog.debug("codec selected "+codecs.data[i].name);
      }
    }
    var path = [[CPBundle mainBundle] pathForResource:@"UI/format."+codec+".gui"];
    _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];
    CPLog.debug([_data rawString]);
    try{
      data=[[_data rawString] objectFromJSON];
      [_form init];

      var codecSelector=[_form buildComboBox:option];
      var itemarray=[codecSelector itemArray];
      for(var a=0;a<[itemarray count];a++){
        menuitem=[itemarray objectAtIndex:a];
        [menuitem setTarget:self];
        [menuitem setAction:@selector(formatSelectionChanged:)];
      }
      [_form buildFormView:data];
    }catch(err)
    {
      CPLog.debug(err);
      var path = [[CPBundle mainBundle] pathForResource:@"UI/format.default.gui"];
      _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];
      data=[[_data rawString] objectFromJSON];
      [_form init];
      var codecSelector=[_form buildComboBox:option];
      var itemarray=[codecSelector itemArray];
      for(var a=0;a<[itemarray count];a++){
        menuitem=[itemarray objectAtIndex:a];
        [menuitem setTarget:self];
        [menuitem setAction:@selector(formatSelectionChanged:)];
      }

      [_form buildFormView:data];
    }

  }

@end
