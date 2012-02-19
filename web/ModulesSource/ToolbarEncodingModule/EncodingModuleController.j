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
@import "ActivityController.j"


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
@implementation EncodingModuleController : CPViewController
{
  
  @outlet CPTableView  jobTableView;
  id jsonData;
}

#pragma mark -
#pragma mark Initialization

/*! called at cib awaking
 */
- (void)awakeFromCib
{
  CPLog.debug("awakeFromCib and reload table view new EncodingModuleController");
  
  jsonData={"data":[]};
  [jobTableView reloadData];
  //[self refresh];
  //[CPTimer scheduledTimerWithTimeInterval:10 target:self selector:@selector(refresh) userInfo:nil repeats:true];
  
  
  //[fieldJID setSelectable:YES];
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
  // you should close all your opened windows and popover now.
  
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

/*! called when entity' nickname changed
 @param aNotification the notification
 */
- (void)_didUpdateNickName:(CPNotification)aNotification
{
  [fieldName setStringValue:[_entity nickname]];
}


#pragma mark -
#pragma mark Utilities

// put your utilities here


#pragma mark -
#pragma mark Actions

/*! send hello to entity
 @param aSender the sender of the action
 */
- (IBAction)sendHello:(id)aSender
{
  // try to always proxy your IBAction like this.
}



#pragma mark -
#pragma mark Delegates

// put your delegates here


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
  if ([tableColumn identifier] == "icons"){
    // return a CPImage instance that will be displayed in the custom column (CPImageView)
    if (row % 2){
      return _iconImage1;
    }
    else if (row % 3){
      return _iconImage3;
    }
    else{
      return _iconImage2;
    }
  }else{
    // return a CPString instance that will be displayed in a regular column (CPTextField)
    //CPLog.debug([tableColumn identifier]);
    if([tableColumn identifier]==1){
      return [CPString stringWithFormat:@"%s test", jsonData.data[row].id ];
    }else if([tableColumn identifier]==2){
      if(jsonData.data[row].created>1){
        return [CPDate dateWithTimeIntervalSince1970:jsonData.data[row].created ];
      }else{
        return ""
      }
      //return [CPString stringWithFormat:@"%s", jsonData.data[row].begintime ];
    }else if([tableColumn identifier]==3){
      if(jsonData.data[row].endtime>1){
        return [CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime ];
      }else{
        return "";
      }
      //return [CPString stringWithFormat:@"%s", jsonData.data[row].endtime ];
      //return [[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime]];
    }else if([tableColumn identifier]==4){
      if(jsonData.data[row].endtime.length>1){
        return [self formatDuration:[[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime]]];
      }else{
        return "";
      }
    }else if([tableColumn identifier]==5){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].progress ];
    }else if([tableColumn identifier]==6){
      if(jsonData.data[row].fps)
        return [CPString stringWithFormat:@"%s", jsonData.data[row].fps ];
      else return "";
    }else if([tableColumn identifier]==7){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].status ];
    }else{
      if([tableColumn identifier]==8){
        return jsonData.data[row];
      }
    }
  }
}

- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  //this method is called when the network request returns. the data is the returned
  //information from flickr. we set the array of photo urls as the data to our collection view
  //CPLog.debug("status==200 : "+[_response statusCode]==200);
  
  //if([_response statusCode]==200){
    jsonData=[data objectFromJSON];
    [jobTableView reloadData];
  //}else{
    //alert(data);
  //}
}
- (void)refresh{
  var request = [CPURLRequest requestWithURL:"/api/v1/encoding"];
  [request setHTTPMethod:"GET"];
  //CPLog.debug(request.HTTPMethod);
  // see important note about CPJSONPConnection above
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
}

@end


// add this code to make the CPLocalizedString looking at
// the current bundle.
function CPBundleLocalizedString(key, comment)
{
  // DO NOT FORGET TO CHANGE THE CLASS NAME HERE
  return CPLocalizedStringFromTableInBundle(key, nil, [CPBundle bundleForClass:TNSampleTabModuleController], comment);
}
