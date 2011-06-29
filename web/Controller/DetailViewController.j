@import <Foundation/CPObject.j>

//@import "../Views/PhotosView.j"


@implementation DetailViewController : CPObject
  {
    CPView      contentView;

    CPDictionary    views;
    CPView          currentView;

  }

  - (void)init
  {

    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(viewSelectionDidChange:)
      name:ProfileClicked
      object:nil];
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(viewSelectionDidChange:)
      name:EncodingClicked
      object:nil];
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(viewSelectionDidChange:)
      name:CPOutlineViewSelectionDidChangeNotification
      object:nil];

    views = [CPDictionary dictionary];
    return self;
  }

  - (void)addViewController:(id)viewController
  {
    var view = [viewController setupView:[contentView bounds]];
    [contentView setDocumentView:view];
    [view setHidden:YES];

    [views setObject:view forKey:[view className]];
  }
  - (void)addViewWithName:(id)view name:(CPString)name
  {
    [view setHidden:YES];
    CPLog.debug([view className]+" set hidden");
    [views setObject:view forKey:name];
  }



  - (void)viewSelectionDidChange:(CPNotification)notification
  {
    CPLog.debug("UserInfo:"+[notification userInfo]);
    var masterView = [notification object];
    var view = [views objectForKey:[masterView class]];

    [currentView setHidden:YES];
    if (view){
      [view setHidden:NO];
      currentView = view;
      var url=[masterView class]=="EncodingView"?"encoding":"profile";
      var request = [CPURLRequest requestWithURL:"/api/v1/"+url+"?id="+[notification userInfo]];
      var connection = [CPURLConnection connectionWithRequest:request delegate:self];
    }
  }
  - (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
  {
    CPLog.debug("received json data:"+data);
    response=[data objectFromJSON];
    [currentView setData:response];

  }
  - (void)connection:(CPURLConnection)aConnection didReceiveResponse:(CPHTTPURLResponse)response
  {
    //CPLog.debug(response);
  }

  - (void)connection:(CPURLConnection)aConnection didFailWithError:(CPString)error
  {
    alert(error); //a network error occurred
  }

@end
