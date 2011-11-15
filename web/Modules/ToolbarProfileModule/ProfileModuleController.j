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
  
}

#pragma mark -
#pragma mark Initialization

/*! called at cib awaking
 */
- (void)awakeFromCib
{
  var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
  [request setHTTPMethod:"GET"];
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
  [buttonBar setValue:CPThemeStateDisabled forThemeAttribute:@"button-bezel-color"]
  //profileView=[[ProfileEditView alloc] initWithFrame:[profileView bounds]];
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
  if(jsonData.data[[[aNotification object] selectedRow]]){
    CPLog.debug("hello profile:"+jsonData.data[[[aNotification object] selectedRow]].id);
    selectedid=jsonData.data[[[aNotification object] selectedRow]].id;
    [[CPNotificationCenter defaultCenter]
     postNotificationName:ProfileClicked
     object:self
     userInfo:selectedid];
  }
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
  }else
    if([tableColumn identifier]==2){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].name ];
    }
}

@end

