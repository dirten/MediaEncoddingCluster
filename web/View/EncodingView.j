
@import "ProgressTableDataView.j"
@import "ActionsTableDataView.j"
EncodingDoubleClicked = @"EncodingDoubleClicked";
EncodingClicked = @"EncodingClicked";
@implementation EncodingView :CPScrollView
  {
    id jsonData;
    id selectedid;
    CPTableView  tableView;
    CPAlert stopWarn;
    CPAlert deleteWarn;
  }

  -(void)setData:(id)data
  {
    jsonData=data;
  }

  -(id)initWithFrame:(id)frame
  {
    self=[super initWithFrame:frame];

    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(refresh)
      name:EncodingChanged
      object:nil];

    tableView=[[CPTableView alloc] initWithFrame:frame];
    var idcolumn = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 1]];
    [[idcolumn headerView] setStringValue:"Id"];
    [[idcolumn headerView] sizeToFit];
    [idcolumn setWidth:280];
    desc = [CPSortDescriptor sortDescriptorWithKey:@"title" ascending:YES];
    [idcolumn setSortDescriptorPrototype:desc];

    [tableView addTableColumn:idcolumn];
    var namecolumn = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 2]];
    [[namecolumn headerView] setStringValue:"Started"];
    [[namecolumn headerView] sizeToFit];
    [namecolumn setWidth:120];
    [tableView addTableColumn:namecolumn];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 3]];
    [[column headerView] setStringValue:"Completed"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 4]];
    [[column headerView] setStringValue:"Duration"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];

    var progressDataView=[ProgressTableDataView new];
    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 5]];
    [[column headerView] setStringValue:"Progress"];
    [[column headerView] sizeToFit];
    [column setDataView:progressDataView];
    [column setWidth:120];
    [tableView addTableColumn:column];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 6]];
    [[column headerView] setStringValue:"Frames/sec"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 7]];
    [[column headerView] setStringValue:"Status"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];


    [tableView setDataSource:self];
    [tableView setDelegate:self];
    [tableView setTarget:self];
    [tableView setDoubleAction:@selector(doubleClicked)];
    [tableView setUsesAlternatingRowBackgroundColors:YES];
    [self setDocumentView:tableView];

    [self setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
    /*
    var request = [CPURLRequest requestWithURL:"/api/v1/encoding?full"];
    [request setHTTPMethod:"GET"];
    CPLog.debug(request.HTTPMethod);
    // see important note about CPJSONPConnection above
    var connection = [CPURLConnection connectionWithRequest:request delegate:self];
    */
    [tableView setAllowsMultipleSelection:YES];
    [self refresh];
    [CPTimer scheduledTimerWithTimeInterval:10 target:self selector:@selector(refresh) userInfo:nil repeats:true];
    return self;
  }
  -(void)setHidden:(BOOL)flag{

    [super setHidden:flag];

  }
  - (int)numberOfRowsInTableView:(CPTableView)tabView
  {
    var result=0;
    if(jsonData){
      jsonData.data.unshift({"id":"double click to create a new Encoding","created":"","begintime":"","endtime":"","progress":"","status":""});
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
      return [CPString stringWithFormat:@"%s", jsonData.data[row].id ];
    }else
    if([tableColumn identifier]==2){
      if(jsonData.data[row].begintime>1){
        return [CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime ];
      }else{
      return ""
    }
    //return [CPString stringWithFormat:@"%s", jsonData.data[row].begintime ];
  }else
  if([tableColumn identifier]==3){
    if(jsonData.data[row].endtime>1){
      return [CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime ];
    }else{
    return "";
  }
  //return [CPString stringWithFormat:@"%s", jsonData.data[row].endtime ];
  //return [[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime]];
}else
if([tableColumn identifier]==4){
  if(jsonData.data[row].endtime.length>1){
    return [self formatDuration:[[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].endtime] timeIntervalSinceDate:[CPDate dateWithTimeIntervalSince1970:jsonData.data[row].begintime]]];
  }else{
  return "";
}
}else
if([tableColumn identifier]==5){
  return [CPString stringWithFormat:@"%s", jsonData.data[row].progress ];
}else
if([tableColumn identifier]==6){
    if(jsonData.data[row].fps)
	return [CPString stringWithFormat:@"%s", jsonData.data[row].fps ];
    else return "";
}else
if([tableColumn identifier]==7){
  return [CPString stringWithFormat:@"%s", jsonData.data[row].status ];
}else
if([tableColumn identifier]==8){
  return jsonData.data[row];
}
}
}
-(id)formatDuration:(id)d
{
  var duration=parseInt(d);
  var metric=" sec"
  var rest=0;
  var result=""
  //if(parseInt(duration/(60*60))>0){
    var hour=parseInt(duration/(60*60));
    rest=parseInt(duration%(60*60));
    result=hour+":";
  //}
  //if(parseInt(rest/(60))>0){
    var minutes=parseInt(rest/(60));
    rest=parseInt(duration%(60));
    result+=minutes+":";
  //}
  //if(parseInt(rest/(60))>0){
    var seconds=parseInt(rest);
    result+=seconds+"";
  //}
  if(parseInt(duration/60)>0){
    duration=parseInt(duration/60);
    metric=" min"
  }
  if(parseInt(duration/60)>0){
    duration=parseInt(duration/60);
    metric=" h"
  }
  return result;

}

