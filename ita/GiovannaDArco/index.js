const express = require('express');
const bodyParser = require('body-parser');
const exphbs = require('express-handlebars');

const appRoute = require('./routes/app/app')

const app = express();

const PORT = process.env.PORT || 3000;

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.engine('handlebars', exphbs({default: 'main'}));
app.set('view engine', 'handlebars');

app.use('/', appRoute);
app.use('/', (req, res) => res.status(404).render('404'));

app.listen(PORT, () => {
  console.log(`Server started on port ${PORT}`);
});
