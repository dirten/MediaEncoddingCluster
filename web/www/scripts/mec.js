
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
function inspect(obj, maxLevels, level)
{
  var str = '', type, msg;

    // Start Input Validations
    // Don't touch, we start iterating at level zero
    if(level == null)  level = 0;

    // At least you want to show the first level
    if(maxLevels == null) maxLevels = 1;
    if(maxLevels < 1)     
        return '<font color="red">Error: Levels number must be > 0</font>';

    // We start with a non null object
    if(obj == null)
    return '<font color="red">Error: Object <b>NULL</b></font>';
    // End Input Validations

    // Each Iteration must be indented
    str += '<ul>';

    // Start iterations for all objects in obj
    for(property in obj)
    {
      try
      {
          // Show "property" and "type property"
          type =  typeof(obj[property]);
          str += '<li>(' + type + ') ' + property + 
                 ( (obj[property]==null)?(': <b>null</b>'):('')) + '</li>\n';

          // We keep iterating if this property is an Object, non null
          // and we are inside the required number of levels
          if((type == 'object') && (obj[property] != null) && (level+1 < maxLevels))
          str += inspect(obj[property], maxLevels, level+1);
      }
      catch(err)
      {
        // Is there some properties in obj we can't access? Print it red.
        if(typeof(err) == 'string') msg = err;
        else if(err.message)        msg = err.message;
        else if(err.description)    msg = err.description;
        else                        msg = 'Unknown';

        str += '<li><font color="red">(Error) ' + property + ': ' + msg +'</font></li>\n';
      }
    }

      // Close indent
      str += '</ul>';

    return str;
}

function testmyAlert(){
    //alert("here is my form alert");
    return true;
}

function setFormData(formname, data){
  var formList=$$(formname,'select, input, textarea');
  $each(formList,function(fe){
    if(fe.get('name')!=null){
	fe.set('value',data[fe.get('name')]);
    }
    if(fe.get('type')=='checkbox'){
        fe.set('checked', data[fe.get('name')]==true);
    }


  });
}

function setFormChangeListener(form, listener){
  var formList=$$(form,'select, input, textarea, radio');
  $each(formList,function(fe){
    if(fe.get('name')!=null){
	$(fe.get('name')).addEvent('change', listener);
    }
  });
}

