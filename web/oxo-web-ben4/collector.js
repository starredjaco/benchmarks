const express = require('express');
const app = express();

app.get('/collect', (req, res) => {
  console.log('COLLECTOR RECEIVED:', req.query);
  res.sendStatus(200);
});

const port = 9000;
app.listen(port, () => console.log('Collector listening on http://localhost:' + port));
