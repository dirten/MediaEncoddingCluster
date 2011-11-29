

@implementation GraphListView : CPScrollView
{
  CPTableView  tableView;
  id jsonData;
  TextInput input;
  TextInput delgraph;
}

-(id)initWithFrame:(id)frame
{
  self=[super initWithFrame:frame];
  if(self){
    frame.origin.x=0;
    frame.origin.y=0;

    tableView=[[CPTableView alloc] initWithFrame:frame];
  
    var column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 1]];
    [[column headerView] setStringValue:"Id"];
    [[column headerView] sizeToFit];
    [column setWidth:280];
    [tableView addTableColumn:column];

    [tableView setDataSource:self];
    [tableView setDelegate:self];
    [tableView setTarget:self];
    [tableView setDoubleAction:@selector(doubleClicked)];

    [self setDocumentView:tableView];
    [self setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
    [self refresh];
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
  CPLog.debug("RowCount="+result);
  return result;
}

- (id)tableView:(CPTableView)tabView objectValueForTableColumn:(CPTableColumn)tableColumn row:(int)row
{
  //CPLog.debug("load table data");
  if([tableColumn identifier]==1){
    CPLog.debug("item found for row"+row+"="+JSON.stringify(jsonData.data[row]));
    var str=jsonData.data[row].name.length>0?jsonData.data[row].name:jsonData.data[row].uuid;
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
  var request = [CPURLRequest requestWithURL:"/api/v1/graph"];
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
- (CPMenu)tableView:(CPTableView)aTableView menuForTableColumn:(CPTableColumn)aColumn row:(int)aRow
{
// menu should open/close
// comment
// tag
    var menu = [[CPMenu alloc] initWithTitle:"Graph Menu"],
    menuItems = ["Rename ...", "Delete"],
    menuActions = [@selector(rename:), @selector(delete:)],
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
        //[newMenuItem setEnabled:(numberOfSelectedIssues === 1)];
        //break;
        // we want a seperator so just skip it for now
    	[menu addItem:newMenuItem];
    }

    // add the seperator and the last item
    //[menu addItem:[CPMenuItem separatorItem]];
    //[menu addItem:newMenuItem];

    return menu;
}

- (void)delete:(id)sender
{
  /*asking for a name*/
  delgraph=[[InputWindow alloc] initWithTitle:@"Delete Graph" andText:@"are you sure to delete the graph:"+[self selectedId]];
  [delgraph setDelegate:self];  

}
- (void)rename:(id)sender
{
  /*asking for a name*/
  input=[[InputWindow alloc] initWithTitle:@"Name for the Graph" andText:@"please enter a new Name for the Graph."];
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
}

- (void)alertDidEnd:(CPAlert)anAlert returnCode:(int)tag
{
  var name=[input inputText];
  if (tag === 1){
    if(anAlert==input)
      [self _rename:name];
    if(anAlert==delgraph)
      [self _delete:[self selectedId]];
    }
  [CPTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(refresh) userInfo:nil repeats:NO];
}
@end