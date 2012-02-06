
@import "../Controller/NodeEditorController.j"

SelectGraphByUUID = @"SelectGraphByUUID";

@implementation GraphListView : CPScrollView
{
  CPTableView  tableView;
  id jsonData;
  TextInput input;
  TextInput delgraph;
  InputWindow newWin;
  id selectUUID;
}

-(id)initWithFrame:(id)frame
{
  self=[super initWithFrame:frame];
  if(self){
    frame.origin.x=0;
    frame.origin.y=0;
    [self setAutohidesScrollers:YES];
    [self setHasHorizontalScroller:NO ];
    [self setHasVerticalScroller:YES]; 
    tableView=[[CPTableView alloc] initWithFrame:frame];
  
    var column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 1]];
    [[column headerView] setStringValue:"Name"];
    [[column headerView] sizeToFit];
    [column setWidth:280];
    [tableView addTableColumn:column];

    [tableView setDataSource:self];
    [tableView setDelegate:self];
    [tableView setTarget:self];
    [tableView setDoubleAction:@selector(doubleClicked)];
    [tableView setUsesAlternatingRowBackgroundColors:YES];
    [self setDocumentView:tableView];
    [self setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
    [self refresh];
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(selectGraphAtUUID:)
      name:SelectGraphByUUID
      object:nil];
  }
  return self;
}

- (int)numberOfRowsInTableView:(CPTableView)tabView
{
  var result=0;
  if(jsonData){
    //jsonData.data.unshift({"id":"double click to create a new Encoding","created":"","begintime":"","endtime":"","progress":"","status":""});
    result=jsonData.data.length;
  }
  CPLog.debug("graphlist RowCount="+result);
  return result;
}

- (id)tableView:(CPTableView)tabView objectValueForTableColumn:(CPTableColumn)tableColumn row:(int)row
{
  //CPLog.debug("load table data");
  if([tableColumn identifier]==1){
    CPLog.debug("item found for row"+row+"="+JSON.stringify(jsonData.data[row]));
    var str="";
    if(jsonData.data[row].name!=undefined&&jsonData.data[row].name.length>0){
      str=jsonData.data[row].name;
    }else{
      str=jsonData.data[row].uuid;    
    }
    if(selectUUID==jsonData.data[row].uuid){
      [self selectGraphAtIndex:row];
      selectUUID="";
    }
    //var str=jsonData.data[row].name.length>0?jsonData.data[row].name:jsonData.data[row].uuid;
    return [CPString stringWithFormat:@"%s", str];
  }
}

- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  CPLog.debug("status==200 : "+[_response statusCode]);
  if([_response statusCode]==200){
    jsonData=[data objectFromJSON];
    [tableView reloadData];
  }else{
    alert(data);
  }
}
- (void)connection:(CPURLConnection)aConnection didReceiveResponse:(CPHTTPURLResponse)response
{
  _response=response;
}

- (void)tableViewSelectionDidChange:(CPNotification)aNotification{
  if([self selectedId]){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:LoadNodeEditorView
      object:self
      userInfo:[self selectedId]];
  }
}

- (void) doubleClicked
{
  if([self selectedId]){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:LoadNodeEditorView
      object:self
      userInfo:[self selectedId]];
  }
}

- (void)refresh{
  var request = [CPURLRequest requestWithURL:"/api/v1/flow"];
  [request setHTTPMethod:"GET"];
  //CPLog.debug(request.HTTPMethod);
  // see important note about CPJSONPConnection above
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
}

