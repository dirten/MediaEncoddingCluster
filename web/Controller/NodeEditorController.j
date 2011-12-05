@import "../View/NodeEditorView.j"


SaveNodeEditorView = @"SaveNodeEditorView";
LoadNodeEditorView = @"LoadNodeEditorView";
NewNodeEditorView = @"NewNodeEditorView";
RenameNodeEditorView = @"RenameNodeEditorView";
DeleteNodeEditorView = @"DeleteNodeEditorView";
SubmitNodeEditorView = @"SubmitNodeEditorView";

var _instance;
testfunc=function(){
  CPLog.debug("Loaded here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
}
testfunc();
@implementation NodeEditorController : CPObject
{
  NodeEditorView view;
  CPDictionary elementClasses;
  id loadedUUID;
  CPString loadedName;
  id growl;
}

+(id)instance:(id)theView
{
  CPLog.debug("Instance:"+_instance);
  if(!_instance)
    _instance=[[NodeEditorController alloc] initWithView:theView];
  CPLog.debug("Instance after:"+_instance);
  return _instance;
}
-(id)initWithView:(id)theView{
  self=[super init];
  CPLog.debug("Creating new Instance of NodeEditorController");
  view=theView;
  elementClass=[CPDictionary dictionary];
  [elementClass setObject:[NodeInput class] forKey:@"InputTask"];
  [elementClass setObject:[NodeEncoding class] forKey:@"EncodingTask"];
  [elementClass setObject:[NodeOutput class] forKey:@"OutputTask"];
  [elementClass setObject:[NodeExecutable class] forKey:@"ExecutableTask"];
  loadedUUID="";
  growl=[TNGrowlCenter defaultCenter];

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
  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(newNodeEditorView:)
    name:NewNodeEditorView
    object:nil];
  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(renameNodeEditorView:)
    name:RenameNodeEditorView
    object:nil];
  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(deleteNodeEditorView:)
    name:DeleteNodeEditorView
    object:nil];
  [[CPNotificationCenter defaultCenter]
    addObserver:self
    selector:@selector(submitNodeEditorView:)
    name:SubmitNodeEditorView
    object:nil];
    return self;
}
  
-(void)saveNodeEditorView:(CPNotification)notification{
  CPLog.debug("saveNodeEditorView"+notification);
  
  var data={}
  data.tasks=new Array();
  data.links=new Array();
  data.positions=new Array();

  var elements=[view elements];
  var elementCount = [elements count];
  CPLog.debug("Element count"+elementCount);
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
  if(loadedName)
    data.name=loadedName;
  if(elementCount>0){
    var request = [CPURLRequest requestWithURL:@"/api/v1/graph"];
    [request setHTTPMethod:"POST"];
    [request setHTTPBody:JSON.stringify(data)];
    var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
    CPLog.debug("Graph Save Result"+[result rawString]);
    loadedUUID=[result JSONObject].uuid;
    [[notification object] refresh];
  }
  [growl pushNotificationWithTitle:@"Graph Saved" message:"Graph "+loadedName+" successful saved"];
  [view setUnsavedChanges:NO];
  [view setNeedsDisplay:YES];
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
  loadedName=data.name;
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
  [view setName:loadedName];
  [view setNeedsDisplay:YES];
}

-(void)newNodeEditorView:(CPNotification)notification
{
  CPLog.debug("Self"+self);
  [view clearElements];
  //loadedUUID=nil;
  loadedName=[notification object];
  var data={};
  data.tasks=new Array();
  data.links=new Array();
  data.positions=new Array();

  CPLog.debug("new Editor with name "+loadedName);
  CPLog.debug("Data:"+JSON.stringify(data));
  //if(loadedUUID)
  //  data.uuid=loadedUUID;
  if(loadedName)
    data.name=loadedName;
  var request = [CPURLRequest requestWithURL:@"/api/v1/graph"];
  [request setHTTPMethod:"POST"];
  [request setHTTPBody:JSON.stringify(data)];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("Graph Save Result"+[result rawString]);
  loadedUUID=[result JSONObject].uuid;
  //[[notification object] refresh];
  [view setName:loadedName];
  [view setNeedsDisplay:YES];
}

-(void)renameNodeEditorView:(CPNotification)notification
{
  var path="/api/v1/graph?uuid="+[notification userInfo];
  var response=[CPHTTPURLResponse alloc];
  var error;
  var raw_data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:response];
  //CPLog.debug("raw_data:"+[raw_data rawString]);
  var data=[raw_data JSONObject].data;
  var oldName=data.name;
  data.name=[notification object];

  var request = [CPURLRequest requestWithURL:@"/api/v1/graph"];
  [request setHTTPMethod:"POST"];
  [request setHTTPBody:JSON.stringify(data)];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("Graph Save Result"+[result rawString]);
  //loadedUUID=[result JSONObject].uuid;
  [growl pushNotificationWithTitle:@"Graph renamed" message:"Graph "+oldName+" successful renamed to "+data.name];

}

-(void)deleteNodeEditorView:(CPNotification)notification
{
  var request = [CPURLRequest requestWithURL:@"/api/v1/graph?delete&uuid="+[notification userInfo]];
  //[request setHTTPMethod:"POST"];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("Graph delete Result"+[result rawString]);
  [view clearElements];
  loadedUUID=nil;
  loadedName=nil;
  [view setNeedsDisplay:YES];
}

-(void)submitNodeEditorView:(CPNotification)notification
{
  var request = [CPURLRequest requestWithURL:@"/api/v1/graph?submit&uuid="+[notification userInfo]];
  //[request setHTTPMethod:"POST"];
  var result = [CPURLConnection sendSynchronousRequest:request returningResponse:nil];
  CPLog.debug("Graph submit Result"+[result rawString]);
  [view clearElements];
  loadedUUID=nil;
  loadedName=nil;
  [view setNeedsDisplay:YES];


}
@end