
@import "../../View/ProgressTableDataView.j"
@import "../../View/NodeEditorView.j"
@import "../../Controller/NodeActivityController.j"


@implementation TaskActivityController: CPObject
{
  @outlet CPTableView     taskTableView;
  @outlet NodeEditorView  nodeEditorView;
  id jsonData;
  CPString jobid;
  NodeEditorController nodeEditorController;
  
}
-(void)awakeFromCib{
  [[CPNotificationCenter defaultCenter]
   addObserver:self
   selector:@selector(loadTasksOnNotification:)
   name:EncodingClicked
   object:nil];
  [nodeEditorView setBackgroundColor:[CPColor whiteColor]];

  var progressDataView=[ProgressTableDataView new];
  var column = [taskTableView tableColumnWithIdentifier:@"3"];
  [column setDataView:progressDataView];
  [self refresh];
  [CPTimer scheduledTimerWithTimeInterval:10 target:self selector:@selector(refresh) userInfo:nil repeats:true];
  //[[NodeEditorController alloc] initWithView:theView]
  nodeEditorController=[[NodeActivityController alloc] initWithView:nodeEditorView];

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
  }else if([tableColumn identifier]==2){
    return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].name ];
  }else if([tableColumn identifier]==3){
    if(jsonData.data[0].tasks[row].statustext=="Processing"){
      return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].progress ];
    }else
      return jsonData.data[0].tasks[row].statustext;
  }else if([tableColumn identifier]==4){
    return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].statustext ];
  }else if([tableColumn identifier]==5){
    return [CPString stringWithFormat:@"%s", jsonData.data[0].tasks[row].statusmessage ];
  }
}

- (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
{
  if([_response statusCode]==200){
    jsonData=[data objectFromJSON];
    [taskTableView reloadData];
  }else{
    alert(data);
  }
}
- (void)loadTasksOnNotification:(CPNotification)notification
{
  //jobid = [notification userInfo];
  //var not=[[CPNotification notificationWithName:@"noname" object:self userInfo:jobid]];
  CPLog.debug("loadNodeEditorView:"+[notification userInfo]);

  [nodeEditorController _load:notification];
  //[self refresh];
}

- (void)refresh{
  if(jobid){
    var request = [CPURLRequest requestWithURL:"/api/v1/job?id="+jobid];
    [request setHTTPMethod:"GET"];
    var connection = [CPURLConnection connectionWithRequest:request delegate:self];
  }
}

@end
