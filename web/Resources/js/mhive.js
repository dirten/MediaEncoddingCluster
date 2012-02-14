//var XMLHttpRequest = require("browser/xhr").XMLHttpRequest;
//require("browserjs/xhr").XMLHttpRequest 


function connect(){
  console.log("connecting to host "+this.host+":"+this.port);
}

function getXMLHttpRequest() 
{
  if (window.XMLHttpRequest) {
    return new window.XMLHttpRequest;
  }
  else {
    try {
      return new ActiveXObject("MSXML2.XMLHTTP.3.0");
    }
    catch(ex) {
      return null;
    }
  }
}
function request(method, url, postdata){
  var xmlhttp=getXMLHttpRequest();
  var poststr;
  console.log("xmlhttp:"+xmlhttp);
  xmlhttp.open(method,"http://"+this.host+":"+this.port+url, false);
  if(postdata){
    poststr=JSON.stringify(postdata);
    console.log("data-length"+poststr.length);
    xmlhttp.setRequestHeader("Content-length", poststr.length);
  //xmlhttp.setRequestHeader("Connection", "close");
  }
  console.log("data-length"+poststr);
  xmlhttp.send(poststr);
  console.log(xmlhttp.statusText);
  data=xmlhttp.responseText;
  console.log("Data:"+data);
  if(data.length)
    return eval('(' + data + ')');
  else
    return undefined;
}

function getCodecList(){
  return this.request("GET","/api/v1/codec");
}

function getFormatList(){
  return this.request("GET","/api/v1/format");
}

function getProfileList(){
  return this.request("GET","/api/v1/profile");
}

function createProfile(newProfile){
  return this.request("POST","/api/v1/profile", newProfile);
}

function updateProfile(uuid,profile){
  return this.request("POST","/api/v1/profile/"+uuid, profile);
}

function deleteProfile(uuid){
  return this.request("DELETE","/api/v1/profile/"+uuid);
}

function getProfile(uuid){
  return this.request("GET","/api/v1/profile/"+uuid);
}

function getFlowList(){
  return this.request("GET","/api/v1/flow");
}

function MHive(host, port, username, password){
  this.host=host;
  this.port=port;
  this.connect=connect;
  this.request=request;
  this.getCodecList=getCodecList;
  this.getFormatList=getFormatList;
  this.getProfileList=getProfileList;
  this.getFlowList=getFlowList;
  this.createProfile=createProfile;
  this.updateProfile=updateProfile;
}

function testFunction(){
  print();
}

function test(){
  var mhive=new MHive("10.122.6.90", 4000);
  mhive.connect();
  mhive.createProfile({
    data:"test"
  });
  mhive.getProfileList();
}
