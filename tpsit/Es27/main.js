$(() => {

  $('button#ellisse').click(() => {
    $('#svg').html(`<ellipse cx="350" cy="350" rx="300" ry="250"/>`);
  });

  $('button#rettangolo').click(() => {
    $('#svg').html(`<rect x="50" y="250" width="600" height="200"/>`);
  });

  $('button#esagono').click(() => {
    $('#svg').html(`<polygon points="500,350 425,480 275,480 200,350 275,220 425,220"></polygon>`);
  });

  $('button#quadrato').click(() => {
    $('#svg').html(`<rect x="50" y="50" width="600" height="600"/>`);
  });

});
