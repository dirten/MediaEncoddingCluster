
$(document).ready(function () {
  $( "#date" ).datepicker();


  $('#tabs')
      .tabs()
      .addClass('ui-tabs-vertical ui-helper-clearfix');

});

function buildForm( formname, data ) {
    console.log(data);
    $.each( data.gui.groups, function( key, val ) {
      $('#'+formname).append('<fieldset id="'+val.id+'"><legend>'+val.title+'</legend></fieldset>');
    });

    $.each( data.gui.options, function( key, val ) {
        $('#'+val.group).append(buildControl(val));
    });
}

function buildControl(option){
    var result='<label for="'+option.id+'">'+option.title+'</label>';
    if(option.control.type==='ComboBox'){
        result+=buildComboBox(option)+'</br>';
    }else{
        result+=buildTextBox(option)+'</br>';
    }
    return result;
}

function buildTextBox(option){
    return '<input value="'+option.control.default+'" type="text" name="'+option.id+'" id="'+option.id+'" class="text ui-widget-content ui-corner-all">';
}

function buildComboBox(option){
    var result="";
    result+='<select name="'+option.id+'" id="'+option.id+'">';
    $.each( option.control.items, function( key, val ) {
               var selected=val.value===option.control.default?"selected":"";
        result+='<option '+selected+' value="'+val.value+'">'+val.key+'</option>';
    });
    result+="</select>";
    return result;
}
