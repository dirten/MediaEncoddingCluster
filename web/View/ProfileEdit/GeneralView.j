@import "/View/LabelTextFieldView.j"
@implementation GeneralView :CPView
{
    CPTextField label;
    CPTextField field;
    LabelTextFieldView view;
}
-(void)data:(id)obj
{
    [field setObjectValue:obj.data.name];
}
-(id)init
{
    //[super init];
    CPLog.debug("general view init");

    //view=[[LabelTextFieldView alloc] initWithId:@"testid"];
    view=[[LabelTextFieldView alloc] initWithFrame:CGRectMake(0,10,500,40)];
    [self addSubview:view];
    view=[[LabelTextFieldView alloc] initWithFrame:CGRectMake(0,35,500,40)];
    [self addSubview:view];
    view=[[LabelTextFieldView alloc] initWithFrame:CGRectMake(0,60,500,40)];
    [self addSubview:view];
    view=[[LabelTextFieldView alloc] initWithFrame:CGRectMake(0,85,500,40)];
    [self addSubview:view];
    //CPLog.debug('Id='+[view identifier]);
    //[view setBackgroundColor:[CPColor blackColor]];
    //[view setFrameOrigin:CGPointMake(50.0,15.0)];
    //[view setBounds:[self bounds]];
    /*
    label=[CPTextField labelWithTitle:@"Profile Name"  ];
    [label setFrameOrigin:CGPointMake(50.0,15.0)];
    //[label setAlignment:CPRightTextAlignment];

    field = [CPTextField textFieldWithStringValue:@"test" placeholder:@"placeholder" width:350 ];
    [field setFrameOrigin:CGPointMake(125.0,10.0)];


    [self addSubview:label];
    [self addSubview:field];*/
    return self;
}
@end