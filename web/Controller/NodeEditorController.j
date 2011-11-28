@import "../View/NodeEditorView.j"


SaveNodeEditorView = @"SaveNodeEditorView";
LoadNodeEditorView = @"LoadNodeEditorView";

@implementation NodeEditorController : CPObject
{
  NodeEditorView view;
  CPDictionary elementClasses;
  id loadedUUID;
}

-(void)initWithView:(id)theView{
  view=theView;
  elementClass=[CPDictionary dictionary];
  [elementClass setObject:[NodeInput class] forKey:@"InputTask"];
  [elementClass setObject:[NodeEncoding class] forKey:@"EncodingTask"];
  [elementClass setObject:[NodeOutput class] forKey:@"OutputTask"];
  [elementClass setObject:[NodeExecutable class] forKey:@"ExecutableTask"];
  loadedUUID="";
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

  var data={}
  data.tasks=new Array();
  data.links=new Array();
  data.positions=new Array();

  var elements=[view elements];
  var elementCount = [elements count];
  for (var index = 0; index<elementCount ; index++)
	{
    var element = [elements objectAtIndex:index];
    CPLog.debug("ElementsData:"+JSON.stringify([[element data] toJSON]));
    var task={
      name:[element taskName],
      uid:[element uid],
      data:[[element data] toJSON].data
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
  if(loadedUUID)
    data.uuid=loadedUUID;
  
  var request = [CPURLRequest requestWithURL:@"/api/v1/graph"];
  [request setHTTPMethod:"POST"];
  [request setHTTPBody:JSON.stringify(data)];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("Graph Save Result"+[result rawString]);

  //CPLog.debug("Array:"+array);
}

-(void)loadNodeEditorView:(CPNotification)notification{
  //CPLog.debug("loadNodeEditorView:"+[notification userInfo]);
  //var data={"tasks":[{"name":"Input","uid":1},{"name":"Encoding","uid":2},{"name":"Encoding","uid":3},{"name":"Output","uid":4}],"links":[{"uid":1,"linksTo":2},{"uid":1,"linksTo":3},{"uid":2,"linksTo":4},{"uid":3,"linksTo":4}],"positions":[{"uid":1,"x":581,"y":47},{"uid":2,"x":858,"y":42},{"uid":3,"x":1070,"y":361},{"uid":4,"x":1130,"y":90}]};
  var path="/api/v1/graph?uuid="+[notification userInfo];
  var response=[CPHTTPURLResponse alloc];
  var error;
  var raw_data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:response];
  //CPLog.debug("raw_data:"+[raw_data rawString]);
  var data=[raw_data JSONObject].data;
  loadedUUID=data.uuid;
  //CPLog.debug("Obj:"+data.tasks.length);
  var elements=[CPDictionary dictionary];
  [view clearElements];
  for(a=0;a<data.tasks.length;a++){
    var task=data.tasks[a];
    var taskdata=undefined;
    if(data.tasks[a].data)
      taskdata={data:data.tasks[a].data};
    
    //CPLog.debug("TaskData:"+JSON.stringify(taskdata));
    var obj=[[[elementClass objectForKey:task.name] alloc] init];
    [obj setUid:task.uid];
    [obj setBoundsOrigin:CPPointMake(data.positions[a].x,data.positions[a].y)];
    [obj setProgress:@" "];
    if(taskdata!=undefined)
      [obj setData:[CPDictionary dictionaryWithJSObject:taskdata recursively:YES]];
    [[view elements] addObject:obj];
    [elements setObject:obj forKey:task.uid];
    
    //[view setNeedsDisplay:YES];
    //CPLog.debug("Data:"+obj);
  }
  for(a=0;a<data.links.length;a++){
    var link=data.links[a];
    var src=[elements objectForKey:link.uid];
    var trg=[elements objectForKey:link.linksTo];
    //CPLog.debug("SourceHandle:"+CPStringFromPoint([src outHandlePoint]));
    //CPLog.debug("SourceHandle:"+CPStringFromPoint([trg inHandlePoint]));
    [src addTarget:trg];
    [trg addSource:src];
  }
  [view setNeedsDisplay:YES];
}
-(id)createObjectForName:(CPString)name
{
  
}
@end