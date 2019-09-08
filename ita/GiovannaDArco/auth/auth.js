const fs = require('fs');
const authPWD = './auth/auth.json';

module.exports = {
  password: JSON.parse(fs.readFileSync(authPWD)).passw,
  modifyPassword: (newPass) => {
    fs.writeFileSync(authPWD, JSON.stringify({
      passw: newPass
    }), err => {
      if (err) console.log(err)
    });
  },
  authenticate: (passw) => passw == JSON.parse(fs.readFileSync(authPWD)).passw
}
