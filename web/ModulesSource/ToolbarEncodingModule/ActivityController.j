//
//  ActivityController.m
//  web
//
//  Created by Jan Hölscher on 12.11.11.
//  Copyright (c) 2011 280 North, Inc. All rights reserved.
//
@import "../../View/ProgressTableDataView.j"
//@import "/Resources/js/dateFormat.js"

@implementation ActivityController: CPObject
{
  @outlet CPTableView  jobTableView;
  @outlet CPView       webView;
  id                   activityWebView;
  id jsonData;
  id selectedid;
  id selectedRowIndex;
  id growl;
  
  CPAlert stopGraph;
}

-(void)awakeFromCib{
  CPLog.debug("awaked from cib ActivityController")
  var progressDataView=[ProgressTableDataView new];
  //CPLog.debug("ProgressTableDataView:"+progressDataView);
  //CPLog.debug("JobTable:"+jobTableView);
  var column = [jobTableView tableColumnWithIdentifier:@"5"];
  //CPLog.debug("Column:"+column);
  [column setDataView:progressDataView];
  [self refresh];
  var bundle      = [CPBundle bundleForClass:[self class]]
  activityWebView=[[[DetailWebView alloc] initWithFrame:[webView bounds]] initWithTemplate:[bundle pathForResource:@"JobWebView.html"]];
  //activityWebView=[[[DetailWebView alloc] initWithFrame:[webView bounds]] initWithTemplate:"Resources/JobWebView.html"];
  [webView addSubview:activityWebView];
  [CPTimer scheduledTimerWithTimeInterval:10 target:self selector:@selector(refresh) userInfo:nil repeats:true];
  
}

