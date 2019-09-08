$(() => {

    $('.form-control').change(() => {
        $('#out').text($('#km').val()/$('#h').val());
    });

});