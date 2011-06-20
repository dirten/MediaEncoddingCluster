@import <Foundation/CPObject.j>


@implementation ProfileViewController : CPObject
  {
    CPView view;
  }

  - (id)init
  {
  }
  - (id)setupView:(CGRect)frame
  {
    if(view==nil)
      view = [[ProfileView alloc] initWithFrame:frame];
    return view
  }
@end
