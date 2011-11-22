//
//  MyTestController.m
//  
//
//  Created by Jan Hölscher on 08.11.11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

@implementation TestClass : CPObject
{
  float testFloat;
  CPString testString;
  
}

-(void)init{
  testString=[CPString stringWithString:"70"];//@"testvalue";
  testFloat=70.0;
  CPLog.debug("TestClass initialized with:"+testString);
}

-(void)change{
  testString=@"testvalue changed";
}
-(void)setString:(CPString)data{
  testString=data;
}

@end

@implementation TestTransformer: CPValueTransformer
{
  
}
+(class)transformedValueClass{return [CPNumber class];}
+(BOOL)allowReverseTransformation{return NO;}

-(id)transformedValue:(id)value{
  return parseInt(value);
}

@end

@implementation TestWindowController : CPObject
{
  @outlet CPTextField testField;
  @outlet CPSlider testSlider;
  
  TestClass testClass;
  
}
-(void)awakeFromCib{
//CPLog.debug("TestWindowController awakeFromCib!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  testClass=[TestClass alloc];
  [testClass init];
  controller=[[CPObjectController alloc] init];
  [controller bind:@"contentObject" toObject:self withKeyPath:@"testClass" options:nil];
  //[testField bind:@"objectValue" toObject:controller withKeyPath:@"selection.testString" options:nil];
  transformer=[TestTransformer alloc];
  [testField 
   bind:CPValueBinding 
   toObject:controller 
   withKeyPath:@"selection.testFloat" 
   options:[CPDictionary dictionaryWithObject:transformer forKey:CPValueTransformerBindingOption]
   ];
  [testSlider 
   bind:CPValueBinding 
   toObject:controller 
   withKeyPath:@"selection.testFloat" 
   options:[CPDictionary dictionaryWithObject:transformer forKey:CPValueTransformerBindingOption]
   ];
}

#pragma mark -
#pragma mark Actions

-(IBAction)sliderChanged:value{
  //    [testField setObjectValue:[value value]];
}
-(IBAction)buttonClicked:(id)aSender{
  CPLog.debug("button clicked:"+testClass.testFloat);
  //[testClass setValue:@"bla fasel" forKey:@"testString"];
  //CPLog.debug("button clicked:"+testClass.testString);
  [testClass setValue:"20.0" forKey:@"testFloat"]
  //    [testField setObjectValue:[value value]];
}
@end
