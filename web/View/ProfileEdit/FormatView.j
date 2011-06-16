@import <AppKit/CPMenuItem.j>
@import "/View/FormBuilder.j"
@implementation FormatView :CPView
  {
    id _json;
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
    var option={"id":"id", "type":"string","title":"Codec","group":"picture_settings","unit":"",
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
    [_form buildComboBox:option];
    [_form buildFormView:data];
    [self addSubview:_form];
  }

@end
