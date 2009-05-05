

/**
 * 
 */
function form2json(element){
  var formList=$$(element,'select, input');
  var result='{';
  $each(formList,function(fe){
    if(fe.get('name')!=null&&fe.get('value')!=undefined)
      result+='"'+fe.get('name')+'":"'+fe.get('value')+'",';
  });
  result+='}';
  return result;
}