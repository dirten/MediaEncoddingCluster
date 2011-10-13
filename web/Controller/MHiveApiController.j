@implementation MHiveAPIController : CPObject
{
}

+ (id)sharedController
{
    if (!SharedController)
    {
        SharedController = [[super alloc] init];
        [CPURLConnection setClassDelegate:SharedController];
    }

    return SharedController;
}

@end
