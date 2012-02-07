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
@implementation NodeActivityController : CPObject
{
  NodeEditorView view;
  CPDictionary elementClasses;
  CPDictionary elements;
  id loadedUUID;
  id loadedJobID;
  id alldata  @accessors(property=data);
  CPString loadedName;
  id growl;
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
  elements=[CPDictionary dictionary];

  growl=[TNGrowlCenter defaultCenter];

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
  if([view hasUnsavedChanges]){
    var alert=[[YesNoAlert alloc] 
                initWithLabel:@"Current Graph has unsaved changes!" 
                question:@"Would you proceed anyway, this will discard all changes!" 
                yesLabel:@"proceed without saving" 
                noLabel:@"Cancel" 
                target:self 
                yesAction:@selector(_load:)
                yesObject:notification
                noAction:nil
                noObject:nil];
  }else{
    [self _load:notification];
  }
}

-(void)_load:(CPNotification)notification
{
  CPLog.debug("_load");
  CPLog.debug("loadNodeEditorView:"+[notification userInfo]);
  //var data={"tasks":[{"name":"Input","uid":1},{"name":"Encoding","uid":2},{"name":"Encoding","uid":3},{"name":"Output","uid":4}],"links":[{"uid":1,"linksTo":2},{"uid":1,"linksTo":3},{"uid":2,"linksTo":4},{"uid":3,"linksTo":4}],"positions":[{"uid":1,"x":581,"y":47},{"uid":2,"x":858,"y":42},{"uid":3,"x":1070,"y":361},{"uid":4,"x":1130,"y":90}]};
  //var path="/api/v1/encoding/"+[notification userInfo];
  //var response=[CPHTTPURLResponse alloc];
  //var error;
  //var raw_data = [CPURLConnection sendSynchronousRequest:[CPURLRequest requestWithURL:path] returningResponse:response];
  var result=[[MHiveApiController sharedController] viewEncoding:[notification userInfo]];
  //CPLog.debug("raw_data:"+[raw_data rawString]);
  //CPLog.debug("json_data:"+[raw_data JSONObject]);
  
  data=result.data;//[raw_data JSONObject].data;
  //var data=alldata.flow;
  if(data==undefined||data.flow==undefined){
    [growl pushNotificationWithTitle:@"Failed to load the Flow?" message:"The Server returns an unknown Flow Format!"];
     return;
  }
  var flow=data.flow
  if(loadedJobID!=[notification userInfo]){
    loadedJobID=[notification userInfo];
    loadedUUID=flow.uuid;
    loadedName=flow.name;
    [view clearElements];
    elements=[CPDictionary dictionary];
    for(a=0;a<flow.tasks.length;a++){
      var task=flow.tasks[a];
      var taskdata=undefined;
      if(task.data)
        taskdata={data:task.data};

      CPLog.debug("TaskData:"+JSON.stringify(taskdata));
      var obj=[[[elementClass objectForKey:task.name] alloc] init];
      if(obj){
        [obj setUid:task.uid];
        [obj setBoundsOrigin:CPPointMake(task.position.x,task.position.y)];
        if(false&&alldata.graphstatus!=undefined&&alldata.graphstatus[task.uid]!=undefined){
          CPLog.debug("task.uid"+task.uid);
          [obj setProgress:alldata.graphstatus[task.uid].progress];
          if(alldata.graphstatus[task.uid].message!=undefined&&
              alldata.graphstatus[task.uid].message.length)
            [obj setMessage:alldata.graphstatus[task.uid].message];
          if(alldata.graphstatus[task.uid].exception!=undefined&&
             alldata.graphstatus[task.uid].exception.length)
            [obj setMessage:alldata.graphstatus[task.uid].exception];
          [obj setStatus:alldata.graphstatus[task.uid].status];
        }else{
          [obj setProgress:@" "];      
        }
        if(task.linksTo){
          for(b=0;b<task.linksTo.length;b++){
            [obj addTarget:task.linksTo[b]];
          }
        }

        if(taskdata!=undefined)
          [obj setData:[CPDictionary dictionaryWithJSObject:taskdata recursively:YES]];
        [[view elements] addObject:obj];
        [elements setObject:obj forKey:task.uid];
      }else{
        CPLog.error("Element "+task.name+" not found!");
      }
      //[view setNeedsDisplay:YES];
      //CPLog.debug("Data:"+obj);
    }
    [view setName:loadedName];
  }else{
    //[[[elements objectEnumerator] allObjects] makeObjectsPerformSelector:@selector(setProgress:) withObject:@" "];
    //[[[elements objectEnumerator] allObjects] makeObjectsPerformSelector:@selector(setMessage:) withObject:@" "];
    //[[[elements objectEnumerator] allObjects] makeObjectsPerformSelector:@selector(setStatus:) withObject:-1];
    for(a=0;a<data.status.tasks.length;a++){
      var status=data.status.tasks[a];
      var obj=[elements objectForKey:status.uid];
      if(obj){
        [obj setProgress:status.progress];
        [obj setMessage:status.message];
        if(status.exception.length)
          [obj setMessage:status.exception];
        [obj setStatus:status.status];
      }else{
        CPLog.error("Object for status with id "+status.uid+" not found");
      }
      //CPLog.debug("Object = "+obj);
      if(false&&alldata.graphstatus!=undefined&&alldata.graphstatus[task.uid]!=undefined){
        CPLog.debug("task.uid"+task.uid+" set progress="+alldata.graphstatus[task.uid].progress);
        
        [obj setProgress:alldata.graphstatus[task.uid].progress];
        if(alldata.graphstatus[task.uid].message!=undefined&&
            alldata.graphstatus[task.uid].message.length)
          [obj setMessage:alldata.graphstatus[task.uid].message];
        if(alldata.graphstatus[task.uid].exception!=undefined&&
           alldata.graphstatus[task.uid].exception.length)
          [obj setMessage:alldata.graphstatus[task.uid].exception];
        [obj setStatus:alldata.graphstatus[task.uid].status];
      }else{
        //[obj setProgress:@" "];      
      }
      //[obj setNeedsDisplay:YES];
    }
  }
  //CPLog.debug("Obj:"+data.tasks.length);
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
  var mdata=[result JSONObject];
  CPLog.debug("Graph submit Result JSON:"+mdata);

  if(mdata.status=="ok"){
    [growl pushNotificationWithTitle:@"Graph submitted" message:"Graph "+loadedName+" successful submitted for execution!"];
  }else{
     var stopWarn = [[CPAlert alloc] init];
     [stopWarn setTitle:"Failed to submit the Graph?"];
     [stopWarn setMessageText:mdata.message];
     [stopWarn setAlertStyle:CPWarningAlertStyle];
     [stopWarn addButtonWithTitle:"Close"];
     [stopWarn runModal];

  }
  //[view clearElements];
  //loadedUUID=nil;
  //loadedName=nil;
  //[view setNeedsDisplay:YES];


}
@end