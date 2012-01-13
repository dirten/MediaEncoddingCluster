@import <AppKit/CPMenuItem.j>
@import "../FormBuilder.j"
@implementation AudioView :CPView
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
    [self loadCodecUI:[data valueForKey:@"id"]/*data.id*/];
    [_form setData:data];
  }
  -(id)init
  {
    _form=[[FormBuilder alloc] initWithFrame:CGRectMake(10, 30, CGRectGetWidth([self bounds]) , CGRectGetHeight([self bounds]))];
    option={"id":"id", "type":"string","title":"Codec","group":"picture_settings","unit":"",
      "control":{
        "type":"ComboBox",
        "defaults":"0"
      }
    };
    codecstxt = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:@"/api/v1/codec"] returningResponse:nil];
    codecs=[[codecstxt rawString] objectFromJSON];

    var items=[{"key":"please select an audio codec","value":"0"}];
    for(var a=0;a<codecs.data.length;a++){
      if(codecs.data[a].type==1)
        items.push({"key":codecs.data[a].longname,"value":codecs.data[a].id});
    }
    option.control.items=items;
    [self addSubview:_form];
    return self;
  }

  -(void)codecSelectionChanged:(id)sender
  {
    CPLog.debug("selection changed");
    [self loadCodecUI:[sender title]];
    [_form itemSelectionChanged:sender];
    [_form setData:_json];
  }
  
  -(void)loadCodecUI:(id)codecid
  {
    //var path = [[CPBundle mainBundle] pathForResource:@"encoder.video.libx264.gui"];
    var codec=codecid;
    CPLog.debug("set codec UI id"+codecid);
    for(var i=0;i<codecs.data.length;i++){
      //CPLog.debug(codecs.data[i].longname)
      if(codecs.data[i].longname==codecid){
        codec=codecs.data[i].id;
        CPLog.debug("codec selected "+codecs.data[i].id);
      }
    }
    var path = [[CPBundle mainBundle] pathForResource:@"UI/encoder.audio."+codec+".gui"];
    _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];
    //CPLog.debug([_data rawString]);
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
      CPLog.debug("AudioView::loadCodecUI:"+err);
      var path = [[CPBundle mainBundle] pathForResource:@"UI/encoder.audio.default.gui"];
      _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];
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
    }
    CPLog.debug("AudioView::loadCodecUI complete");
  }

@end
