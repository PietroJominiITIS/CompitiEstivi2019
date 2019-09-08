$(() => {
  for(let i = 1; i < 6; i++) {
    let w = 300, h = 300, x = Math.floor(Math.random() * (700 - w)), y = Math.floor(Math.random() * (700 - h)), r = Math.floor(Math.random()*256), g = Math.floor(Math.random()*256), b = Math.floor(Math.random()*256);
    $('#svg').append(`<rect x="${x}" y="${y}" width="${w}" height="${h}" style="fill:rgb(${r},${g},${b}); stroke-width:0;"></rect>`);
  }

  // had to reload the page due to DOM
  $("body").html($("body").html());
});
