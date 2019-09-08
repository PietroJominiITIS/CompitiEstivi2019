$(() => {

  let vol = 0.5;

  $('#play').click(() => {
    $('#video')[0].play();
    $('span#status')[0].innerHTML = 'Playing';
  });

  $('#pause').click(() => {
    $('#video')[0].pause();
    $('span#status')[0].innerHTML = 'Paused';
  });

  $('#vplus').click(() => {
    vol += 0.05;
    if (vol > 1) vol = 1;
    $('#video')[0].volume = vol;
    $('span#vol')[0].innerHTML = Math.floor(vol * 100);
  });

  $('#vminus').click(() => {
    vol -= 0.05;
    if (vol < 0) vol = 0;
    $('#video')[0].volume = vol;
    $('span#vol')[0].innerHTML = Math.floor(vol * 100);
  });

  $('.goto').click((e) => {
    $('#video')[0].currentTime = e.target.getAttribute('data-to-add');
  });

});
