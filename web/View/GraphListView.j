

@implementation GraphListView : CPScrollView
{
  CPTableView  tableView;
  id jsonData;
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
  CPLog.debug("load table data");
  if([tableColumn identifier]==1){
    CPLog.debug("item found for row"+row+"="+jsonData.data[row].uuid);
    return [CPString stringWithFormat:@"%s", jsonData.data[row].uuid ];
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

@end