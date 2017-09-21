process.env.NODE_URL="192.168.0.5"; //locate your own ip address
process.env.NODE_PORT="6085";

var counter = 0;
require('mahrio').runServer( process.env, __dirname).then( function(server){
  server.route({
    method: 'POST',
    path: '/api/monitor',
    handler: function( req, rep){
      console.log( ""+(counter++)+": "+req.payload );
      rep('OK');
    }
  })
});
