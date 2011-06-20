ProfileOpen = @"ProfileOpen",
ProfileSave = @"ProfileSave";

@implementation ProfileEditViewController : CPObject
  {
    ProfileEditView view;
  }
  -(void)init{
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(profileEdit:)
      name:ProfileDoubleClicked
      object:nil];

    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(profileSave:)
      name:ProfileSave
      object:nil];
  }
  -(void)profileEdit:(CPNotification)notification{
    CPLog.debug("profileEdit:"+[notification userInfo]);
    view = [[ProfileEditView alloc] init];
    [view setProfileId:[notification userInfo]];
  }
  -(void)profileSave:(CPNotification)notification{

  }
@end