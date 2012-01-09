

@implementation GraphElementView : CPScrollView
{
CPArray elements;
}
-(id)initWithFrame:(CPRect)aFrame
{
  self=[super initWithFrame:aFrame];
  var elementsView = [[CPCollectionView alloc] initWithFrame:aFrame];
  //[elementsView setMaxNumberOfColumns:1];
  [elementsView setAutoresizingMask:CPViewWidthSizable];
  [elementsView setMinItemSize:CGSizeMake(150, 100)];
  [elementsView setMaxItemSize:CGSizeMake(250, 100)];
  [elementsView setDelegate:self];

  var elementPrototype = [[CPCollectionViewItem alloc] init],
      elementView = [[NodeItemView alloc] initWithFrame:CGRectMakeZero()];
  [elementPrototype setView:elementView];
  [elementsView setItemPrototype:elementPrototype];

  [self setDocumentView:elementsView];
  [self setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
  [self setAutohidesScrollers:YES];
  [[self contentView] setBackgroundColor:[CPColor whiteColor]];
  elements = [
    [[NodeInput alloc] init],
    [[NodeEncoding alloc] init],
    [[NodeOutput alloc] init],
    [[NodeExecutable alloc] init]
  ];
  [elementsView setContent:elements];
  return self;
}

-(CPArray)collectionView:(CPCollectionView)aCollectionView
 dragTypesForItemsAtIndexes:(CPIndexSet)indices
{
    return [NodeElementDragType];
}
- (CPData)collectionView:(CPCollectionView)aCollectionView
   dataForItemsAtIndexes:(CPIndexSet)indices
                 forType:(CPString)aType
{
    var firstIndex = [indices firstIndex];
    
    return [CPKeyedArchiver archivedDataWithRootObject:elements[firstIndex]];
}

@end