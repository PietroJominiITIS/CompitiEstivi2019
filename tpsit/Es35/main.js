$(() => {
    
    let update = () => {
        let m = Number($('#m').val());
        let f = Number($('#f').val());


        let mp = (m * 100) / (m + f);
        let fp = (f * 100) / (m + f);

        pie.data.datasets.forEach((dataset) => {
            dataset.data = [mp, fp];
        });

        pie.update(0);
    }

    let pie = new Chart('ch', {
        type: 'pie',
        data: {
          labels: ['Maschi', 'Femmine'],
          datasets: [{
            label: "Distribuzione della classe",
            backgroundColor: ['#03A9F4', '#E91E63'],
            data: [0, 0]
          }]
        },
        options: {
          title: {
            display: true,
            text: 'Distribuzionoe percentuale della classe'
          },
          animation: {
            duration: 0
          }
        }
    });

    update();

    $('.form-control').change(() => {


        update();

    });
    

});