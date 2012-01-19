
@import "NodeView.j"
@import "NodeInput.j"
@import "NodeOutput.j"
@import "NodeExecutable.j"
@import "NodeEncoding.j"
@import "NodeConnectorView.j"
@import "NodeEditorView.j"
@import "GraphListView.j"
@import "GraphElementView.j"

NodeElementDragType = "NodeElementDragType";

@implementation NodeElementPanel : CPPanel
{
  CPArray images;
  GraphListView graphListView;
  GraphElementView graphElementView;
}

- (id)init
{
    self = [self initWithContentRect:CGRectMake(20.0, 30.0, 300.0, 600.0)
                           styleMask:CPHUDBackgroundWindowMask |
                                     CPResizableWindowMask];

    if (self)
    {
        [self setTitle:"Elements"];
        [self setFloatingPanel:YES];

        var contentView = [self contentView],
            bounds = [contentView bounds];
        graphElementView=[[GraphElementView alloc] initWithFrame:CPRectMake(0,0,300,250)];
        [contentView addSubview:graphElementView];
        graphListView=[[GraphListView alloc] initWithFrame:CPRectMake(0,260,300,270)];
        //var graphListView=[[GraphListView alloc] initWithFrame:bounds];
        [contentView addSubview:graphListView];

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
        return self;
        
        bounds.size.height -= 350.0;
        
        var photosView = [[CPCollectionView alloc] initWithFrame:bounds];
        [photosView setMaxNumberOfColumns:0];
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

        //bounds.origin.y += 260.0;
        graphListView=[[GraphListView alloc] initWithFrame:CPRectMake(0,260,300,270)];
        //var graphListView=[[GraphListView alloc] initWithFrame:bounds];
        [contentView addSubview:graphListView];
        
        images = [  
          
          [[NodeInput alloc] init],
          [[NodeEncoding alloc] init],
          [[NodeExecutable alloc] init],
          [[NodeOutput alloc] init],
          [[NodeView alloc] initWithName:@"test3" withInputHandle:YES andOutputHandle:YES taskName:@""],
          [[NodeView alloc] initWithName:@"test4" withInputHandle:YES andOutputHandle:YES taskName:@""],
          [[NodeView alloc] initWithName:@"test5" withInputHandle:YES andOutputHandle:YES taskName:@""],
          [[NodeView alloc] initWithName:@"test6" withInputHandle:YES andOutputHandle:YES taskName:@""]
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

- (void)save:(id)sender
{
  CPLog.debug("SAVING Encoding Graph:");
  [[CPNotificationCenter defaultCenter]
    postNotificationName:SaveNodeEditorView
    object:self
    userInfo:nil];
}
  
- (void)load:(id)sender
{
  CPLog.debug("LOADING Encoding Graph:"+[graphListView selectedId]);
  if([graphListView selectedId]){
    [[CPNotificationCenter defaultCenter]
      postNotificationName:LoadNodeEditorView
      object:self
      userInfo:[graphListView selectedId]];
  }else{
    var alert=[CPAlert alertWithError:@"please select a graph from the list"];
    [alert runModal];
  }
}


@end

@implementation NodeItemView : CPBox
{
  CPTextField textField;
  CPTextField descField;
  CPImageView imageView;
  NodeView node;
}

- (void)setRepresentedObject:(id)anObject
{
  CPLog.debug("ElementViewBounds"+CGStringFromRect([self bounds]));
  CPLog.debug("ElementViewBounds"+CGStringFromRect([anObject bounds]));
  //[self setBounds:[anObject bounds]];
  [self setCornerRadius:3.0];
  //[self setBorderWidth:3.0];
  //return;
  //[self setBorderType:CPGrooveBorder];
  //[self setPostsFrameChangedNotifications:YES];

  //[self addSubview:anObject];
  if(!imageView){
    imageView=[[CPImageView alloc] initWithFrame:CGRectMake(230.0, 25.0, 60.0, 60.0)];
  }
  if ([anObject image]){ 
    //[imageView setAlphaValue:0.5];
    [imageView setImage:[anObject image]];
    [self addSubview:imageView];
  }
  if (!textField){ 
    textField = [[CPTextField alloc] initWithFrame:CGRectMake(230.0, 10.0, 145.0, 20.0)];
    [textField setFont:[CPFont boldSystemFontOfSize:12.0]];
    //[textField setAlphaValue:0.5];
    [self addSubview:textField];
  }
  [textField setStringValue:[anObject name]];
  if (!descField){ 
    descField = [[CPTextField alloc] initWithFrame:CGRectMake(10.0, 10.0, 220.0, 100.0)];
    //[descField setFont:[CPFont boldSystemFontOfSize:12.0]];
    [descField setAlphaValue:0.8];
    [descField setLineBreakMode:CPLineBreakByWordWrapping];
    [self addSubview:descField];
  }
  [descField setStringValue:[anObject taskDescription]];
}

- (void)setSelected:(BOOL)isSelected
{
    [self setBackgroundColor:isSelected ? [CPColor grayColor] : nil];
    [textField setTextColor:isSelected ? [CPColor whiteColor] : [CPColor blackColor]];
}

@end
