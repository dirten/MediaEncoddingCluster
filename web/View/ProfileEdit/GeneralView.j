@implementation GeneralView :CPView
{
    CPTextField label;
    CPTextField field;
}

-(id)init
{
    [super init];
    CPLog.debug("general view init");

    label=[CPTextField labelWithTitle:@"Profile Name"  ];
    [label setFrameOrigin:CGPointMake(50.0,15.0)];
    //[label setAlignment:CPRightTextAlignment];

    field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:200 ];
    [field setFrameOrigin:CGPointMake(150.0,10.0)];


    [self addSubview:label];
    [self addSubview:field];
    return self;
}
@end