const express = require('express');
const uuidv4 = require('uuid/v4');
const fs = require('fs');
const nApp = require('../../lib/naturalApplication.js');
const auth = require('../../auth/auth.js');
const cookieParser = require('cookie-parser');

const router = express.Router();
router.use(cookieParser());

let blacklist = JSON.parse(fs.readFileSync('./data/blacklist.json')).blacklist;
let testi = JSON.parse(fs.readFileSync('./data/testi.json')).testi;

router.get('/', (req, res) => {
  res.redirect('/home');
});

router.get('/home', (req, res) => {
  res.render('home', {
    logged: auth.authenticate(req.cookies.passw),
    wordsAcc: nApp.parseAcc(testi, blacklist).slice(0, 15),
    wordsNonAcc: nApp.parseNonAcc(testi, blacklist).slice(0, 15)
  });
});

router.get('/info', (req, res) => {
  res.render('info', {
    logged: auth.authenticate(req.cookies.passw)
  });
});

router.get('/blacklist', (req, res) => {
  res.render('blacklist', {
    logged: auth.authenticate(req.cookies.passw),
    blacklist,
    nWords: blacklist.length,
    refocus: (req.query.focus) ? true : false
  });
});

router.post('/rmWord', (req, res) => {
  if (blacklist.indexOf(req.body.word) > -1 && auth.authenticate(req.cookies.passw)) blacklist.splice(blacklist.indexOf(req.body.word), 1);
  fs.writeFileSync('./data/blacklist.json', JSON.stringify({
    blacklist
  }), err => {
    if (err) console.log(err)
  });
  res.redirect('/blacklist');
});

router.post('/addWord', (req, res) => {
  if (blacklist.indexOf(req.body.word) <= -1 && auth.authenticate(req.cookies.passw)) blacklist.push(req.body.word);
  fs.writeFileSync('./data/blacklist.json', JSON.stringify({
    blacklist
  }), err => {
    if (err) console.log(err);
  });
  res.redirect('/blacklist?focus=true');
});

router.get('/aknowledgeWord', (req, res) => {
  res.render('aknowledgeWord', {
    testi: testi.filter(el => el.text.toLowerCase().includes(req.query.word)),
    word: req.query.word,
    logged: auth.authenticate(req.cookies.passw)
  });
});

router.get('/testi', (req, res) => {
  res.render('testi', {
    logged: auth.authenticate(req.cookies.passw),
    testi
  });
});

router.post('/addText', (req, res) => {
  if (testi.filter(el => el.id == req.body.id).length <= 0 && auth.authenticate(req.cookies.passw)) testi.push({
    title: req.body.title,
    id: uuidv4(),
    text: req.body.text,
    origin: req.body.origin,
    acc: ((req.body.acc == 'acc') ? true : false)
  });
  fs.writeFileSync('./data/testi.json', JSON.stringify({
    testi
  }));
  res.redirect('/testi');
});

router.post('/rmText', (req, res) => {
  testi = (auth.authenticate(req.cookies.passw)) ? testi.filter(el => el.id != req.body.id) : testi;
  fs.writeFileSync('./data/testi.json', JSON.stringify({
    testi
  }));
  res.redirect('/testi');
});

router.get('/testo', (req, res) => {
  res.render('testo', {
    testo: testi.filter(el => el.id == req.query.id)[0],
    logged: auth.authenticate(req.cookies.passw)
  });
});

router.get('/login', (req, res) => {
  let badRequest = false;
  if (req.query.badRequest) badRequest = true;
  res.render('login', {
    logged: auth.authenticate(req.cookies.passw),
    badRequest
  });
});

router.post('/login', (req, res) => {
  res.cookie('passw', req.body.passw);
  if (auth.authenticate(req.body.passw)) res.redirect('/login');
  else res.redirect('/login?badRequest=true');
});

router.get('/logout', (req, res) => {
  res.cookie('passw', '');
  res.render('login', {
    loggedOut: true
  });
});

router.get('/setting', (req, res) => {
  res.render('setting', {
    logged: auth.authenticate(req.cookies.passw),
    pChangedStatus: {
      somethignToExpect: (req.query.changePasswStatus) ? true : false,
      pChangedSuccessfully: (req.query.changePasswStatus != '-1') ? true : false
    }
  });
});

router.post('/changePassw', (req, res) => {
  let status;
  if (auth.authenticate(req.cookies.passw) && req.body.newPassw != '') {
    auth.modifyPassword(req.body.newPassw);
    res.cookie('passw', req.body.newPassw);
    status = 0;
  } else status = -1;
  res.redirect('setting?changePasswStatus=' + status);
});

module.exports = router;
