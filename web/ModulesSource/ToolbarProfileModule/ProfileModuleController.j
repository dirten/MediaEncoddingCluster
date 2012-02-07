@import <Foundation/Foundation.j>
@import "../../Categories/CPButtonBar+themeGray.j"
@import "../../View/ProfileEditView.j"
@import "../../View/LongOperationView.j"


@implementation ProfileModuleController : CPViewController
{
  @outlet CPTableView     profileTableView;
  @outlet CPView          profileView;
  @outlet CPButtonBar     buttonBar;
  id                      jsonData;
  ProfileEditView         profileEditView;
  id    opWin;
  InputWindow input;
  CPDictionary pdata;
}

#pragma mark -
#pragma mark Initialization

/*! called at cib awaking
 */
- (void)awakeFromCib
{
  //opWin=[[LongOperationView alloc] initWithFrame:CGRectMake(0,0,200,200)];
  [buttonBar setGrayTheme];


  var saveButton=[[CPButton alloc] initWithFrame:CGRectMake(0, 0, 55, 25)];
  [saveButton setTitle:@"Save"];
  [saveButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  //[saveButton setBordered:NO];
  [saveButton setTarget:self];
  [saveButton setAction:@selector(save:)];

  var newButton=[[CPButton alloc] initWithFrame:CGRectMake(0, 0, 55, 25)];
  [newButton setTitle:@"New"];
  [newButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [newButton setTarget:self];
  [newButton setAction:@selector(new:)];

  var deleteButton=[[CPButton alloc] initWithFrame:CGRectMake(0, 0, 55, 25)];
  [deleteButton setTitle:@"Delete"];
  [deleteButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
  [deleteButton setTarget:self];
  [deleteButton setAction:@selector(delete:)];

  var buttons=[CPArray array];
  [buttons addObject:saveButton];
  [buttons addObject:newButton];
  [buttons addObject:deleteButton];
  [buttonBar setButtons:buttons];

  CPLog.debug("awakeFromCib and reload table view new bla"+self);
  [self loadTableView];
  //var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
  //[request setHTTPMethod:"GET"];
  //var connection = [CPURLConnection connectionWithRequest:request delegate:self];
  //[buttonBar setValue:CPThemeStateDisabled forThemeAttribute:@"button-bezel-color"]
  json={
    "data":{
      "format":{},
      "video":{},
      "audio":{}
    }
  };
  pdata=[CPDictionary dictionaryWithJSObject:json recursively:YES];  
  //CPLog.debug("awakeFromCib profileEdirtView"+CPStringFromRect([profileView bounds]));
  /*
  profileEditView=[[ProfileEditView alloc] initWithData:data];
  [profileEditView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
  [[profileView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
  [profileView addSubview:profileEditView];
  */
  //[profileEditView setFrameSize:[profileView bounds]];
}

#pragma mark -
#pragma mark TNModule overrides

-(void)toolbarItemClicked:(id)aSender
{
  
}
/*! called when module is loaded
 */
- (void)willLoad
{
}

/*! called when module is unloaded
 */
- (void)willUnload
{
}

/*! called when module becomes visible
 */
- (BOOL)willShow
{
  return YES;
}

/*! called when module becomes unvisible
 */
- (void)willHide
{
}

/*! called when user permissions changed
 */
- (void)permissionsChanged
{
  
  // You may need to update your GUI to disable some
  // controls if permissions changed
}


#pragma mark -
#pragma mark Notification handlers



#pragma mark -
#pragma mark Utilities

// put your utilities here


#pragma mark -
#pragma mark Actions
- (void)tableViewSelectionDidChange:(CPNotification)aNotification{
    [[profileView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];

  if(jsonData.data[[[aNotification object] selectedRow]]){
    //CPLog.debug("hello profile 1212:"+jsonData.data[[[aNotification object] selectedRow]].id);
    selectedid=jsonData.data[[[aNotification object] selectedRow]].id;
    //var path="/api/v1/profile/"+selectedid;
    //var response=[CPHTTPURLResponse alloc];
    //var error;
    //var raw_data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:response];
    var profile=[[MHiveApiController sharedController] viewProfile:selectedid];
    //CPLog.debug("raw_data:"+[raw_data rawString]);
    //var data=[raw_data JSONObject];
    //if(data!=undefined){
    //[opWin orderFront:self];
    //[opWin center];
    //[[CPRunLoop currentRunLoop] performSelectors]; 
    //[[CPRunLoop mainRunLoop] performSelectors] ;
    //[[CPRunLoop currentRunLoop] limitDateForMode:CPDefaultRunLoopMode]; 
  //CPLog.debug("awakeFromCib profileEdirtView"+CPStringFromRect([profileView bounds]));
  //CPLog.debug("awakeFromCib profileEdirtView"+CPStringFromRect([profileView bounds]));
    pdata=[CPDictionary dictionaryWithJSObject:profile recursively:YES];
  var name="";  
  if(profile.data)
      name=profile.data.name
  [[TNGrowlCenter defaultCenter] pushNotificationWithTitle:@"Please wait" message:@"Please whait while loading profile "+name];
  [CPTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(open:) userInfo:pdata repeats:NO];

    //}
    //[opWin close];
  }
}
-(void)open:(id)data
{
    //CPLog.debug("Open Data:"+[data userInfo]);
    pdata=[data userInfo];
    profileEditView=[[ProfileEditView alloc] initWithData:pdata];
    [profileView addSubview:profileEditView];
    [profileEditView setFrameSize:CPSizeMake([profileView bounds].size.width,[profileView bounds].size.height)];

}

#pragma mark -
#pragma mark Delegates

// put your delegates here
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  jsonData=[data objectFromJSON];
  CPLog.debug("json="+jsonData.requestId);
  [profileTableView reloadData];
}

- (void)loadTableView
{
  jsonData=[[MHiveApiController sharedController] profiles];
  CPLog.debug("jsondata="+jsonData);
  [profileTableView reloadData];
}

- (int)numberOfRowsInTableView:(CPTableView)tabView
{
  var result=0;
  if(jsonData){
    result=jsonData.data.length;
  }
  CPLog.debug("RowCount="+result);
  return result;
}

- (id)tableView:(CPTableView)tabView objectValueForTableColumn:(CPTableColumn)tableColumn row:(int)row
{
  
  if([tableColumn identifier]==1){
    return [CPString stringWithFormat:@"%s", jsonData.data[row].id ];
  }else{
    if([tableColumn identifier]==2){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].name ];
    }
  }
    CPLog.debug("RowId="+jsonData.data[row].id+" selected="+selectedid);
    if(jsonData.data[row].id==selectedid){
      [self selectProfileAtIndex:row];
    }
}
- (void)selectProfileAtIndex:(unsigned)index
{
    var indexSet = index < 0 ? [CPIndexSet indexSet] : [CPIndexSet indexSetWithIndex:index];

    if (index >= 0)
        [profileTableView scrollRowToVisible:index];

    [profileTableView selectRowIndexes:indexSet byExtendingSelection:NO];
    //[self tableViewSelectionDidChange:nil];
}




- (void)delete:(id)sender
{
  CPLog.debug("Delete Encoding Profile:");
    var alert=[[YesNoAlert alloc] 
                initWithLabel:@"Delete current selected Profile!" 
                question:@"Do you really want to delete the Profile:"+selectedid
                yesLabel:@"Delete" 
                noLabel:@"Cancel" 
                target:self 
                yesAction:@selector(_delete:)
                yesObject:selectedid
                noAction:nil
                noObject:nil]; 
}

-(void)_delete:(id)data
{
  //var request = [CPURLRequest requestWithURL:@"/api/v1/profile/"+data];
  //[request setHTTPMethod:"DELETE"];
  //var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  var result=[[MHiveApiController sharedController] deleteProfile:data];
  CPLog.debug("DeleteResult:"+result);
  [[profileView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
  [self loadTableView];
  //var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
  //[request setHTTPMethod:"GET"];
  //var connection = [CPURLConnection connectionWithRequest:request delegate:self];
  selectedid=0;
  [profileTableView selectRowIndexes:[CPIndexSet indexSet] byExtendingSelection:NO];
}

- (void)save:(id)sender
{
    CPLog.debug("SAVING Encoding Profile:"+ JSON.stringify([pdata toJSON].data));
    //var url="/api/v1/profile";
    var result;
    if(selectedid){
      result=[[MHiveApiController sharedController] updateProfile:[pdata toJSON].data uuid:selectedid];
    }else{
      result=[[MHiveApiController sharedController] createProfile:[pdata toJSON].data];
    }
      
    //CPLog.debug("Saving Profile URL="+url);
    //var request = [CPURLRequest requestWithURL:url];
    //[request setHTTPMethod:"POST"];
    //[request setHTTPBody:JSON.stringify([pdata toJSON].data)];
    //var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
    CPLog.debug("SAVING Encoding Profile Result:"+ JSON.stringify(result));
    //var mdata=[result JSONObject];
    if(result.response.status==undefined||result.response.status=="error"){
       var stopWarn = [[CPAlert alloc] init];
       [stopWarn setTitle:"Failed to save the Profile?"];
       [stopWarn setMessageText:result.response.message];
       [stopWarn setAlertStyle:CPWarningAlertStyle];
       [stopWarn addButtonWithTitle:"Close"];
       [stopWarn runModal];
    }else{
      [[TNGrowlCenter defaultCenter] pushNotificationWithTitle:@"Profile Saved" message:"Profile successful saved"];
      selectedid=result.uuid;
    }
    [self loadTableView];
    //var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
    //[request setHTTPMethod:"GET"];
    //var connection = [CPURLConnection connectionWithRequest:request delegate:self];
}

- (void)new:(id)sender
{
  CPLog.debug("New Encoding Profile:");
    json={
    "data":{
      "format":{},
      "video":{},
      "audio":{}
    }
  };
  pdata=[CPDictionary dictionaryWithJSObject:json recursively:YES];  
  //CPLog.debug("awakeFromCib profileEdirtView"+CPStringFromRect([profileView bounds]));
  [profileTableView selectRowIndexes:[CPIndexSet indexSet] byExtendingSelection:NO];
  
  profileEditView=[[ProfileEditView alloc] initWithData:pdata];
  [profileEditView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
  [[profileView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
  [profileView addSubview:profileEditView];
  [profileEditView setFrameSize:CPSizeMake([profileView bounds].size.width,[profileView bounds].size.height)];

  selectedid=0;

}


- (void)alertDidEnd:(CPAlert)anAlert returnCode:(int)tag
{
  if(anAlert==input){
    var name=[input inputText];
    if (tag === 1)
      [self _new:name];
      var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
      [request setHTTPMethod:"GET"];
      var connection = [CPURLConnection connectionWithRequest:request delegate:self];
  }
}

@end

