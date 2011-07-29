@import <AppKit/CPView.j>

@implementation ActionsTableDataView : CPView
{
    id _value;
    id _myId;
}

- (void)setObjectValue:(id)aValue
{
  _value=aValue;
  [self setNeedsDisplay:YES];
}

- (void)drawRect:(CGRect)aRect
{
    [[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    //var l=base_value>0?"%":"";
    if(_value.id=="double click to create a new Encoding")return;
    //CPLog.debug("stop:"+_value)
    
    var stopButton=[[CPButton alloc] initWithFrame:CGRectMake(0,0,50,18)];
    [stopButton setTitle:@"Stop"];
    [stopButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
    [stopButton setTarget:self];
    [stopButton setAction:@selector(stop:)];

    var deleteButton=[[CPButton alloc] initWithFrame:CGRectMake(50,0,90,18)];
    [deleteButton setTitle:@"Delete"];
    [deleteButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
    [deleteButton setTarget:self];
    [deleteButton setAction:@selector(delete:)];

    //var label=[CPTextField labelWithTitle:base_value];
    //[label setFrameOrigin:CGPointMake(10.5,2.5)];
    //[label setTextColor:[CPColor darkGrayColor]];

    [self addSubview:stopButton];
    [self addSubview:deleteButton];

}

- (void)stop:(id)sender{
    CPLog.debug("stop:"+_value.id)
    
    var url="/api/v1/encoding";
    if(_value.id)
	url+="?id="+_value.id;
    url+="&stop";
  var request = [CPURLRequest requestWithURL:url];
  //[request setHTTPMethod:"POST"];
  //[request setHTTPBody:JSON.stringify(encodingData)];
  // see important note about CPJSONPConnection above
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug([result rawString]);
  resultData=[[result rawString] objectFromJSON];
  if(resultData.ok){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:EncodingChanged
      object:self
      userInfo:resultData.id];
    //[self close];
    var alert=[CPAlert alertWithMessageText:@"Encoding Stopped" defaultButton:@"Ok" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Encoding successfull Stopped"];
    [alert setAlertStyle:CPInformationalAlertStyle];
    [alert runModal];
  }else{
    var alert=[CPAlert alertWithError:resultData.error.description];
    [alert runModal];
  }

}

- (void)delete:(id)sender{
    CPLog.debug(sender)
    var url="/api/v1/encoding";
    if(_value.id)
	url+="?id="+_value.id;
    url+="&delete";
  var request = [CPURLRequest requestWithURL:url];
  //[request setHTTPMethod:"POST"];
  //[request setHTTPBody:JSON.stringify(encodingData)];
  // see important note about CPJSONPConnection above
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug([result rawString]);
  resultData=[[result rawString] objectFromJSON];
  if(resultData.ok){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:EncodingChanged
      object:self
      userInfo:resultData.id];
    //[self close];
    var alert=[CPAlert alertWithMessageText:@"Encoding deleted" defaultButton:@"Ok" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Encoding successfull Deleted"];
    [alert setAlertStyle:CPInformationalAlertStyle];
    [alert runModal];
  }else{
    var alert=[CPAlert alertWithError:resultData.error.description];
    [alert runModal];
  }

}
@end
