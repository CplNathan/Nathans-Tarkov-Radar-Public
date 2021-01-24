const io = require('socket.io')(8080, {
  cors: {
    origin: '*',
  }
});
const validator = require('./DataTypes');
const types = validator.dataTypes;

const passcode = "boop";
const tokenchars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

let session = {
  "token": randomString(4, tokenchars),
  "host": false,
  "start": false,
  "players": [],
  "groups": [],
  "loot": [],
  "exfils": []
}

io.on('connection', socket => {
  socket.on('authenticate', (data, ack) => {
    console.log("Client requesting authentication");
    // Validate the current data structure
    let expectedData = new types.RegisterObject().dataStructure;
    let validatedData = validator.validatePacket(data, expectedData);

    let validated = validatePasscode(data.Passcode);
    if (validated) {
      socket["radarData"] = validatedData;
      for (item in session["loot"]) {
        socket.emit("addloot", session["loot"][item]);
      }
      if (socket["radarData"].isHost == true) {
        let sockets = io.sockets.sockets; // All Sockets

        // Disconnect any other hosts
        var isocket = 0;
        for (isocket of sockets) {
          let currentSocket = isocket[1];

          if (currentSocket["radarData"] == undefined)
            continue;

          if (currentSocket["radarData"].isHost && currentSocket.id != socket.id) {
            console.log("Kicking host with ID: {currentSocket.id}")
            currentSocket.disconnect();
          }
        }

        session["host"] = true;
        session["token"] = randomString(4, tokenchars);

        console.log("Host Connected");
      }
    }

    ack(new types.RegisterResponse(validated).rawData);
  });

  socket.on('disconnect', data => {
    if (socket["radarData"] == undefined)
      return;

    if (socket["radarData"].isHost == true) {
      session["host"] = false;

      console.log("Host Disconnected");
    }
  })

  socket.on('gamestart', data => {
    session["players"] = [];
    session["groups"] = [];
    session["loot"] = [];
    session["exfils"] = [];
    session["start"] = true;

    console.log("Game Started");
  });

  socket.on('gameend', data => {
    session["players"] = [];
    session["groups"] = [];
    session["loot"] = [];
    session["exfils"] = [];
    session["start"] = false;

    console.log("Game Ended");
  });

  // Player data is not cached and is volatile, send out an update.
  socket.on('newplayer', data => {
    try {
      // Validate the current data structure
      let expectedData = new types.NewPlayer().dataStructure;
      let validatedData = validator.validatePacket(data, expectedData);

      // Ensure message is from a host
      if (!validateHost(socket, data)) return;

      // Add player to players
      session["players"].push(validatedData);

      // Find our add group and add player
      if (validatedData.groupid != "") {
        let groupIndex = -1;
        for (group in session["groups"]) {
          if (session["groups"][group].id == validatedData.groupid) {
            groupIndex = group;
            break;
          }
        }

        if (groupIndex == -1) {
          session["groups"].push(
            {
              "id": validatedData.groupid,
              "members": [validatedData.id]
            }
          )
        }
        else {
          session["groups"][group].members.push(validatedData)
        }
      }
    } catch (err) {
      console.log(err)
    }
  });

  // Player data is not cached and is volatile, send out an update.
  socket.on('updateplayer', data => {
    try {
      // Validate the current data structure
      let expectedData = new types.UpdatePlayer().dataStructure;
      let validatedData = validator.validatePacket(data, expectedData);

      // Ensure message is from a host
      if (!validateHost(socket, data)) return;

      // Update the player at the found index
      for (player in session["players"]) {
        if (session["players"][player].id == validatedData.id) {
          session["players"][player].coordinates = validatedData.coordinates;
          session["players"][player].viewangle = validatedData.viewangle;
          session["players"][player].weapon = validatedData.weapon;
          break;
        }
      }
    } catch (err) {
      console.log(err)
    }
  });

  // Player data is not cached and is volatile, send out an update.
  socket.on('deleteplayer', data => {
    try {
      // Validate the current data structure
      let expectedData = new types.DeletePlayer().dataStructure;
      let validatedData = validator.validatePacket(data, expectedData);

      // Ensure message is from a host
      if (!validateHost(socket, data)) return;

      let removeIndex = -1;

      // Delete the player at the found index
      for (player in session["players"]) {
        if (session["players"][player].id == data.id) {
          removeIndex = player;
          break;
        }
      }

      if (removeIndex != -1) {
        session["players"].splice(player, 1);
      }
    } catch (err) {
      console.log(err)
    }
  });

  // Loot data is cached
  socket.on('addloot', data => {
    try {
      // Validate the current data structure
      let expectedData = new types.AddLoot().dataStructure;
      let validatedData = validator.validatePacket(data, expectedData);

      // Ensure message is from a host
      if (!validateHost(socket, data)) return;

      let sockets = io.sockets.sockets; // All Sockets

      var isocket = 0;
      for (isocket of sockets) {
        let currentSocket = isocket[1];
        currentSocket.emit("addloot", validatedData);
      }

      session["loot"].push(validatedData);
    } catch (err) {
      console.log(err)
    }
  });

  // Loot data is cached
  socket.on('deleteloot', data => {
    try {
      // Validate the current data structure
      let expectedData = new types.DeleteLoot().dataStructure;
      let validatedData = validator.validatePacket(data, expectedData);

      // Ensure message is from a host
      if (!validateHost(socket, data)) return;

      let removeIndex = -1;

      for (item in session["loot"]) {
        if (session["loot"][item].signature == validatedData.signature) {
          removeIndex = item;
          break;
        }
      }

      if (removeIndex != -1) {
        let sockets = io.sockets.sockets; // All Sockets

        var isocket = 0;
        for (isocket of sockets) {
          let currentSocket = isocket[1];
          if (!currentSocket["radarData"].isHost) {
            currentSocket.emit("addloot", session["loot"][removeIndex]);
          }
        }

        session["loot"].splice(removeIndex, 1);
      }
    }
    catch (err) {
      console.log(err)
    }
  });

  socket.on('addexfil', data => {
    try {
      // Validate the current data structure
      let expectedData = new types.AddExfil().dataStructure;
      let validatedData = validator.validatePacket(data, expectedData);

      // Ensure message is from a host
      if (!validateHost(socket, data)) return;

      session["exfils"].push(validatedData);
    }
    catch (err) {
      console.log(err)
    }
  });

  socket.on('updateexfil', data => {
    try {
      // Validate the current data structure
      let expectedData = new types.UpdateExfil().dataStructure;
      let validatedData = validator.validatePacket(data, expectedData);

      // Ensure message is from a host
      if (!validateHost(socket, data)) return;

      for (exfil in session["exfils"]) {
        if (session["exfils"][exfil].id == validatedData.id) {
          session["exfils"][exfil].status = validatedData.status;
        }
      }
    }
    catch (err) {
      console.log(err)
    }
  });
});

function randomString(length, chars) {
  let result = '';
  for (let i = length; i > 0; --i) result += chars[Math.floor(Math.random() * chars.length)];
  return result;
}

function validatePasscode(input) {
  return (input === passcode) || input == session["token"];
}

function validateHost(socket, data) {
  if (socket["radarData"] == undefined) {
    return false;
  }

  let host = socket["radarData"].isHost;

  if (!host) {
    socket.disconnect();
    //socket.emit(new types.RegisterResponse(validated).rawData);
  }

  return host;
}

function TickVolatileData() {
  let sockets = io.sockets.sockets; // All Sockets

  let datatransmit = session;
  datatransmit["loot"] = [];

  var isocket = 0;
  for (isocket of sockets) {
    let currentSocket = isocket[1];

    currentSocket.emit("tick", datatransmit);
  }
}

setInterval(TickVolatileData, 25);