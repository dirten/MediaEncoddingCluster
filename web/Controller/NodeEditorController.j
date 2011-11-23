@import "../View/NodeEditorView.j"


SaveNodeEditorView = @"SaveNodeEditorView";
LoadNodeEditorView = @"LoadNodeEditorView";

@implementation NodeEditorController : CPObject
{
  NodeEditorView view;
  CPDictionary elementClasses;
}

-(void)initWithView:(id)theView{
  view=theView;
  elementClass=[CPDictionary dictionary];
  [elementClass setObject:[NodeInput class] forKey:@"Input"];
  [elementClass setObject:[NodeEncoding class] forKey:@"Encoding"];
  [elementClass setObject:[NodeOutput class] forKey:@"Output"];
  [elementClass setObject:[NodeExecutable class] forKey:@"Executable"];

  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(saveNodeEditorView:)
    name:SaveNodeEditorView
    object:nil];
  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(loadNodeEditorView:)
    name:LoadNodeEditorView
    object:nil];
}
  
-(void)saveNodeEditorView:(CPNotification)notification{
  CPLog.debug("saveNodeEditorView");
  CPLog.debug("Elements:"+[view elements]);

  var data={}
  data.tasks=new Array();
  data.links=new Array();
  data.positions=new Array();

  var elements=[view elements];
  var elementCount = [elements count];
  for (var index = 0; index<elementCount ; index++)
	{
    var element = [elements objectAtIndex:index];
    var task={
      name:[element name],
      uid:[element uid]
    };
    {/*this scope is for handle drawing*/
      var targets=[element outputElements];
      var targetCount = [targets count];
      for (var index2 = targetCount - 1; index2>=0; index2--) 
      {
        var target = [targets objectAtIndex:index2];
        var link={
          uid:[element uid],
          linksTo:[target uid]
        };
        data.links.push(link);
      }
    }

    var bounds=[element bounds];
    var pos={
      uid:[element uid],
      x:bounds.origin.x,
      y:bounds.origin.y
    };
    data.tasks.push(task);
    data.positions.push(pos);
  }
  CPLog.debug("Data:"+JSON.stringify(data));
  //CPLog.debug("Array:"+array);
}

-(void)loadNodeEditorView:(CPNotification)notification{
  CPLog.debug("loadNodeEditorView");
  var data={"tasks":[{"name":"Input","uid":1},{"name":"Encoding","uid":2},{"name":"Encoding","uid":3},{"name":"Output","uid":4}],"links":[{"uid":1,"linksTo":2},{"uid":1,"linksTo":3},{"uid":2,"linksTo":4},{"uid":3,"linksTo":4}],"positions":[{"uid":1,"x":581,"y":47},{"uid":2,"x":858,"y":42},{"uid":3,"x":1070,"y":361},{"uid":4,"x":1130,"y":90}]};
  CPLog.debug("Obj:"+data.tasks.length);
  var elements=[CPDictionary dictionary];
  [view clearElements];
  for(a=0;a<data.tasks.length;a++){
    var task=data.tasks[a];
    var obj=[[[elementClass objectForKey:task.name] alloc] init];
    [obj setUid:task.uid];
    [obj setBoundsOrigin:CPPointMake(data.positions[a].x,data.positions[a].y)];
    [[view elements] addObject:obj];
    [elements setObject:obj forKey:task.uid];
    [view setNeedsDisplay:YES];
    CPLog.debug("Data:"+obj);
  }
  for(a=0;a<data.links.length;a++){
    var link=data.links[a];
    var src=[elements objectForKey:link.uid];
    var trg=[elements objectForKey:link.linksTo];
    CPLog.debug("SourceHandle:"+[src outHandlePoint]);
    [src addTarget:trg];
  }
}
-(id)createObjectForName:(CPString)name
{
  
}
@end