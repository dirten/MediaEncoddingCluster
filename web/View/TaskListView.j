@implementation TaskListView :CPScrollView
  {
    id _response
    id jsonData;
    id selectedid;
    CPTableView  tableView;
    CPAlert stopWarn;
    CPAlert deleteWarn;
  }

-(id)initWithFrame:(CGRect)frame
{
    self=[super initWithFrame:frame];
    tableView=[[CPTableView alloc] initWithFrame:frame];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 1]];
    [[column headerView] setStringValue:"Id"];
    [[column headerView] sizeToFit];
    [column setWidth:280];
    [tableView addTableColumn:column];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 2]];
    [[column headerView] setStringValue:"Name"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 3]];
    [[column headerView] setStringValue:"Progress"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 4]];
    [[column headerView] setStringValue:"Status"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];

    column = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 5]];
    [[column headerView] setStringValue:"Message"];
    [[column headerView] sizeToFit];
    [column setWidth:120];
    [tableView addTableColumn:column];

    [tableView setDataSource:self];
    [tableView setDelegate:self];

    [self setDocumentView:tableView];
    [CPTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(refresh) userInfo:nil repeats:true];

  return self;

}

-(void)setData:(id)data
  {
    CPLog.debug("setting data"+data);
    jsonData=data;
    [tableView reloadData];

}

- (int)numberOfRowsInTableView:(CPTableView)tabView
  {
    var result=0;
    if(jsonData){
      result=jsonData.data[0].tasks.length;
    }
    CPLog.debug("RowCount="+result);
    return result;
  }

- (id)tableView:(CPTableView)tabView objectValueForTableColumn:(CPTableColumn)tableColumn row:(int)row
  {
    if([tableColumn identifier]==1){
      return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].uuid ];
    }else
    if([tableColumn identifier]==2){
      return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].name ];
    }else
    if([tableColumn identifier]==3){
      return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].progress ];
    }else
    if([tableColumn identifier]==4){
      return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].statustext ];
    }else
    if([tableColumn identifier]==5){
      return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].statusmessage ];
    }

}

- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  CPLog.debug("status==200 : "+[_response statusCode]==200);

  if([_response statusCode]==200){
    jsonData=[data objectFromJSON];
    [tableView reloadData];
  }else{
    alert(data);
  }
}

- (void)refresh{
  if(jsonData&&jsonData.data[0].uuid){
    var request = [CPURLRequest requestWithURL:"/api/v1/job?id="+jsonData.data[0].uuid];
    [request setHTTPMethod:"GET"];
    var connection = [CPURLConnection connectionWithRequest:request delegate:self];
  }
}

@end