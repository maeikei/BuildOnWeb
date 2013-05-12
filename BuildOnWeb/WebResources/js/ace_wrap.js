var output = ace.edit("OutPutFrame");
output.setTheme("ace/theme/terminal");
output.getSession().setMode("ace/mode/text");
output.setReadOnly(true);
      
var editor = ace.edit("SourceView");
editor.setTheme("ace/theme/xcode");
editor.getSession().setMode("ace/mode/$BOW_TMPL_TYPE$");
editor.getSession().on('change', function(e) {
     var xmlHttpRequest = new XMLHttpRequest();
     xmlHttpRequest.onreadystatechange = function()
     {
         var READYSTATE_COMPLETED = 4;
         var HTTP_STATUS_OK = 200;
/*
         console.log(xmlHttpRequest.readyState);
         console.log(xmlHttpRequest.status);
*/
       if( xmlHttpRequest.readyState == READYSTATE_COMPLETED
               && xmlHttpRequest.status == HTTP_STATUS_OK )
       {
           output.setValue(xmlHttpRequest.responseText,1);
       }
   }
   xmlHttpRequest.open('POST','/EventAceOnChange/$BOW_TMPL_PATH$');
   xmlHttpRequest.setRequestHeader( 'Content-Type', 'application/x-www-form-urlencoded');
   xmlHttpRequest.send( editor.getValue() );
});
