@import <Foundation/CPObject.j>

//@import "../Views/PhotosView.j"


@implementation ContentViewController : CPObject
{
    @outlet     CPView      contentView;

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
}

- (void)addViewController:(id)viewController
{
    var view = [viewController setupViewWithFrame:[contentView bounds]];
    [contentView addSubview:view];
    [view setHidden:YES];
    
    [views setObject:view forKey:[view className]];
}

@end

@implementation ContentViewController (CPOutlineViewNotifications)

- (void)outlineViewSelectionDidChange:(CPNotification)notification
{
    
    var outlineView = [notification object];
    var selectedRow = [[outlineView selectedRowIndexes] firstIndex];
    var item = [outlineView itemAtRow:selectedRow];
    CPLog.debug(item);

}

@end
