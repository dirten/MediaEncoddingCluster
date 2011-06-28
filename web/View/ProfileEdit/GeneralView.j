@import "/View/LabelTextFieldView.j"
@implementation GeneralView :CPView
  {
    FormBuilder _form;
  }
  -(void)setData:(id)data
  {

    [_form setData:data];
  }
  -(id)init
  {
    var data={"gui":{"options":[
      {"id":"id", "type":"string","title":"Profile Id","group":"picture_settings","unit":"",
        "control":{
          "type":"InfoBox",
          "defaults":"                                                                                                   "
        }
      },
      {"id":"name", "type":"string","title":"Profile Name","group":"picture_settings","unit":"",
        "control":{
          "type":"TextBox",
          "defaults":""
        }
      }]}};
      _form=[[FormBuilder alloc] initWithFrame:CGRectMake(10, 30, CGRectGetWidth([self bounds]) , CGRectGetHeight([self bounds]))];
      [_form init];
      [_form buildFormView:data];
      [self addSubview:_form];
      return self;
    }
  @end