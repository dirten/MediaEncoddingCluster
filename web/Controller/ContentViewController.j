@import <Foundation/CPObject.j>

//@import "../Views/PhotosView.j"


@implementation ContentViewController : CPObject
  {
    CPView      contentView;

    CPDictionary    views;
    CPView          currentView;
  }

  - (void)init
  {
    
    [[CPNotificationCenter defaultCenter]
      addObserver:self
      selector:@selector(outlineViewSelectionDidChange:)
      name:CPOutlineViewSelectionDidChangeNotification
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
    //CPLog.debug([view className]+" set hidden");
    [views setObject:view forKey:[view className]];
  }

@end

@implementation ContentViewController (CPOutlineViewNotifications)

  - (void)outlineViewSelectionDidChange:(CPNotification)notification
  {

    var outlineView = [notification object];
    var selectedRow = [[outlineView selectedRowIndexes] firstIndex];
    var item = [outlineView itemAtRow:selectedRow];
    item=[item substringToIndex:[item length]-1];
    var view = [views objectForKey:item  + "View"];
    
    [currentView setHidden:YES];
    if (view)
      {
        [view setHidden:NO];
        [view refresh];
        currentView = view;
      }
    }

  @end
