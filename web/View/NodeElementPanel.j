
@import "NodeView.j"
@import "NodeInput.j"
@import "NodeOutput.j"
@import "NodeExecutable.j"
@import "NodeEncoding.j"
@import "NodeConnectorView.j"
@import "NodeEditorView.j"

NodeElementDragType = "NodeElementDragType";

@implementation NodeElementPanel : CPPanel
{
  CPArray images;
}

- (id)init
{
    self = [self initWithContentRect:CGRectMake(20.0, 30.0, 300.0, 400.0)
                           styleMask:CPHUDBackgroundWindowMask |
                                     CPResizableWindowMask];

    if (self)
    {
        [self setTitle:"Elements"];
        [self setFloatingPanel:YES];

        var contentView = [self contentView],
            bounds = [contentView bounds];

        var okButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-90,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
        [okButton setTitle:@"Save"];
        [okButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
        [okButton setTarget:self];
        [okButton setAction:@selector(save:)];
        [contentView addSubview:okButton];

        var loadButton=[[CPButton alloc] initWithFrame:CGRectMake(CGRectGetWidth([contentView bounds])-190,CGRectGetHeight([contentView bounds])-40,80.0,24.0)];
        [loadButton setTitle:@"Load"];
        [loadButton setAutoresizingMask:CPViewMinXMargin|CPViewMinYMargin];
        [loadButton setTarget:self];
        [loadButton setAction:@selector(load:)];
        [contentView addSubview:loadButton];

        
        bounds.size.height -= 50.0;
        
        var photosView = [[CPCollectionView alloc] initWithFrame:bounds];
        
        [photosView setAutoresizingMask:CPViewWidthSizable];
        [photosView setMinItemSize:CGSizeMake(120, 100)];
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
          
          [[NodeInput alloc] init],
          [[NodeEncoding alloc] init],
          [[NodeOutput alloc] init],
          [[NodeExecutable alloc] init],
          [[NodeView alloc] initWithName:@"test3" withInputHandle:YES andOutputHandle:YES],
          [[NodeView alloc] initWithName:@"test4" withInputHandle:YES andOutputHandle:YES],
          [[NodeView alloc] initWithName:@"test5" withInputHandle:YES andOutputHandle:YES],
          [[NodeView alloc] initWithName:@"test6" withInputHandle:YES andOutputHandle:YES]
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
- (void)save:(id)sender{
  CPLog.debug("SAVING Encoding Graph:");
  [[CPNotificationCenter defaultCenter]
    postNotificationName:SaveNodeEditorView
    object:self
    userInfo:nil];
  }
  //CPLog.debug("Data:"+JSON.stringify([pdata toJSON]));
}
- (void)load:(id)sender{
  CPLog.debug("LOADING Encoding Graph:");
  [[CPNotificationCenter defaultCenter]
    postNotificationName:LoadNodeEditorView
    object:self
    userInfo:nil];
  }
  //CPLog.debug("Data:"+JSON.stringify([pdata toJSON]));
}
@end

@implementation NodeItemView : CPBox
{
    CPTextField textField;
    NodeView node;
}

- (void)setRepresentedObject:(id)anObject
{
    //[self setBounds:[anObject bounds]];
    [self setCornerRadius:3.0];
    //[self setBorderWidth:3.0];
    //return;
    //[self setBorderType:CPGrooveBorder];
    //[self setPostsFrameChangedNotifications:YES];

    //[self addSubview:anObject];
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
