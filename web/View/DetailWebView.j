@import "mustache.js"

@implementation DetailWebView : CPWebView
  {
    id IssuesHTMLTemplate;
    CPDictionary _data;
    CGRect scrollRect;
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
    
    for (var key in data){
      CPLog.debug("Key="+key);
    }
    scrollRect = [self visibleRect];

    var html = Mustache.to_html(IssuesHTMLTemplate, data);
    [self loadHTMLString:html];
    //scrollRect.size.height=200;
  }
  - (void)webView:(CPWebView)aWebView didFinishLoadForFrame:(id)aFrame
{
  CPLog.debug("WebView Loaded");
  for (var key in scrollRect.size){
      CPLog.debug("Key="+key+" value="+scrollRect.size[key]);
    }
    //[self setFrameSize:scrollRect.size];
    //[_frameView scrollRectToVisible:scrollRect];
    [self setFrameSize:scrollRect.size];
  }
  - (void)setDrawsBackground:(BOOL)drawsBackground
{
    if (true)
        _iframe.style.backgroundColor = "rgb(237, 241, 244)";
    else
        _iframe.style.backgroundColor = "";
}
@end
