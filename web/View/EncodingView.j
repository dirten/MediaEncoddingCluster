EncodingDoubleClicked = @"EncodingDoubleClicked";
@implementation EncodingView :CPScrollView
  {
    id jsonData;
    id selectedid;
    CPTableView  tableView;
  }

  -(void)setData:(id)data
  {
    jsonData=data;
  }

  -(id)initWithFrame:(id)frame
  {
    self=[super initWithFrame:frame];
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
    [[column headerView] setStringValue:"Progress"];
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
    var request = [CPURLRequest requestWithURL:"/api/v1/encoding?full"];
    [request setHTTPMethod:"GET"];
    CPLog.debug(request.HTTPMethod);
    // see important note about CPJSONPConnection above
    var connection = [CPURLConnection connectionWithRequest:request delegate:self];

    return self;
  }
  -(void)setHidden:(BOOL)flag{

  [super setHidden:flag];

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
      return [CPString stringWithFormat:@"%s", jsonData.data[row].begintime ];
    }else
    if([tableColumn identifier]==3){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].endtime ];
    }else
    if([tableColumn identifier]==4){
      return [CPString stringWithFormat:@"%s", jsonData.data[row].progress ];
    }
  }
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
  CPLog.debug("json="+jsonData.requestId);
  [tableView reloadData];
}
- (void)connection:(CPURLConnection)aConnection didReceiveResponse:(CPHTTPURLResponse)response
{
  //this method is called when the network request returns. the data is the returned
  //information from flickr. we set the array of photo urls as the data to our collection view

  //[self addImageList:data.photos.photo withIdentifier:lastIdentifier];
  CPLog.debug(response);
}

- (void)connection:(CPURLConnection)aConnection didFailWithError:(CPString)error
{
  alert(error); //a network error occurred
}
- (void)tableViewSelectionDidChange:(CPNotification)aNotification{
    CPLog.debug("hello:"+jsonData.data[[[aNotification object] selectedRow]].id);
    selectedid=jsonData.data[[[aNotification object] selectedRow]].id;
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
    CPLog.debug(request.HTTPMethod);
    // see important note about CPJSONPConnection above
    var connection = [CPURLConnection connectionWithRequest:request delegate:self];
}

@end