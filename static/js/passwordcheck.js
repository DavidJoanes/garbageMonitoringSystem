$(document).ready(function(){
    $('#confirmpassword').keyup( function(e){
        var password = $('#password').val();
        var confirmpassword = $('#confirmpassword').val();


        if(password != confirmpassword){

           $('#pass_response').html("Passwords do not match!").css({'color':'red', 'text-align':'right'});
           $('#button').prop('disabled', true);
        }else{
           $("#pass_response").html("");
           $('#button').removeAttr('disabled');
        }
    })
})