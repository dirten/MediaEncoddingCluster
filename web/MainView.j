@implementation ProfileEditView :CPWindow
{

}
-(id)init{
    CPLog.debug("ProfileEditView INIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    //self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPHUDBackgroundWindowMask|CPClosableWindowMask|CPResizableWindowMask];
    self=[super initWithContentRect:CGRectMake(15,150,325,225) styleMask:CPClosableWindowMask|CPResizableWindowMask];
    //[self setFloatingPanel:YES];
    [self orderFront:self];
    [self setTitle:"Profile Editor"];

    return self;
}
@end
