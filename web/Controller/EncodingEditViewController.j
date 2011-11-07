@import "../View/JobEditorWindow.j"
@implementation EncodingEditViewController : CPObject
  {
    ProfileEditView view;
  }
  -(void)init{
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(openEncodingEditView:)
      name:EncodingDoubleClicked
      object:nil];
      /*
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(encodingSave:)
      name:EncodingSave
      object:nil];*/
  }
  
  -(void)openEncodingEditView:(CPNotification)notification{
    CPLog.debug("endodingEdit:"+[notification userInfo]);
    //view = [[EncodingEditView alloc] init];
    //[view setEncodingId:[notification userInfo]];
    view = [[JobEditorWindow alloc] init];
    //[view setEncodingId:[notification userInfo]];
  }
  -(void)encodingSave:(CPNotification)notification{

  }
@end