- (void)tableView:(CPTableView)aTableView sortDescriptorsDidChange:(CPArray)oldDescriptors
{
  var newDescriptors = [aTableView sortDescriptors];

  [aTableView reloadData];


}
- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  //this method is called when the network request returns. the data is the returned
  //information from flickr. we set the array of photo urls as the data to our collection view


  jsonData=[data objectFromJSON];
  //CPLog.debug("json="+jsonData.requestId);
  [tableView reloadData];
}
- (void)connection:(CPURLConnection)aConnection didReceiveResponse:(CPHTTPURLResponse)response
{
  //this method is called when the network request returns. the data is the returned
  //information from flickr. we set the array of photo urls as the data to our collection view

  //[self addImageList:data.photos.photo withIdentifier:lastIdentifier];
  //CPLog.debug(response);
}

- (void)connection:(CPURLConnection)aConnection didFailWithError:(CPString)error
{
  alert(error); //a network error occurred
}
- (void)tableViewSelectionDidChange:(CPNotification)aNotification{
  if(jsonData.data[[[aNotification object] selectedRow]]){
  CPLog.debug("hello:"+jsonData.data[[[aNotification object] selectedRow]].id);
  selectedid=jsonData.data[[[aNotification object] selectedRow]].id;
  [[CPNotificationCenter defaultCenter]
    postNotificationName:EncodingClicked
    object:self
    userInfo:selectedid];
  }
}

- (void) doubleClicked{
  CPLog.debug("double clicked"+selectedid);
  [[CPNotificationCenter defaultCenter]
    postNotificationName:EncodingDoubleClicked
    object:self
    userInfo:selectedid];
}
}
- (void)refresh{
  var request = [CPURLRequest requestWithURL:"/api/v1/encoding?full"];
  [request setHTTPMethod:"GET"];
  //CPLog.debug(request.HTTPMethod);
  // see important note about CPJSONPConnection above
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
}

- (CPMenu)tableView:(CPTableView)aTableView menuForTableColumn:(CPTableColumn)aColumn row:(int)aRow
{
// menu should open/close
// comment
// tag
    var menu = [[CPMenu alloc] initWithTitle:"Encoding Menu"],
    menuItems = ["Stop Encoding", "Delete Encoding"],
    menuActions = [@selector(stop:), @selector(delete:)],
    //isOpen = displayedIssuesKey === "openIssues",
    numberOfSelectedIssues = [[tableView selectedRowIndexes] count],
    count = menuItems.length,
    i = 0;

    // if we have more than one issue selected and the user right clicks
    // on a different issue we should just select the issue he right clicked
    if (![[tableView selectedRowIndexes] containsIndex:aRow])
        [self selectEncodingAtIndex:aRow];

    // this might have just changed... recalculate
    numberOfSelectedIssues = [[tableView selectedRowIndexes] count];

    for (; i < count; i++)
    {
        var title = menuItems[i],
        newMenuItem = [[CPMenuItem alloc] initWithTitle:title action:menuActions[i] keyEquivalent:nil];

        [newMenuItem setTarget:self];

        switch (title)
        {
            case "Stop Encoding":
                if (numberOfSelectedIssues > 1)
                    [newMenuItem setTitle:"Stop (" + numberOfSelectedIssues + ") Encodings" ];

                //[newMenuItem setEnabled:!isOpen];
                break;

	    case "Delete Encoding":
		if (numberOfSelectedIssues > 1)
		    [newMenuItem setTitle:"Delete (" + numberOfSelectedIssues + ") Encodings"];


		//[newMenuItem setEnabled:isOpen];
		break;

	    case "Tag":
		var shouldTag = numberOfSelectedIssues === 1;

		//[newMenuItem setEnabled:shouldTag];

		//if (shouldTag)
		    //[newMenuItem setSubmenu:[self _tagsMenu]];
		break;
	    case "Comment":
		[newMenuItem setEnabled:(numberOfSelectedIssues === 1)];
		break;
	    // we want a seperator so just skip it for now
	    case "View On GitHub":
		[newMenuItem setEnabled:(numberOfSelectedIssues === 1 || aRow !== CPNotFound)];
		continue;
		break;
	}

	if (aRow === CPNotFound)
	    [newMenuItem setEnabled:NO];

	[menu addItem:newMenuItem];
    }

    // add the seperator and the last item
    //[menu addItem:[CPMenuItem separatorItem]];
    [menu addItem:newMenuItem];

    return menu;
}
- (void)selectEncodingAtIndex:(unsigned)index
{
    var indexSet = index < 0 ? [CPIndexSet indexSet] : [CPIndexSet indexSetWithIndex:index];

    if (index >= 0)
        [tableView scrollRowToVisible:index];

    [tableView selectRowIndexes:indexSet byExtendingSelection:NO];
    //[self tableViewSelectionDidChange:nil];
}

