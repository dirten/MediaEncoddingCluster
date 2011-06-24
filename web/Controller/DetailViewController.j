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
  - (void)addView:(id)view
  {
    [view setHidden:YES];
    CPLog.debug([view className]+" set hidden");
    [views setObject:view forKey:[view className]];
  }



  - (void)viewSelectionDidChange:(CPNotification)notification
  {
    var outlineView = [notification object];
    CPLog.debug([outlineView class]);
  @end
