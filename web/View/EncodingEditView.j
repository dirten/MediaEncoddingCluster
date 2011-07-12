
EncodingChanged=@"EncodingChanged";
EncodingCreated=@"EncodingCreated";
EncodingCanceled=@"EncodingCanceled";

@implementation EncodingEditView :CPWindow
  {
    FormBuilder _form;
    id encodingData;
  }
  -(id)setEncodingId:(id) id{
    CPLog.debug("setting encoding id"+ id);
    if(id!=0){
      var request = [CPURLRequest requestWithURL:"/api/v1/encoding?id="+id];
      [request setHTTPMethod:"GET"];
      // see important note about CPJSONPConnection above
      var connection = [CPURLConnection connectionWithRequest:request delegate:self];
    }else{
        encodingData={};
        [_form setData:encodingData];

  }
}

-(id)init{
  self=[super initWithContentRect:CGRectMake(280,50,600,155) styleMask:CPClosableWindowMask|CPResizableWindowMask];

  [self orderFront:self];
  [self setTitle:"Encoding Editor"];
  var contentView=[self contentView];

  _form=[[FormBuilder alloc] initWithFrame:CGRectMake(10, 20, CGRectGetWidth([contentView bounds]) , CGRectGetHeight([contentView bounds])+15)];
  [_form init];
  optionProfiles={"id":"profileid", "type":"string","title":"Profile","group":"picture_settings","unit":"",
    "control":{
      "type":"ComboBox",
      "default":"0"
    }
  };
  optionInput={"id":"infile", "type":"string","title":"Inputfile","group":"picture_settings","unit":"",
    "control":{
      "type":"SearchBox",
      "default":""
    }
  };
  optionOutput={"id":"outfile", "type":"string","title":"Outputfile","group":"picture_settings","unit":"",
    "control":{
      "type":"TextBox",
      "default":""
    }
  };
  codecstxt = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:@"/api/v1/profile"] returningResponse:nil];
  codecs=[[codecstxt rawString] objectFromJSON];
  var items=[{"key":"please select a profile","value":"0"}];
  for(var a=0;a<codecs.data.length;a++){
    items.push({"key":codecs.data[a].name,"value":codecs.data[a].id});
  }
  optionProfiles.control.items=items;
  [_form buildComboBox:optionProfiles];
  
  inField=[_form buildTextBox:optionInput];
  
  [_form buildTextBox:optionOutput];
  [contentView addSubview:_form];


  var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-90,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
  [okButton setTitle:@"Save"];
  [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];

  var cancelButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-180,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
  [cancelButton setTitle:@"Cancel"];
  [cancelButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [cancelButton setTarget:self];
  [cancelButton setAction:@selector(close)];

  [contentView addSubview:okButton];
  [contentView addSubview:cancelButton];
  [okButton setTarget:self];
  [okButton setAction:@selector(save:)];
  return self;
}
- (void)inFieldChanged:(id)sender{
    array=[CPArray arrayWithObjects:["eins1","eins2","eins3","zwei"]];
    CPLog.debug(array);
    [sender setRecentSearches:array];
    CPLog.debug(sender);
}

- (void)save:(id)sender{
  CPLog.debug(JSON.stringify(encodingData));
  var url="/api/v1/encoding";
  if(encodingData.id)
    url+="?id="+encodingData.id;

  var request = [CPURLRequest requestWithURL:url];
  [request setHTTPMethod:"POST"];
  [request setHTTPBody:JSON.stringify(encodingData)];
  // see important note about CPJSONPConnection above
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug([result rawString]);
  resultData=[[result rawString] objectFromJSON];
  if(resultData.id){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:EncodingChanged
      object:self
      userInfo:resultData.id];
    [self close];
    var alert=[CPAlert alertWithMessageText:@"Encoding Created" defaultButton:@"Ok" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Encoding successfull Created"];
    [alert setAlertStyle:CPInformationalAlertStyle];
    [alert runModal];
  }else{
    var alert=[CPAlert alertWithError:resultData.error.description];
    [alert runModal];
  }
}
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  CPLog.debug("received json data:"+data);
  encodingData=[data objectFromJSON];
  //if(encodingData.error)
    //encodingData={};
  //delete encodingData.profile;
  [_form setData:encodingData];
}
- (void)connection:(CPURLConnection)aConnection didReceiveResponse:(CPHTTPURLResponse)response
{
  //CPLog.debug(response);
}

- (void)connection:(CPURLConnection)aConnection didFailWithError:(CPString)error
{
  alert(error); //a network error occurred
}

@end
