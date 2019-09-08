$(() => {

    let update = () => {
        bar.data.datasets.forEach(dataset => {
            dataset.data = [$('#n1').val(), $('#n2').val(), $('#n3').val(), $('#n4').val()]
        });
        bar.update();
    }

    let bar = new Chart('ch', {
        type: 'bar',
        data: {
            labels: ['Nazione 1', 'Nazione 2', 'Nazione 3', 'Nazione 4'],
            datasets: [{
                label: 'Produzionoe di acciaio',
                data: [1, 34, -54, -198],
                backgrounColor: [
                    'rgba(233, 30, 99, 0.8)',
                    'rgba(3, 169, 244, 0.8)',
                    'rgba(255, 152, 0, 0.8)',
                    'rgba(0, 150, 136, 0.8)'
                ],
                borderColor: [
                    'rgba(233, 30, 99, 1)',
                    'rgba(3, 169, 244, 1)',
                    'rgba(255, 152, 0, 1)',
                    'rgba(0, 150, 136, 1)'
                ],
                borderWidth: 2
            }]
        }
    });

    update();

    $('.form-control').change(() => {
        update();
    });

});