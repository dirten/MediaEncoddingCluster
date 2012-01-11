@import <AppKit/CPMenuItem.j>
@import "../FormBuilder.j"
@implementation VideoView :CPScrollView
  {
    id _json;
    id codecs;
    id option;

    var nexttop;
    CPDictionary _elements;
    CPDictionary _options;
    FormBuilder _form;
    CPView mainView;
  }
  -(void)setData:(id)data
  {
    _json=data;
    [self loadCodecUI:[data valueForKey:@"id"]/*data.id*/];
    [_form setData:data];
    //[self setBounds:[_form bounds]];
  }
  -(id)initWithFrame:(CGRect)frame
  {
  var self=[super initWithFrame:frame];
  //mainView = [[CPView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth([self bounds]), CGRectGetHeight([self bounds])+1000)];  
  //[mainView setBackgroundColor:[CPColor darkGrayColor ]];
  _form=[[FormBuilder alloc] initWithFrame:CGRectMake(10, 30, CGRectGetWidth([self bounds]) , CGRectGetHeight([self bounds])+1000)];

    option={"id":"id", "type":"string","title":"Codec","group":"picture_settings","unit":"",
      "control":{
        "type":"ComboBox",
        "defaults":"0"
      }
    };
    var codecstxt = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:@"/api/v1/codec"] returningResponse:nil];
    codecs=[[codecstxt rawString] objectFromJSON];

    var items=[{"key":"please select a video codec","value":"0"}];
    for(var a=0;a<codecs.data.length;a++){
      if(codecs.data[a].type==0)
        items.push({"key":codecs.data[a].longname,"value":codecs.data[a].id});
    }
    option.control.items=items;
    //[self loadCodecUI:undefined];
    //[mainView addSubview:_form]
    //[self addSubview:mainView];
    [self setDocumentView:_form];
    [self setHasHorizontalScroller:NO];
//[self setNeedsDisplay:YES];

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
      [_form init];
      
    var codec=codecid;
    //CPLog.debug("set codec UI id"+codecid);
    for(var i=0;i<codecs.data.length;i++){
      //CPLog.debug(codecs.data[i].longname)
      if(codecs.data[i].longname==codecid){
        codec=codecs.data[i].id;
        //CPLog.debug("codec selected "+codecs.data[i].id);
      }
    }
    

    var codecSelector=[_form buildComboBox:option];
    var itemarray=[codecSelector itemArray];
    for(var a=0;a<[itemarray count];a++){
      menuitem=[itemarray objectAtIndex:a];
      [menuitem setTarget:self];
      [menuitem setAction:@selector(codecSelectionChanged:)];
    }

    var path = [[CPBundle mainBundle] pathForResource:@"UI/encoder.video."+codec+".gui"];
    _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];
    //CPLog.debug([_data rawString]);
    try{
      data=[[_data rawString] objectFromJSON];
      [_form buildFormView:data];
    }catch(err)
    {
      CPLog.debug(err);
      if(codecid&&codecid!=option.control.items[0].key){
        var path = [[CPBundle mainBundle] pathForResource:@"UI/encoder.video.default.gui"];
        _data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:nil];
        data=[[_data rawString] objectFromJSON];
        [_form buildFormView:data];
      }
    }
  }

@end
