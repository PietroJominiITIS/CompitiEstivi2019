const natural = require('natural');
natural.PorterStemmerIt.attach();
module.exports = {
  parseAcc: (testi, blacklist) => {
    let wordsAcc = testi.reduce((all, curr) => {
      return (curr.acc) ? all.concat(curr.text.toLowerCase().replace(/[|&;$%@"<>()+,.#*\[\]'~`]/g, '').split(' ')) : all; // curr.text.tokenizeAndStem()
    }, []).reduce((all, curr) => {
      if (blacklist.indexOf(curr) <= -1) {
        if (curr in all)  all[curr]++;
        else all[curr] = 1;
      }
      return all;
    }, {});

    return Object.keys(wordsAcc).sort((a, b) => wordsAcc[b] - wordsAcc[a]).map(el => {
      return {
        name: el,
        count: wordsAcc[el]
      }
    });;
  },
  parseNonAcc: (testi, blacklist) => {
    let wordsAcc = testi.reduce((all, curr) => {
      return (!curr.acc) ? all.concat(curr.text.toLowerCase().replace(/[|&;$%@"<>()+,.#*\[\]'~`]/g, '').split(' ')) : all;
    }, []).reduce((all, curr) => {
      if (blacklist.indexOf(curr) <= -1) {
        if (curr in all)  all[curr]++;
        else all[curr] = 1;
      }
      return all;
    }, {});

    return Object.keys(wordsAcc).sort((a, b) => wordsAcc[b] - wordsAcc[a]).map(el => {
      return {
        name: el,
        count: wordsAcc[el]
      }
    });;
  }
};
