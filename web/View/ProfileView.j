@import "/Controller/ProfileEditViewController.j"
ProfileViewRefresh = @"ProfileViewRefresh";

ProfileDoubleClicked = @"ProfileDoubleClicked";
ProfileClicked = @"ProfileClicked";

@implementation ProfileView :CPScrollView
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
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(refresh)
      name:ProfileChanged
      object:nil];
    

    self=[super initWithFrame:frame];
    tableView=[[CPTableView alloc] initWithFrame:frame];
    var idcolumn = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 1]];
    [[idcolumn headerView] setStringValue:"Id"];
    [[idcolumn headerView] sizeToFit];
    [idcolumn setWidth:280];
    [tableView addTableColumn:idcolumn];
    var namecolumn = [[CPTableColumn alloc] initWithIdentifier:[CPString stringWithFormat:@"%d", 2]];
    [[namecolumn headerView] setStringValue:"Profile Name"];
    [[namecolumn headerView] sizeToFit];
    [namecolumn setWidth:400];
    [tableView addTableColumn:namecolumn];

    [tableView setDataSource:self];
    [tableView setDelegate:self];
    [tableView setTarget:self];
    [tableView setDoubleAction:@selector(doubleClicked)];
    [tableView setUsesAlternatingRowBackgroundColors:YES];
    [self setDocumentView:tableView];

    [self setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
    var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
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
      return [CPString stringWithFormat:@"%s", jsonData.data[row].name ];
    }
  }
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
  [[CPNotificationCenter defaultCenter]
    postNotificationName:ProfileClicked
    object:self
    userInfo:selectedid];
}

- (void) doubleClicked{
  /*
  CPLog.debug("double clicked"+selectedid);
  var profilePanel =[[ProfileEditView alloc] init];
  [profilePanel setProfileId:selectedid];
  CPLog.debug(selected.test);
*/

[[CPNotificationCenter defaultCenter]
    postNotificationName:ProfileDoubleClicked
    object:self
    userInfo:selectedid];
}
- (void)refresh{
  var request = [CPURLRequest requestWithURL:"/api/v1/profile"];
  [request setHTTPMethod:"GET"];
  CPLog.debug(request.HTTPMethod);
  // see important note about CPJSONPConnection above
  var connection = [CPURLConnection connectionWithRequest:request delegate:self];
}
@end