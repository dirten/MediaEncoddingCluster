

/**
 * 
 */
function form2json(element){
  var formList=$$(element,'select, input');
  var result='{';
  $each(formList,function(fe){
    if(fe.get('name')!=null&&fe.get('value')!=undefined){
      if(fe.get('type')=='radio'){
        if(fe.get('checked')==true)
          result+='"'+fe.get('name')+'":"'+fe.get('value')+'",';
      }
      else
      if(fe.get('type')=='checkbox'){
        if(fe.get('checked')==true){
          result+='"'+fe.get('name')+'":"true",';
        } else{
          result+='"'+fe.get('name')+'":"false",';
        }
      }else
      result+='"'+fe.get('name')+'":"'+fe.get('value')+'",';
    }
  });
  result+='}';
  return result;
}