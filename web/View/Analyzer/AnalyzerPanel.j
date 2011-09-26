@implementation PhotoView : CPView
{
    CPImageView _imageView;
}

- (void)setRepresentedObject:(id)anObject
{
  CPLog.debug(anObject);
    if (!_imageView)
    {
        var frame = CGRectInset([self bounds], 5.0, 5.0);
        
        _imageView = [[CPImageView alloc] initWithFrame:frame];
        
        [_imageView setImageScaling:CPScaleProportionally];
        [_imageView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
        
        [self addSubview:_imageView];
    }
    
    [_imageView setImage:anObject];
}

- (void)setSelected:(BOOL)isSelected
{
    [self setBackgroundColor:isSelected ? [CPColor grayColor] : nil];
}

@end

@implementation AnalyzerPanel :CPPanel
{
  CPArray images;
}

-(id)initWithFrame:(GCRect)frame{
  //var self=[super initWithFrame:frame];
self = [self initWithContentRect:CGRectMake(10.0, 20.0, 300.0, 400.0)
                           styleMask:CPHUDBackgroundWindowMask |
                                     CPClosableWindowMask |
                                     CPResizableWindowMask];
 [self setTitle:"Photos"];
        [self setFloatingPanel:YES];
        
var contentView = [self contentView],
            bounds = [contentView bounds];
  //var contentView=[self contentView];

  //image=[[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=2&type=dec" size:CPSizeMake(30, 25)];
  images=[
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=2&type=dec" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=2&type=dec1" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=3&type=dec" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=3&type=dec1" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=4&type=dec" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=4&type=dec1" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=5&type=dec" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=5&type=dec1" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=6&type=dec" ],
    [[CPImage alloc] initWithContentsOfFile:@"http://localhost:8080/api/v1/pufs?puid=1871&frame=6&type=dec1" ]
  ];
  

  var photosView = [[CPCollectionView alloc] initWithFrame:bounds];
  [photosView setAutoresizingMask:CPViewWidthSizable|CPViewHeightSizable];
        [photosView setMinItemSize:CGSizeMake(100, 100)];
        [photosView setMaxItemSize:CGSizeMake(100, 100)];

  var itemPrototype = [[CPCollectionViewItem alloc] init],
      photoView = [[PhotoView alloc] initWithFrame:CGRectMakeZero()];

  [itemPrototype setView:photoView];
        
  [photosView setItemPrototype:itemPrototype];        
  var scrollView = [[CPScrollView alloc] initWithFrame:bounds];
  [scrollView setDocumentView:photosView];
  [scrollView setAutoresizingMask:CPViewWidthSizable | CPViewHeightSizable];
  [scrollView setAutohidesScrollers:NO];

  [[scrollView contentView] setBackgroundColor:[CPColor whiteColor]];

  [contentView addSubview:scrollView];


  [photosView setContent:images];
  //[self addSubview:image];
  return self;
}
@end
