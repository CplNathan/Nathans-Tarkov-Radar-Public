angular.module("tarkovradar").controller("main", ['$scope', function ($scope) {
    let socketip = 'wss://example.com'

    let canvas = $("#radarCanvas")[0];
    let ctx = canvas.getContext("bitmaprenderer");

    $scope.RADAR_RADIUS = window.innerHeight > window.innerWidth ? window.innerWidth / 2 : window.innerHeight / 2;
    $scope.RADAR_SCALE = 10;
    $scope.FOCUS_PLAYER = "";
    $scope.TEAMS = [];
    $scope.LOOTFILTER = [];
    $scope.HIGHVAUEONLY = false;
    $scope.HIDECORPSES = false;

    $scope.loot = [];
    $scope.players = [];
    $scope.groups = [];
    $scope.exfils = [];

    canvas.width = $scope.RADAR_RADIUS * 2;
    canvas.height = $scope.RADAR_RADIUS * 2;

    let dummydata = { "host": false, "start": false, "players": [], "exfils": [], "groups": [], "token": "NOT-AUTHENTICATED" };

    let render_worker = new Worker('render_worker.js');

    let htmlBuffer = document.createElement('canvas', { alpha: false });
    let offscreenBuffer = htmlBuffer.transferControlToOffscreen();

    let htmlSprite = document.createElement('canvas', { alpha: false });
    let offscreenSprite = htmlSprite.transferControlToOffscreen();

    let htmlText = document.createElement('canvas', { alpha: false });
    let offscreenOverlay = htmlText.transferControlToOffscreen();
    render_worker.postMessage({ "event": "init", "canvas": offscreenBuffer, "sprite": offscreenSprite, "overlay": offscreenOverlay, "RADAR_RADIUS": $scope.RADAR_RADIUS, "RADAR_SCALE": $scope.RADAR_SCALE, "FOCUS_PLAYER": $scope.FOCUS_PLAYER, "TEAMS": $scope.TEAMS, "LOOTFILTER": $scope.LOOTFILTER, "HIGHVAUEONLY": $scope.HIGHVAUEONLY, "HIDECORPSES": $scope.HIDECORPSES }, [offscreenBuffer, offscreenSprite, offscreenOverlay]);

    let render_behind = 0;
    render_worker.addEventListener('message', function (e) {
        if (e.data.msg === 'render') {
            ctx.transferFromImageBitmap(e.data.bitmap);

            render_behind--;
        }
    });

    function handleTick(data) {
        if (render_behind > 0)
            return;

        $scope.players = data.players;
        $scope.groups = data.groups;
        $scope.exfils = data.exfils;
        data.loot = $scope.loot;

        render_worker.postMessage({ "event": "render", "message": data, "behind": render_behind });
        render_behind++;
    }

    function connect() {
        let socket = new WebSocket(socketip);

        handleTick(dummydata)

        socket.onmessage = function (event) {
            //socket.send("{ 'event': 'ack' }");

            try {
                event = JSON.parse(event.data);
                switch (event.event) {
                    case "tick":
                        handleTick(event.message)
                        break;
                    case "addloot":
                        $scope.loot.push(event.message.item);
                        break;
                    case "removeloot":
                        for (item in $scope.loot) {
                            if ($scope.loot[item].signature == event.message.item.signature) {
                                $scope.loot.splice(item, 1);
                                break;
                            }
                        }
                        break;
                    case "gamestart":
                        $scope.loot = [];
                        break;
                    case "gameend":
                        $scope.loot = [];
                        break;
                }
            }
            catch (err) {
                console.log("Invalid Message Format");
                console.log(err)
            }
        };

        socket.onclose = function (event) {
            handleTick(dummydata)
            setTimeout(function () { connect() }, 1000);
        };

        socket.onopen = function (event) {
            let passcode = prompt("Please enter the session passcode", "");
            socket.send("{\"event\":\"authenticate\",\"message\":{\"token\":\"" + passcode + "\"}}")
        };
    }

    $scope.changedValue = function () {
        render_worker.postMessage({ "event": "update", "RADAR_RADIUS": $scope.RADAR_RADIUS, "RADAR_SCALE": $scope.RADAR_SCALE, "FOCUS_PLAYER": $scope.FOCUS_PLAYER, "TEAMS": $scope.TEAMS, "LOOTFILTER": $scope.LOOTFILTER, "HIGHVAUEONLY": $scope.HIGHVAUEONLY, "HIDECORPSES": $scope.HIDECORPSES });
    }

    window.addEventListener('resize', resizeCanvas, false);
    function resizeCanvas() {
        $scope.RADAR_RADIUS = window.innerHeight > window.innerWidth ? window.innerWidth / 2 : window.innerHeight / 2;
        canvas.width = $scope.RADAR_RADIUS * 2;
        canvas.height = $scope.RADAR_RADIUS * 2;
        $scope.$apply()
        $scope.changedValue()
    }
    connect();
}]);