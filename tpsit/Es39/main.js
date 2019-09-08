$(() => {

    let tot = 0;

    $('#confirm').click(() => {
        let v = Number($('#price').val());
        tot += v;
        $('#totale').val(tot);
    });

});