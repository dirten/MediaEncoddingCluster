@implementation LabelTextFieldView :CPView
{
    id _id;
    CPTextField label;
    CPTextField field;

}
-(id)initWithId:(id)identifier
{
     //[super init];
    _id=identifier;
    label=[CPTextField labelWithTitle:@"Profile Name"  ];
    //[label setFrameOrigin:CGPointMake(50.0,15.0)];
    //[label setAlignment:CPRightTextAlignment];

    field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:350 ];
    //[field setFrameOrigin:CGPointMake(125.0,10.0)];


    [self addSubview:label];
    [self addSubview:field];

    return self;
}
-(id)initWithFrame:(CGRect)aFrame
{
     [super initWithFrame:aFrame];
    //_id=identifier;
    label=[CPTextField labelWithTitle:@"Profile Name"  ];
    [label setFrameOrigin:CGPointMake(0.0,5.0)];
    [label setAlignment:CPRightTextAlignment];

    field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:350 ];
    [field setFrameOrigin:CGPointMake(125.0,0.0)];


    [self addSubview:label];
    [self addSubview:field];

    return self;
}

-(id)identifier
{
    return _id;
}

-(void)data:(id)obj
{
    [field setObjectValue:obj.data.name];
}


@end