- (void)alertDidEnd:(CPAlert)anAlert returnCode:(int)tag{
    if (anAlert === stopWarn && tag === 1)
            [self _stop];
        else if (anAlert === deleteWarn && tag === 1)
            [self _delete];
                                                                }
- (void)stop:(id)sender{
    //CPLog.debug("stop:"+_value.id)
    var issues = [tableView selectedRowIndexes],
         count = [issues count];
         
     if (count > 1)
     {
	 stopWarn = [[CPAlert alloc] init];
         [stopWarn setTitle:"Are You Sure?"];
         [stopWarn setMessageText:"Are you sure you want to stop " + count + " Encodings?"];
         [stopWarn setAlertStyle:CPWarningAlertStyle];
         [stopWarn addButtonWithTitle:"Cancel"];
         [stopWarn setDelegate:self];
         [stopWarn addButtonWithTitle:"Stop Encodings"];
         [stopWarn runModal];
     }
     else
        [self _stop];
}
- (void)delete:(id)sender{
    var issues = [tableView selectedRowIndexes],
         count = [issues count];
         
     if (count > 1)
     {
	 deleteWarn = [[CPAlert alloc] init];
         [deleteWarn setTitle:"Are You Sure?"];
         [deleteWarn setMessageText:"Are you sure you want to delete " + count + " Encodings?"];
         [deleteWarn setAlertStyle:CPWarningAlertStyle];
         [deleteWarn addButtonWithTitle:"Cancel"];
         [deleteWarn setDelegate:self];
         [deleteWarn addButtonWithTitle:"Delete Encodings"];
         [deleteWarn runModal];
     }
     else
        [self _delete];
}
- (void)_stop{

    //items = [tableView objectsAtIndexes:rows];
    var encodings = [tableView selectedRowIndexes],
    count = [encodings count];
    [tableView selectRowIndexes:[CPIndexSet indexSet] byExtendingSelection:NO];
    var iarray=[CPArray array];
    [encodings getIndexes:iarray maxCount:count inIndexRange:nil];
    while (count--){
        //var issue = encodings[count];
        CPLog.debug("stop encoding:"+jsonData.data[iarray[count]].id);


	var url="/api/v1/encoding";
	if(jsonData.data[iarray[count]].id)
	    url+="?id="+jsonData.data[iarray[count]].id;
	else
	    continue;
	url+="&stop";
	var request = [CPURLRequest requestWithURL:url];
	var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
	CPLog.debug([result rawString]);
	resultData=[[result rawString] objectFromJSON];
	if(resultData.ok){
	    //[self close];
	    //var alert=[CPAlert alertWithMessageText:@"Encoding Stopped" defaultButton:@"Ok" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Encoding successfull Stopped"];
	    //[alert setAlertStyle:CPInformationalAlertStyle];
	    //[alert runModal];
	}else{
	    var alert=[CPAlert alertWithError:resultData.error.description];
	    [alert runModal];
	}
    }
    [[CPNotificationCenter defaultCenter]
    	    postNotificationName:EncodingChanged
    	    object:self
    	    userInfo:nil];

}

- (void)_delete{



    //items = [tableView objectsAtIndexes:rows];
    var encodings = [tableView selectedRowIndexes],
    count = [encodings count];
    [tableView selectRowIndexes:[CPIndexSet indexSet] byExtendingSelection:NO];
    var iarray=[CPArray array];
    [encodings getIndexes:iarray maxCount:count inIndexRange:nil];
    while (count--){
        //var issue = encodings[count];
        CPLog.debug("stop encoding:"+jsonData.data[iarray[count]].id);


	var url="/api/v1/encoding";
	if(jsonData.data[iarray[count]].id)
	    url+="?id="+jsonData.data[iarray[count]].id;
	else
	    continue;
	url+="&delete";
	var request = [CPURLRequest requestWithURL:url];
	var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
	CPLog.debug([result rawString]);
	resultData=[[result rawString] objectFromJSON];
	if(resultData.ok){
	    //[self close];
	    //var alert=[CPAlert alertWithMessageText:@"Encoding Stopped" defaultButton:@"Ok" alternateButton:nil otherButton:nil informativeTextWithFormat:@"Encoding successfull Stopped"];
	    //[alert setAlertStyle:CPInformationalAlertStyle];
	    //[alert runModal];
	}else{
	    var alert=[CPAlert alertWithError:resultData.error.description];
	    [alert runModal];
	}
    }
    [[CPNotificationCenter defaultCenter]
    	    postNotificationName:EncodingChanged
    	    object:self
    	    userInfo:nil];

}
@end