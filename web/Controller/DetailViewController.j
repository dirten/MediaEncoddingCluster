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
    CPLog.debug("MasterView:"+[masterView class]);
    var view = [views objectForKey:[masterView class]];

    if (view){
      [currentView setHidden:YES];
      [view setHidden:NO];
      currentView = view;
      var url=[masterView class]=="EncodingView"?"job":"profile";
      var request = [CPURLRequest requestWithURL:"/api/v1/"+url+"?id="+[notification userInfo]];
      var connection = [CPURLConnection connectionWithRequest:request delegate:self];
    }
  }
  - (void)connection:(CPURLConnection)aConnection didReceiveData:(CPString)data
  {
    //CPLog.debug("received json data:"+data);
    response=[data objectFromJSON];
    if(response.begintime&&response.endtime){
      var end=response.endtime==1?(new Date()).getTime()/1000:response.endtime;
      var start=response.begintime==1?end:response.begintime;
      response.duration=[self formatDuration:(end-start)];
      }
    if(response.created&&response.begintime){
      var begin=response.begintime==1?(new Date()).getTime()/1000:response.begintime;
      response.queuedtime=[self formatDuration:(begin-response.created)];
      }
    if(response.created)
      response.created=[[CPDate dateWithTimeIntervalSince1970:response.created] description];
    if(response.begintime)
      response.begintime=[[CPDate dateWithTimeIntervalSince1970:response.begintime] description];
    if(response.endtime)
      response.endtime=[[CPDate dateWithTimeIntervalSince1970:response.endtime] description];
    //if(response.profile)
    //  response.profile=JSON.stringify(response.profile);
    if(response.data)
      response.profilestr=data;//JSON.stringify(response.data);

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
-(id)formatDuration:(id)d
{
  var duration=parseInt(d);
  var metric=" sec"
  var rest=0;
  var result=""
  //if(parseInt(duration/(60*60))>0){
    var hour=parseInt(duration/(60*60));
    rest=parseInt(duration%(60*60));
    result=hour+" h ";
  //}
  //if(parseInt(rest/(60))>0){
    var minutes=parseInt(rest/(60));
    rest=parseInt(duration%(60));
    result+=minutes+" min. ";
  //}
  //if(parseInt(rest/(60))>0){
    var seconds=parseInt(rest);
    result+=seconds+" sec.";
  //}
  if(parseInt(duration/60)>0){
    duration=parseInt(duration/60);
    metric=" min"
  }
  if(parseInt(duration/60)>0){
    duration=parseInt(duration/60);
    metric=" h"
  }
  return ""+result;

}
@end