- (int)numberOfRowsInTableView:(CPTableView)tabView
{
  var result=0;
  if(jsonData){
    result=jsonData.length;
  }
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
      return [CPString stringWithFormat:@"%s (%s)", jsonData[row].name, jsonData[row].filename];
    }else if([tableColumn identifier]==2){
      if(jsonData[row].created>1){
        //return [CPDate date:jsonData.data[row].created withFormat:@"isoDateTime"];
        //return [CPString stringWithFormat:@"%04d-%02d-%02d %02d:%02d:%02d %s", self.getFullYear(), self.getMonth()+1, self.getDate(), self.getHours(), self.getMinutes(), self.getSeconds(), [CPDate timezoneOffsetString:self.getTimezoneOffset()]];
        return [[CPDate dateWithTimeIntervalSince1970:jsonData[row].created ] description];
      }else{
        return ""
      }
      //return [CPString stringWithFormat:@"%s", jsonData.data[row].begintime ];
    }else if([tableColumn identifier]==3){
      if(jsonData[row].completed>1){
        return [CPDate dateWithTimeIntervalSince1970:jsonData[row].completed ];
      }else{
        return "";
      }
      //return [CPString stringWithFormat:@"%s", jsonData.data[row].endtime ];
      //return [[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime]];
    }else if([tableColumn identifier]==4){
      if(jsonData[row].endtime.length>1){
        return [self formatDuration:[[CPDate dateWithTimeIntervalSince1970:jsonData[row].endtime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData[row].begintime]]];
      }else{
        return "";
      }
    }else if([tableColumn identifier]==5){
      if(jsonData[row].status=="Processing"){
        return [CPString stringWithFormat:@"%s", jsonData[row].progress ];
      }else
        return jsonData[row].status;
    }else if([tableColumn identifier]==6){
      if(jsonData[row].fps)
        return [CPString stringWithFormat:@"%s", jsonData[row].fps ];
      else return "";
    }else if([tableColumn identifier]==7){
      return [CPString stringWithFormat:@"%s", jsonData[row].status ];
    }else if([tableColumn identifier]==8){
      return jsonData[row];
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
    for(var a=0;a<jsonData.length;a++){
      if(jsonData[a].id==selectedid)
      [jobTableView selectRowIndexes:[CPIndexSet indexSetWithIndex:a] byExtendingSelection:NO];
    }
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

- (void)tableViewSelectionDidChange:(CPNotification)aNotification{
  if(jsonData[[[aNotification object] selectedRow]]){
    selectedROwIndex=[[aNotification object] selectedRow];
    CPLog.debug("hello new activity selected:"+jsonData[[[aNotification object] selectedRow]].uuid);
    selectedid=jsonData[[[aNotification object] selectedRow]].uuid;
    [activityWebView setData:jsonData[[[aNotification object] selectedRow]]];
    [[CPNotificationCenter defaultCenter]
     postNotificationName:EncodingClicked
     object:self
     userInfo:selectedid];
  }
}

- (CPMenu)tableView:(CPTableView)aTableView menuForTableColumn:(CPTableColumn)aColumn row:(int)aRow
{
// menu should open/close
// comment
// tag
    var menu = [[CPMenu alloc] initWithTitle:"Graph Menu"],
    //menuItems = ["New","Submit","Rename ...", "Delete"],
    //menuActions = [@selector(newGraph:),@selector(submit:),@selector(rename:), @selector(delete:)],
    menuItems = ["Stop this Flow","Restart this Flow"],
    menuActions = [@selector(stop:),@selector(restart:)],
    numberOfSelectedIssues = [[aTableView selectedRowIndexes] count],
    count = menuItems.length,
    i = 0;

    // if we have more than one issue selected and the user right clicks
    // on a different issue we should just select the issue he right clicked
    if (![[aTableView selectedRowIndexes] containsIndex:aRow])
        [self selectGraphAtIndex:aRow];

    // this might have just changed... recalculate
    numberOfSelectedIssues = [[aTableView selectedRowIndexes] count];

    for (; i < count; i++)
    {
        var title = menuItems[i],
        newMenuItem = [[CPMenuItem alloc] initWithTitle:title action:menuActions[i] keyEquivalent:nil];
        [newMenuItem setTarget:self];
        if(i==0){
          //[newMenuItem setEnabled:jsonData[aRow].statuscode==1||jsonData.data[aRow].statuscode==0];
        }else{
          //[newMenuItem setEnabled:jsonData.data[aRow].statuscode!=1];
        }
          
        //break;
        // we want a seperator so just skip it for now
    	[menu addItem:newMenuItem];
    }

    // add the seperator and the last item
    //[menu addItem:[CPMenuItem separatorItem]];
    //[menu addItem:newMenuItem];

    return menu;
}
- (void)selectGraphAtIndex:(unsigned)index
{
    var indexSet = index < 0 ? [CPIndexSet indexSet] : [CPIndexSet indexSetWithIndex:index];

    if (index >= 0)
        [jobTableView scrollRowToVisible:index];

    [jobTableView selectRowIndexes:indexSet byExtendingSelection:NO];
    //[self tableViewSelectionDidChange:nil];
}

- (void)restart:(id)sender
{
  var result=[[MHiveApiController sharedController] restartEncoding:selectedid];
  //var request = [CPURLRequest requestWithURL:"/api/v1/encoding/"+selectedid+"/restart"];
  //[request setHTTPMethod:@"POST"];
  //var raw_data = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  //CPLog.debug("restart raw_data:"+[raw_data rawString]);
  //var d=[raw_data JSONObject].data;
  if(result.response.status=="ok"){
    growl=[TNGrowlCenter defaultCenter];
    //CPLog.debug("GrowlCenter"+growl);
    [growl pushNotificationWithTitle:@"Sucess" message:@"Graph sucessful restarted"];
  }else{
    var alert=[CPAlert alertWithError:result.response.message];
    [alert runModal];
  }
}
- (void)stop:(id)sender
{
  /*asking for a name*/
  stopgraph=[[CPAlert alloc] init];// initWithTitle:@"Delete Graph" andText:@"are you sure to delete the graph:"+[self selectedId]];
  [stopgraph setTitle:"Are You Sure?"];
  [stopgraph setMessageText:"Are you sure you want to stop the Graph with ID :" + [CPString stringWithFormat:@"%s (%s)", jsonData.data[selectedROwIndex].graphname, jsonData.data[selectedROwIndex].infile]+ "?"];
  [stopgraph setAlertStyle:CPWarningAlertStyle];
  [stopgraph addButtonWithTitle:"Cancel"];
  [stopgraph setDelegate:self];
  [stopgraph addButtonWithTitle:"Stop Graph"];
  [stopgraph runModal];
}

-(void)_stop:(id)sender
{
  var result=[[MHiveApiController sharedController] stopEncoding:selectedid];
  //var request = [CPURLRequest requestWithURL:"/api/v1/encoding/"+selectedid+"/stop"];
  //[request setHTTPMethod:@"POST"];
  //var raw_data = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  //CPLog.debug("stop raw_data:"+[raw_data rawString]);
  //var d=[raw_data JSONObject].data;
  if(result.response.status=="ok"){
    growl=[TNGrowlCenter defaultCenter];
    [growl pushNotificationWithTitle:@"Sucess" message:@"Graph sucessful stopped"];
  }else{
    var alert=[CPAlert alertWithError:result.response.message];
    [alert runModal];
  }
}
- (void)alertDidEnd:(CPAlert)anAlert returnCode:(int)tag
{
  if (tag === 1){
    [self _stop:nil];
  }
  [CPTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(refresh) userInfo:nil repeats:NO];
}
@end
