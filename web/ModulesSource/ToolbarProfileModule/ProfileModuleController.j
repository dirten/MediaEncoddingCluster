/*
 * TNSampleTabModule.j
 *
 * Copyright (C) 2010 Antoine Mercadal <antoine.mercadal@inframonde.eu>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


@import <Foundation/Foundation.j>
@import "../../Categories/CPButtonBar+themeGray.j"
@import "../../View/ProfileEditView.j"
@import "../../View/LongOperationView.j"


// import only AppKit part you need here.
@import <AppKit/CPTextField.j>


// if you don't need this variables outside of this file,
// *always* use the 'var' keyword to make them filescoped
// otherwise, it will be application scoped
var TNArchipelTypeDummyNamespace = @"archipel:dummy",
TNArchipelTypeDummyNamespaceSayHello = @"sayhello";

/*! @defgroup  sampletabmodule Module SampleTabModule
 @desc Development starting point to create a Tab module
 */

/*! @ingroup sampletabmodule
 Sample tabbed module implementation
 Please respect the pragma marks as much as possible.
 */
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
  var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
  [request setHTTPMethod:"GET"];
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
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
    var path="/api/v1/profile?id="+selectedid;
    var response=[CPHTTPURLResponse alloc];
    var error;
    var raw_data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:response];
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
    pdata=[CPDictionary dictionaryWithJSObject:[raw_data JSONObject] recursively:YES];
  var name="";  
  if([raw_data JSONObject].data)
      name=[raw_data JSONObject].data.name
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
  var request = [CPURLRequest requestWithURL:@"/api/v1/profile?id="+data];
  [request setHTTPMethod:"DELETE"];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("DeleteResult:"+[result rawString]);
  [[profileView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
  var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
  [request setHTTPMethod:"GET"];
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
  selectedid=0;
  [profileTableView selectRowIndexes:[CPIndexSet indexSet] byExtendingSelection:NO];
}

- (void)save:(id)sender
{
    CPLog.debug("SAVING Encoding Profile:"+ JSON.stringify([pdata toJSON].data));
    var url="/api/v1/profile";
    if(selectedid)
      url+="?id="+selectedid;
    CPLog.debug("Saving Profile URL="+url);
    var request = [CPURLRequest requestWithURL:url];
    [request setHTTPMethod:"POST"];
    [request setHTTPBody:JSON.stringify([pdata toJSON].data)];
    var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
    CPLog.debug("SAVING Encoding Profile Result:"+ [result rawString]);
    var mdata=[result JSONObject];
    if(mdata.error!=undefined){
       var stopWarn = [[CPAlert alloc] init];
       [stopWarn setTitle:"Failed to save the Profile?"];
       [stopWarn setMessageText:mdata.error.description];
       [stopWarn setAlertStyle:CPWarningAlertStyle];
       [stopWarn addButtonWithTitle:"Close"];
       [stopWarn runModal];
    }else{
      [[TNGrowlCenter defaultCenter] pushNotificationWithTitle:@"Profile Saved" message:"Profile successful saved"];
      selectedid=mdata.id;
    }
    var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
    [request setHTTPMethod:"GET"];
    var connection = [CPURLConnection connectionWithRequest:request delegate:self];
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

