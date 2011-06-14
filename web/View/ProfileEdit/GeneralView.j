@implementation GeneralView :CPView
{
    CPTextField label;
    CPTextField field;
}
-(id)data:(id)obj
{
    [field setObjectValue:obj.data.name];
}
-(id)init
{
    [super init];
    CPLog.debug("general view init");

    label=[CPTextField labelWithTitle:@"Profile Name"  ];
    [label setFrameOrigin:CGPointMake(50.0,15.0)];
    //[label setAlignment:CPRightTextAlignment];

    field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:350 ];
    [field setFrameOrigin:CGPointMake(125.0,10.0)];


    [self addSubview:label];
    [self addSubview:field];
    return self;
}
@end