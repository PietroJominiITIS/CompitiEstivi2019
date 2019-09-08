$(() => {
    $('#sel').change(() => {
        $('#out').text($("#sel :selected").text());
    });
});