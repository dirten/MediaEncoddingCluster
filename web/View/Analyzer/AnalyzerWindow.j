@import "AnalyzerPanel.j"
@implementation AnalyzerWindow :CPWindow

-(id)init{
  var self=[super initWithContentRect:CGRectMake(280,50,600,500) styleMask:CPClosableWindowMask|CPResizableWindowMask];
  var contentView=[self contentView];
  [self orderFront:self];
  [self setTitle:"Codec Analyzer"];
  [[[AnalyzerPanel alloc] initWithFrame:[contentView bounds]] orderFront:nil];
//[[[PhotoPanel alloc] init] orderFront:nil];
  //[contentView addSubview:analyzerPanel];
  return self;
}
@end