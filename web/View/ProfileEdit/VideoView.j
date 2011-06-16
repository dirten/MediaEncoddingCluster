@import <AppKit/CPMenuItem.j>
@import "/View/FormBuilder.j"
@implementation VideoView :CPView
  {
    id _json;
    id codecs;
    id option;
    
    var nexttop;
    CPDictionary _elements;
    CPDictionary _options;
    FormBuilder _form;
  }
  -(void)setData:(id)data
  {
    _json=data;
    CPLog.debug("setting data for profile"+data);
    [_form setData:data];
    //_json.testdata='bla';
  }
  -(id)init
  {
    nexttop=20.0;
    //CPLog.debug("format view init");
    _elements=[CPDictionary alloc];
    _options=[CPDictionary alloc];
    //[self setAutohidesScrollers:YES];
    var mainBundle = [CPBundle mainBundle];
    var path = [mainBundle pathForResource:@"encoder.video.libx264.gui"];
    //CPLog.debug(path);
    _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];
    _form=[[FormBuilder alloc] initWithFrame:CGRectMake(10, 30, CGRectGetWidth([self bounds]) , CGRectGetHeight([self bounds]))];
    data=[[_data rawString] objectFromJSON];
    option={"id":"id", "type":"string","title":"Codec","group":"picture_settings","unit":"",
      "control":{
        "type":"ComboBox",
        "default":"0"
      }
    };
    codecstxt = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:@"/api/v1/codec"] returningResponse:nil];
    codecs=[[codecstxt rawString] objectFromJSON];

    var items=[];
    for(var a=0;a<codecs.data.length;a++){
      if(codecs.data[a].type==0)
        items.push({"key":codecs.data[a].longname,"value":codecs.data[a].id});
    }
    option.control.items=items;
    [_form init];
    var codecSelector=[_form buildComboBox:option];
    var itemarray=[codecSelector itemArray];
    for(var a=0;a<[itemarray count];a++){
      menuitem=[itemarray objectAtIndex:a];
      [menuitem setTarget:self];
      [menuitem setAction:@selector(codecSelectionChanged:)];

    }
    [_form buildFormView:data];
    [self addSubview:_form];
  }

  -(void)codecSelectionChanged:(id)sender
  {
    //var path = [[CPBundle mainBundle] pathForResource:@"encoder.video.libx264.gui"];
    var codec="default";
    CPLog.debug("codec selection changed"+[sender title]);
    for(var i=0;i<codecs.data.length;i++){
      //CPLog.debug(codecs.data[i].longname)
      if(codecs.data[i].longname==[sender title]){
        codec=codecs.data[i].id;
        CPLog.debug("codec selected "+codecs.data[i].id);
      }
    }
    var path = [[CPBundle mainBundle] pathForResource:@"encoder.video."+codec+".gui"];
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
        [menuitem setAction:@selector(codecSelectionChanged:)];
      }
      [_form buildFormView:data];
    }catch(err)
    {
      CPLog.debug(err);
    }
    [_form itemSelectionChanged:sender];
    [_form setData:_json];
  }

@end
