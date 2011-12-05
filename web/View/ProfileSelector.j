
@import "ProfileView.j"
@implementation ProfileSelector :CPWindow
{
  ProfileView profileList;
}

+(id)show
{
  var window=[CPWindow alloc];
  window=[window initWithContentRect:CGRectMake(280,50,700,155) styleMask:CPClosableWindowMask|CPResizableWindowMask];
  [window orderFront:self];
  [window setTitle:"Encoding Editor"];
  var contentView=[window contentView];
  profileList=[[ProfileView alloc] initWithFrame:[contentView bounds]];
  [contentView addSubview:profileList];
  return window;

}
@end