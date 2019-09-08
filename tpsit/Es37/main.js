$(() => {

    $('.form-control').change(() => {

        let p = Number($('#p').val()), h = Number($('#a').val());
        let bmi = p/(h*h);
        
        let out;
        if (bmi < 18) out = 'Basso';
        else if (bmi < 25) out = 'Corretto';
        else out = 'Alto';
        
        $('#out').text(out);

    });

});