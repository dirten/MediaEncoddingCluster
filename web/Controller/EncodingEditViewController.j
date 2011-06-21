
@implementation EncodingEditViewController : CPObject
  {
    ProfileEditView view;
  }
  -(void)init{
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(profileEdit:)
      name:EncodingDoubleClicked
      object:nil];
      /*
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(profileSave:)
      name:EncodingSave
      object:nil];*/
  }
  
  -(void)profileEdit:(CPNotification)notification{
    CPLog.debug("profileEdit:"+[notification userInfo]);
    view = [[EncodingEditView alloc] init];
    [view setEncodingId:[notification userInfo]];
  }
  -(void)profileSave:(CPNotification)notification{

  }
@end