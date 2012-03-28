

@implementation ElementClassDictionary : CPObject
{

}

+(CPDictionary)nodeElementDictionary{
  var elementClass=[CPDictionary dictionary];
  [elementClass setObject:[NodeInput class] forKey:[[[NodeInput alloc] init] taskName]];
  [elementClass setObject:[NodeEncoding class] forKey:[[[NodeEncoding alloc] init] taskName]];
  [elementClass setObject:[NodeOutput class] forKey:[[[NodeOutput alloc] init] taskName]];
  [elementClass setObject:[NodeExecutable class] forKey:[[[NodeExecutable alloc] init] taskName]];
  return elementClass;
}
+(id)nodeElements{
  return [
    [[NodeInput alloc] init],
    [[NodeEncoding alloc] init],
    [[NodeOutput alloc] init]
    //[[NodeExecutable alloc] init]
  ];

  //return elements;
}

@end