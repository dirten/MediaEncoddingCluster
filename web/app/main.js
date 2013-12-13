
var GLOBALS = {};

$(document).ready(function () {
  $( "#date" ).datepicker();

  $('#tabs')
      .tabs()
      .addClass('ui-tabs-vertical ui-helper-clearfix');

      /*preload formats*/
      $.getJSON( "/api/v1/format", function( data ) {
        GLOBALS.formats=data;
      })

      /*preload encoder*/
      $.getJSON( "/api/v1/codec", function( data ) {
        GLOBALS.codecs=data;
      })
  });

function bindFormData( formname, data ){
    /*setting the values from existing entries*/
    $.each( data, function( key, val ) {
      var element=$('#'+formname+' >> #'+key);
      element.val(val);
    });

    /*add change listener to all input elements*/
    $.each( $('#'+formname+' :input'), function( key, val ) {

       var element=$('#'+formname+' >> #'+val.getAttribute("name"));

       /*this fills up the data with default values when creating a new data entry*/
       data[val.getAttribute("name")]=element.val();

       element.change(function() {
        data[val.getAttribute("name")]=element.val();
       });
    });
}

function buildForm( formname, data ) {
    $.each( data.gui.groups, function( key, val ) {
      $('div#'+formname).append('<fieldset id="'+val.id+'"><legend>'+val.title+'</legend></fieldset>');
    });

    $.each( data.gui.options, function( key, val ) {
        $('div#'+formname +'> fieldset#'+val.group).append(buildControl("",val));
    });
}

function buildControl(formname, option){
    var result='<label for="'+option.id+'">'+option.title+'</label>';
    if(option.control.type==='ComboBox'){
        result+=buildComboBox(formname,option)+'</br>';
    }else if(option.control.type==='FormatBox'){
        result+=buildFormatComboBox(formname,option)+'</br>';
    }else if(option.control.type==='VideoCodecBox'){
        result+=buildVideoComboBox(formname,option)+'</br>';
    }else if(option.control.type==='AudioCodecBox'){
        result+=buildAudioComboBox(formname,option)+'</br>';
    }else{
        result+=buildTextBox(formname,option)+'</br>';
    }
    return result;
}

function buildTextBox(formname,option){
    return '<input value="'+option.control.default+'" type="text" name="'+option.id+'" id="'+formname+option.id+'" class="text ui-widget-content ui-corner-all">';
}

function buildComboBox(formname,option){
    var result="";
    result+='<select name="'+option.id+'" id="'+formname+option.id+'">';
    $.each( option.control.items, function( key, val ) {
        var selected=val.value===option.control.default?"selected":"";
        result+='<option '+selected+' value="'+val.value+'">'+val.key+'</option>';
    });
    result+="</select>";
    return result;
}

function buildFormatComboBox(formname,option){
    var result="";
    result+='<select name="'+option.id+'" id="'+formname+option.id+'">';
    $.each( GLOBALS.formats.data, function( key, val ) {
        result+='<option value="'+val.id+'">'+val.longname+'</option>';
    });
    result+="</select>";
    return result;
}

function buildVideoComboBox(formname,option){
    var result="";
    result+='<select name="'+option.id+'" id="'+formname+option.id+'">';
    $.each( GLOBALS.codecs.data, function( key, val ) {
       if(val.type===0){
           result+='<option value="'+val.id+'">'+val.longname+'</option>';
       }
    });
    result+="</select>";
    return result;
}

function buildAudioComboBox(formname,option){
    var result="";
    result+='<select name="'+option.id+'" id="'+formname+option.id+'">';
    $.each( GLOBALS.codecs.data, function( key, val ) {
       if(val.type===1){
           result+='<option value="'+val.id+'">'+val.longname+'</option>';
       }
    });
    result+="</select>";
    return result;
}

