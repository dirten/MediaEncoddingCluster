
@import "../../View/ProgressTableDataView.j"
@import "../../View/NodeEditorView.j"
@import "../../Controller/NodeActivityController.j"


@implementation TaskActivityController: CPObject
{
  @outlet CPTableView     taskTableView;
  @outlet NodeEditorView  nodeEditorView;
  @outlet CPView          webView;
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
  [CPTimer scheduledTimerWithTimeInterval:3 target:self selector:@selector(refresh) userInfo:nil repeats:true];
  //[[NodeEditorController alloc] initWithView:theView]
  nodeEditorController=[[NodeActivityController alloc] initWithView:nodeEditorView];
  [nodeEditorView setDelegate:self];
  var bundle      = [CPBundle bundleForClass:[self class]]
  activityWebView=[[[DetailWebView alloc] initWithFrame:[webView bounds]] initWithTemplate:[bundle pathForResource:@"TaskWebView.html"]];
  [webView addSubview:activityWebView];

}
-(void)selectionDidChangeInEditorView:element{
  CPLog.debug("Selection in node view Changed to"+element);
  CPLog.debug("Selection Json:"+JSON.stringify([nodeEditorController data]));
  var elements=[nodeEditorController data].graph.tasks;
  for(var a=0;a<elements.length;a++){
    var task=[nodeEditorController data].graph.tasks[a];
    if(task.uid==[element uid]){
      [activityWebView setData:task];
      break;
    }
  }
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
  jobid = [notification userInfo];
  //var noti=[[CPNotification notificationWithName:@"noname" object:self userInfo:jobid]];
  CPLog.debug("loadNodeEditorView:"+[notification userInfo]);

  [nodeEditorController _load:notification];
  //[nodeEditorController _load:noti];
  //[self refresh];
}

- (void)refresh{
  if(jobid){
    [[CPNotificationCenter defaultCenter]
     postNotificationName:EncodingClicked
     object:self
     userInfo:jobid];

  }
}

@end
