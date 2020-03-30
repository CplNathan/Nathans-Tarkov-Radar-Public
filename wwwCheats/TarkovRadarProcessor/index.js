const WebSocket = require('ws');

const MASTER = "owo";

const wss = new WebSocket.Server({ port: 8080 });

let players = [];
let groups = [];
let loot = [];
let exfils = [];
let gamestarted = false;
let hostconnected = false;

let sessiontoken = randomString(6, '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ');

let hostkeepalive;

function formSocketMessage(event, message) {
  let stringify = JSON.stringify({
    "event": event,
    "message": message
  });

  return stringify;
}

function resetData(newSession) {
  players = [];
  loot = [];
  exfils = [];
  gamestarted = newSession;

  if (!newSession)
    sessiontoken = randomString(6, '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ');
}

function terminatehost(wss) {
  console.log("Terminating old hosts");
  wss.clients.forEach(function each(client) {
    if (client["host"] == true)
      client.terminate();
  });

  hostconnected = false;

  resetData(false);
}

function randomString(length, chars) {
  let result = '';
  for (let i = length; i > 0; --i) result += chars[Math.floor(Math.random() * chars.length)];
  return result;
}

function decodeSocketMessage(event, wss, ws) {
  try {
    let host = ws["host"] === undefined ? false : ws["host"];
    event = JSON.parse(event);
    let data = event.message;
    switch (event.event) {
      case "keepalive":
        if (!host) return;
        clearTimeout(hostkeepalive);
        hostkeepalive = setTimeout(function () {
          terminatehost(wss);
        }, 30000);
        break;
      case "gamestart":
        if (!host) return;
        console.log("Game Starting");
        wss.clients.forEach(function each(client) {
          if (client["host"] == true || client["authenticated"] == false) return;
          client.send(formSocketMessage("gamestart"));
        });
        resetData(true);
        break;
      case "gameend":
        if (!host) return;
        console.log("Game Ending");
        wss.clients.forEach(function each(client) {
          if (client["host"] == true || client["authenticated"] == false) return;
          client.send(formSocketMessage("gameend"));
        });
        resetData(false);
        break;
      case "newplayer":
        if (!host) return;
        players.push(data);
        if (data.groupid != "") {
          let foundgroup = false;
          for (group in groups) {
            if (groups[group].id == data.groupid) {
              groups[group].members.push(data)
              foundgroup = true;
              break;
            }
          }
          if (!foundgroup) {
            groups.push(
              {
                "id": data.groupid,
                "members": [data.id]
              }
            )
          }
        }
        break;
      case "updateplayer":
        if (!host) return;
        for (player in players) {
          if (players[player].id == data.id) {
            players[player].coordinates = data.coordinates;
            players[player].viewangle = data.viewangle;
            players[player].weapon = data.weapon;
            break;
          }
        }
        break;
      case "deleteplayer":
        if (!host) return;
        for (player in players) {
          if (players[player].id == data.id) {
            console.log("Deleting player: " + players[player].id);
            players.splice(player, 1);
            break;
          }
        }
        break;
      case "addloot":
        if (!host) return;
        wss.clients.forEach(function each(client) {
          if (client["host"] == true || client["authenticated"] == false) return;
          client.send(formSocketMessage("addloot", { "item": data }));
        });
        loot.push(data);
        break;
      case "deleteloot":
        if (!host) return;
        console.log("Deleting Loot: " + data.signature);
        for (item in loot) {
          if (loot[item].signature == data.signature) {
            wss.clients.forEach(function each(client) {
              if (client["host"] == true || client["authenticated"] == false) return;
              client.send(formSocketMessage("removeloot", { "item": loot[item] }));
            });
            loot.splice(item, 1);
            break;
          }
        }
        break;
      case "addexfil":
        if (!host) return;
        exfils.push(data);
        break;
      case "updateexfil":
        if (!host) return;
        for (exfil in exfils) {
          if (exfils[exfil].id == data.id) {
            exfils[exfil].status = data.status;
          }
        }
        break;
      case "host":
        terminatehost(wss);

        clearTimeout(hostkeepalive);
        hostkeepalive = setTimeout(function () {
          terminatehost(wss);
        }, 30000);

        hostconnected = true;
        ws["host"] = true;
        break;
      case "authenticate":
        if (data.token == sessiontoken || data.token == MASTER) {
          ws["authenticated"] = true;
        }
        else {
          ws.terminate();
        }
        break;
    }

  }
  catch (err) {
    console.log("Invalid Message Format");
    console.log(err); // todo comment
  }

  return event.message;
}

wss.on('connection', function connection(ws) {
  ws["host"] = false;
  ws["authenticated"] = false;
  for (item in loot) {
    ws.send(formSocketMessage("addloot", { "item": loot[item] }));
  }
  ws.on('message', function incoming(message) {
    decodeSocketMessage(message, wss, ws);
  });
});

setInterval(function () {
  tick();
}, 25);

function tick() {
  wss.clients.forEach(function each(client) {
    if (client["host"] == true || client["authenticated"] == false) return;
    client.send(formSocketMessage("tick", { "host": hostconnected, "start": gamestarted, "players": players, "groups": groups, "exfils": exfils, "token": sessiontoken }));
  });
};