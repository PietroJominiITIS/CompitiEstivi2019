$(() => {

    $('#fout').hide();

    $('#m').on('change.bootstrapSwitch', () => {
        $('#fout').hide();
        $('#mout').show();
    });

    $('#f').on('change.bootstrapSwitch', () => {
        $('#mout').hide();
        $('#fout').show();
    });

});