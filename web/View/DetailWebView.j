@import "../Resources/js/mustache.js"

@implementation DetailWebView : CPWebView
  {
    id IssuesHTMLTemplate;
    CPDictionary _data;
    CGRect scrollRect;
    id _id;
  }
  -(void)initWithTemplate:(CPString)template
  {
    var request = new CFHTTPRequest();
    request.open("GET", "Resources/"+template+".html", true);

    request.onsuccess = function()
    {
      CPLog.debug("request completed");
      if (request.success()){
        IssuesHTMLTemplate = request.responseText();
        //CPLog.debug("WebTemplate"+IssuesHTMLTemplate)
        //[self load:0];
      }
    }
    request.onfailure=function(){
      CPLog.debug("Request failed");
    }
    request.send("");
    [_scrollView setAutohidesScrollers:NO];
     [self setDrawsBackground:YES];
    [self setScrollMode:CPWebViewScrollAppKit];
    [self setFrameLoadDelegate:self];
    [self setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
    return self;
  }
  -(void)setData:(id)data
  {
    _id=data.id;
    CPLog.debug("Data Id"+_id);
    //CPLog.debug("Data Renderfunc"+data.renderfunc);
    /*
    for (var key in data){
      CPLog.debug("Key="+key);
    }
    */
    scrollRect = [self visibleRect];

    var html = Mustache.to_html(IssuesHTMLTemplate, data);
    [self loadHTMLString:html];
//    var domWindow = [self DOMWindow];
//    domWindow.blaalert();
    //scrollRect.size.height=200;
  }
  - (void)webView:(CPWebView)aWebView didFinishLoadForFrame2:(id)aFrame
{
  CPLog.debug("WebView Loaded");
  
}
  - (void)setDrawsBackground:(BOOL)drawsBackground
{
    if (true)
        _iframe.style.backgroundColor = "rgb(237, 241, 244)";
    else
        _iframe.style.backgroundColor = "";
//        _iframe.style.backgroundColor = "rgb(237, 241, 244)";
}
- (void)webView:(CPWebView)aWebView didFinishLoadForFrame:(id)aFrame
{
    CPLog.debug("(void)webView:(CPWebView)aWebView didFinishLoadForFrame:(id)aFrame");
    /*
    var domWindow = [self DOMWindow];
   statsdata = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:@"/api/v1/statistic?id="+_id] returningResponse:nil];
  stats=[[statsdata rawString] objectFromJSON];
  //CPLog.debug([statsdata rawString]);
  if(stats.data){
    var data=[{
              name: 'Quality',
              data: stats.data.quality
            }, {
              name: 'Berlin',
              data: [-0.9, 0.6, 3.5, 8.4, 13.5, 17.0, 18.6, 17.9, 14.3, 9.0, 3.9, 1.0]
            }, {
              name: 'London',
              data: [3.9, 4.2, 5.7, 8.5, 11.9, 15.2, 17.0, 16.6, 14.2, 10.3, 6.6, 4.8]
            }];
	  if(domWindow.show_graph2)
            domWindow.show_graph2(stats.data);
          //domWindow.show_graph3();
        }
  for (var key in scrollRect.size){
      CPLog.debug("Key="+key+" value="+scrollRect.size[key]);
    }*/
    //[self setFrameSize:scrollRect.size];
    //[_frameView scrollRectToVisible:scrollRect];
    //[self setFrameSize:scrollRect.size];
    //[self setNeedsDisplay:YES];

  }
@end
