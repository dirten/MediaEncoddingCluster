@import <AppKit/CPView.j>

@implementation ProgressTableDataView : CPView
{
    float value;
    int base_value;
}

- (void)setObjectValue:(id)aValue
{
  base_value=aValue;
  //aValue=aValue/100;
    value = MAX(MIN(aValue/100, 1), 0);
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(CGRect)aRect
{
    [[self subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
    var bounds = [self bounds],
        maxWidth = (bounds.size.width - 2) * value,
        context = [[CPGraphicsContext currentContext] graphicsPort],
        rectToDraw = CGRectMake(2, 2, 5, bounds.size.height - 4),
        transform = CGAffineTransformMakeTranslation(6, 0);

    CGContextBeginPath(context);
    while (rectToDraw.origin.x < maxWidth)
    {
        if (CGRectGetMaxX(rectToDraw) > maxWidth)
            rectToDraw.size.width -= (CGRectGetMaxX(rectToDraw) - maxWidth);

        CGContextAddRect(context, rectToDraw);
        rectToDraw = CGRectApplyAffineTransform(rectToDraw, transform);
    }

    var color = [CPColor whiteColor];
    if (![self hasThemeState:CPThemeStateSelectedDataView])
        color = [CPColor colorWithRed:176/255 green:178/255 blue:180/255 alpha:0.7];

    CGContextSetFillColor(context, color);
    CGContextFillPath(context);
    var l=base_value>0?"%":"";
    var label=[CPTextField labelWithTitle:base_value+l];
    [label setFrameOrigin:CGPointMake(10.5,2.5)];
    [label setTextColor:[CPColor darkGrayColor]];

    [self addSubview:label];

}

@end
