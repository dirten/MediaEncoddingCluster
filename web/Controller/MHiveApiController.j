var SharedController;

@implementation MHiveApiController : CPObject
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

-(id)makerequest:(CPString)url method:(CPString)method
{
  return [self makerequest:url method:method json:undefined];
}

-(id)makerequest:(CPString)url method:(CPString)method json:(id)json
{   var request = [CPURLRequest requestWithURL:url];
    [request setHTTPMethod:method];
    //[request setValue:@"application/x-www-form-urlencoded; charset=ISO-8859-1" forHTTPHeaderField:@"Content-Type"];
    //[request setValue:@"application/x-www-form-urlencoded; charset=UTF-8" forHTTPHeaderField:@"Content-Type"];
    [request setValue:@"application/json; charset=UTF-8" forHTTPHeaderField:@"Content-Type"];
    if(json!=undefined){
    CPLog.debug("request payload:"+[CPString JSONFromObject:json]);
      [request setHTTPBody:[CPString JSONFromObject:json]];
    }
    var req = new CFHTTPRequest();
    req.open([request HTTPMethod], [[request URL] absoluteString], NO);

    var fields = [request allHTTPHeaderFields],
    key = nil,
    keys = [fields keyEnumerator];

    while (key = [keys nextObject])
    req.setRequestHeader(key, [fields objectForKey:key]);

    req.send([request HTTPBody]);
    CPLog.debug("Request Status="+req.status());
    if(req.status()!=200 && req.status()!=201){
      throw "Unable to complete your request! "+req.status()+"-"+req.statusText();
    }
    var txt = [CPData dataWithRawString:req.responseText()];
    
    var data={
      "response":{
        "status":"error",
        "message":"Connection to server Lost!"
      }
    }
    if([txt rawString].length>0){
      data=[[txt rawString] objectFromJSON];
    }
    CPLog.debug("response payload:"+JSON.stringify(data));
    return data
}

-(id)codecs
{
  var data = [self makerequest:@"/api/v1/codec" method:@"GET"];
  return data;
}

-(id)formats
{
  var data = [self makerequest:@"/api/v1/format" method:@"GET"];
  return data;
}

-(id)profiles
{
  var data = [self makerequest:@"/api/v1/profile" method:@"GET"];
  return data;
}

-(id)createProfile:(id)profile
{
  var data = [self makerequest:@"/api/v1/profile" method:@"POST" json:profile];
  return data;
}

-(id)updateProfile:(id)json uuid:(id)uuid
{
  var data = [self makerequest:@"/api/v1/profile/"+uuid method:@"POST" json:json];
  return data;
}

-(id)viewProfile:(id)profile
{
  var data = [self makerequest:@"/api/v1/profile/"+profile method:@"GET"];
  return data;
}

-(id)deleteProfile:(id)profile
{
  var data = [self makerequest:@"/api/v1/profile/"+profile method:@"DELETE"];
  return data;
}

-(id)flows
{
  var data = [self makerequest:@"/api/v1/flow" method:@"GET"];
  return data;
}

-(id)viewFlow:(id)flowid
{
  var data = [self makerequest:@"/api/v1/flow/"+flowid method:@"GET"];
  return data;
}

-(id)createFlow:(id)flow
{
  var data = [self makerequest:@"/api/v1/flow" method:@"POST" json:flow];
  return data;
}

-(id)submitFlow:(id)uuid
{
  var data = [self makerequest:@"/api/v1/flow/"+uuid+"/submit" method:@"POST"];
  return data;
}

-(id)updateFlow:(id)flow uuid:(id)uuid
{
  var data = [self makerequest:@"/api/v1/flow/"+uuid method:@"POST" json:flow];
  return data;
}

-(id)deleteFlow:(id)flowid
{
  var data = [self makerequest:@"/api/v1/flow/"+flowid method:@"DELETE"];
  return data;
}

-(id)viewEncoding:(id)flowid
{
  var data = [self makerequest:@"/api/v1/encoding/"+flowid method:@"GET"];
  return data;
}

-(id)viewEncodingStatus:(id)flowid
{
  var data = [self makerequest:@"/api/v1/encoding/"+flowid+"/status" method:@"GET"];
  return data;
}

-(id)restartEncoding:(id)flowid
{
  var data = [self makerequest:@"/api/v1/encoding/"+flowid+"/restart" method:@"POST"];
  return data;
}

-(id)stopEncoding:(id)flowid
{
  var data = [self makerequest:@"/api/v1/encoding/"+flowid+"/stop" method:@"POST"];
  return data;
}

@end
