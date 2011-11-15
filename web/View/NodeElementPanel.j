
@import "NodeView.j"
@import "NodeConnectorView.j"
@import "NodeEditorView.j"

NodeElementDragType = "NodeElementDragType";

@implementation NodeElementPanel : CPPanel
{
  CPArray images;
}

- (id)init
{
    self = [self initWithContentRect:CGRectMake(10.0, 20.0, 300.0, 400.0)
                           styleMask:CPHUDBackgroundWindowMask |
                                     CPClosableWindowMask |
                                     CPResizableWindowMask];

    if (self)
    {
        [self setTitle:"Elements"];
        [self setFloatingPanel:YES];



        var contentView = [self contentView],
            bounds = [contentView bounds];
        
        bounds.size.height -= 20.0;
        
        var photosView = [[CPCollectionView alloc] initWithFrame:bounds];
        
        [photosView setAutoresizingMask:CPViewWidthSizable];
        [photosView setMinItemSize:CGSizeMake(100, 100)];
        [photosView setMaxItemSize:CGSizeMake(200, 200)];
        [photosView setDelegate:self];
        var itemPrototype = [[CPCollectionViewItem alloc] init],
            photoView = [[NodeItemView alloc] initWithFrame:CGRectMakeZero()];
        
        [itemPrototype setView:photoView];
        
        [photosView setItemPrototype:itemPrototype];
        
        var scrollView = [[CPScrollView alloc] initWithFrame:bounds];
        
        [scrollView setDocumentView:photosView];
        [scrollView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
        [scrollView setAutohidesScrollers:YES];

        [[scrollView contentView] setBackgroundColor:[CPColor whiteColor]];

        [contentView addSubview:scrollView];
        
        images = [  
          [[NodeConnectorView alloc] initWithName:@"connect"],
          [[NodeView alloc] initWithName:@"test1"],
          [[NodeView alloc] initWithName:@"test2"],
          [[NodeView alloc] initWithName:@"test3"],
          [[NodeView alloc] initWithName:@"test4"],
          [[NodeView alloc] initWithName:@"test5"],
          [[NodeView alloc] initWithName:@"test6"]
        ];
                    
        [photosView setContent:images];
    }
    
    return self;
}
-   (CPArray)collectionView:(CPCollectionView)aCollectionView
 dragTypesForItemsAtIndexes:(CPIndexSet)indices
{
    return [NodeElementDragType];
}
- (CPData)collectionView:(CPCollectionView)aCollectionView
   dataForItemsAtIndexes:(CPIndexSet)indices
                 forType:(CPString)aType
{
    var firstIndex = [indices firstIndex];
    
    return [CPKeyedArchiver archivedDataWithRootObject:images[firstIndex]];
}
@end

@implementation NodeItemView : CPView
{
    CPTextField textField;
    NodeView node;
}

- (void)setRepresentedObject:(id)anObject
{
    //[self addSubview:anObject];
    //return;
    if (!textField)
    {
        textField = [[CPTextField alloc] initWithFrame:CGRectMake(50.0, 15.0, 145.0, 20.0)];
        [textField setFont:[CPFont boldSystemFontOfSize:12.0]];
        [self addSubview:textField];
    }
    [textField setStringValue:[anObject name]];
}

- (void)setSelected:(BOOL)isSelected
{
    [self setBackgroundColor:isSelected ? [CPColor blueColor] : nil];
    [textField setTextColor:isSelected ? [CPColor whiteColor] : [CPColor blackColor]];
}

@end