-(CPString)selectedId
{
  if([tableView selectedRow]!=-1){
    return jsonData.data[[tableView selectedRow]].uuid;
  }else{
    return nil;
  }
}
-(CPString)selectedName
{
  if([tableView selectedRow]!=-1){
    return jsonData.data[[tableView selectedRow]].name;
  }else{
    return nil;
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
    menuItems = ["Rename ...", "Delete"],
    menuActions = [@selector(rename:), @selector(delete:)],
    numberOfSelectedIssues = [[tableView selectedRowIndexes] count],
    count = menuItems.length,
    i = 0;

    // if we have more than one issue selected and the user right clicks
    // on a different issue we should just select the issue he right clicked
    if (![[tableView selectedRowIndexes] containsIndex:aRow])
        [self selectGraphAtIndex:aRow];

    // this might have just changed... recalculate
    numberOfSelectedIssues = [[tableView selectedRowIndexes] count];

    for (; i < count; i++)
    {
        var title = menuItems[i],
        newMenuItem = [[CPMenuItem alloc] initWithTitle:title action:menuActions[i] keyEquivalent:nil];
        [newMenuItem setTarget:self];
        var st=jsonData.data[[tableView selectedRow]].status;
        //[newMenuItem setEnabled:parseInt(st)==st];
        //break;
        // we want a seperator so just skip it for now
    	[menu addItem:newMenuItem];
    }

    // add the seperator and the last item
    //[menu addItem:[CPMenuItem separatorItem]];
    //[menu addItem:newMenuItem];

    return menu;
}

- (void)selectGraphAtUUID:(id)notification
{
  var uuid=[notification userInfo];
  selectUUID=uuid;
  for(a=0;a<jsonData.data.length;a++){
    CPLog.debug("data.uuid="+jsonData.data[a].uuid+" uuid="+uuid);
    if(jsonData.data[a].uuid==uuid){
      [self selectGraphAtIndex:a];
      break;
    }
  }
}

- (void)selectGraphAtIndex:(unsigned)index
{
    var indexSet = index < 0 ? [CPIndexSet indexSet] : [CPIndexSet indexSetWithIndex:index];

    if (index >= 0)
        [tableView scrollRowToVisible:index];

    [tableView selectRowIndexes:indexSet byExtendingSelection:NO];
    //[self tableViewSelectionDidChange:nil];
}

- (void)newGraph:(id)sender
{
  CPLog.debug("New Encoding Graph:");
  /*asking for a name*/
  newWin=[[InputWindow alloc] initWithTitle:@"Name for the new Graph" andText:@"please enter a Name for the new Graph." forValue:nil];
  [newWin setDelegate:self];

}
- (void)_newGraph:(CPString)name
{
  CPLog.debug("New Encoding Graph execute:");
  //[self refresh];
  [[CPNotificationCenter defaultCenter]
    postNotificationName:NewNodeEditorView
    object:name
    userInfo:nil];
}

- (void)submit:(id)sender
{

}
- (void)delete:(id)sender
{
  /*asking for a name*/
  delgraph=[[CPAlert alloc] init];// initWithTitle:@"Delete Graph" andText:@"are you sure to delete the graph:"+[self selectedId]];
  [delgraph setTitle:"Are You Sure?"];
  [delgraph setMessageText:"Are you sure you want to delete the Graph with ID :" + [self selectedName] + "?"];
  [delgraph setAlertStyle:CPWarningAlertStyle];
  [delgraph addButtonWithTitle:"Cancel"];
  [delgraph setDelegate:self];
  [delgraph addButtonWithTitle:"Delete Graph"];
  [delgraph runModal];

}
- (void)rename:(id)sender
{
  /*asking for a name*/
  input=[[InputWindow alloc] initWithTitle:@"Name for the Graph" andText:@"please enter a new Name for the Graph." forValue:[self selectedName]];
  [input setDelegate:self];  
}

- (void)_rename:(CPString)name
{
  CPLog.debug("Rename Encoding Graph:");
  
  [[CPNotificationCenter defaultCenter]
    postNotificationName:RenameNodeEditorView
    object:name
    userInfo:[self selectedId]];
}
- (void)_delete:(CPString)uuid
{
  CPLog.debug("Rename Encoding Graph:");
  [[CPNotificationCenter defaultCenter]
    postNotificationName:DeleteNodeEditorView
    object:uuid
    userInfo:[self selectedId]];
  [self selectGraphAtIndex:-1];
}

- (void)alertDidEnd:(CPAlert)anAlert returnCode:(int)tag
{
  var name=[input inputText];
  if (tag === 1){
    if(anAlert==input)
      [self _rename:name];
    if(anAlert==newWin){
      name=[newWin inputText];
      [self _newGraph:name];
    }
    if(anAlert==delgraph)
      [self _delete:[self selectedId]];
    }
  [CPTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(refresh) userInfo:nil repeats:NO];
}
@end