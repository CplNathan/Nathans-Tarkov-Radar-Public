angular.module("tarkovradar").controller("main", ['$scope', function ($scope) {
    let oldurl = 'http://' + window.location.hostname + ':8080'

    let canvas = $("#radarCanvas")[0];
    let ctx = canvas.getContext("bitmaprenderer");

    $scope.RADAR_RADIUS = window.innerHeight > window.innerWidth ? window.innerWidth / 2 : window.innerHeight / 2;
    $scope.RADAR_SCALE = 10;
    $scope.FOCUS_PLAYER = "";
    $scope.TEAMS = [];
    $scope.LOOTFILTER = [];
    $scope.FOCUSONLY = false;
    $scope.HIDECORPSES = false;

    canvas.width = $scope.RADAR_RADIUS * 2;
    canvas.height = $scope.RADAR_RADIUS * 2;

    const dummydata = { "host": false, "start": false, "players": [], "loot": [], "exfils": [], "groups": [], "token": "NOT-AUTHENTICATED" };
    $scope.data = dummydata;

    let render_worker = new Worker('render_worker.js');

    let htmlBuffer = document.createElement('canvas', { alpha: false });
    let offscreenBuffer = htmlBuffer.transferControlToOffscreen();

    let htmlSprite = document.createElement('canvas', { alpha: false });
    let offscreenSprite = htmlSprite.transferControlToOffscreen();

    let htmlText = document.createElement('canvas', { alpha: false });
    let offscreenOverlay = htmlText.transferControlToOffscreen();
    render_worker.postMessage({ "event": "init", "canvas": offscreenBuffer, "sprite": offscreenSprite, "overlay": offscreenOverlay, "RADAR_RADIUS": $scope.RADAR_RADIUS, "RADAR_SCALE": $scope.RADAR_SCALE, "FOCUS_PLAYER": $scope.FOCUS_PLAYER, "TEAMS": $scope.TEAMS, "LOOTFILTER": $scope.LOOTFILTER, "FOCUSONLY": $scope.FOCUSONLY, "HIDECORPSES": $scope.HIDECORPSES }, [offscreenBuffer, offscreenSprite, offscreenOverlay]);

    let render_behind = 0;
    let shouldprompt = true;
    render_worker.addEventListener('message', function (e) {
        if (e.data.msg === 'render') {
            ctx.transferFromImageBitmap(e.data.bitmap);

            render_behind--;
        }
    });

    function handleTick(data) {
        if (render_behind > 0)
            return;

        $scope.data = {
            "host": data.host,
            "start": data.start,
            "players": data.players,
            "groups": data.groups,
            "exfils": data.exfils,
            "token": data.token,
            "loot": $scope.data.loot
        }

        render_worker.postMessage({ "event": "render", "message": $scope.data, "behind": render_behind });
        render_behind++;
    }

    function connect() {
        if (shouldprompt) {
            shouldprompt = false;
            let url = "";
            while (url == "" || url == null)
                url = prompt("Please enter the server host address", oldurl)
            oldurl = url;
            shouldprompt = true;

            var socket = io.connect(url, {
                'reconnection': true,
                'secure': true
            });

            socket.on('connect', function () {
                var passcode = prompt("Please enter your passcode");
                socket.emit('authenticate', { "Passcode": passcode, "isHost": false, "isClient": true }, (response) => {
                    if (response.success === true) {
                        console.log('User is authenticated');
                        $scope.data = dummydata;
                    }
                    else {
                        alert("Passcode was invalid")
                        socket.disconnect();
                    }
                });
            });

            socket.on('connect_failed', (error) => {
                console.log(error)
                connect()
            });

            socket.on('error', (error) => {
                console.log(error)
                connect()
            });

            socket.on('reconnect_failed', (error) => {
                console.log(error)
                connect()
            });

            socket.on('disconnect', (reason) => {
                console.log(reason)
                connect()
            });

            socket.on('gamestart', function () {
                $scope.data.loot = [];
            });

            socket.on('gameend', function () {
                $scope.data.loot = [];
            });

            socket.on('addloot', function (data) {
                $scope.data.loot.push(data);
            });

            socket.on('removeloot', function (data) {
                for (item in $scope.data.loot) {
                    if ($scope.data.loot[item].signature == data.signature) {
                        $scope.loot.splice(item, 1);
                        break;
                    }
                }
            });

            socket.on('tick', function (data) {
                handleTick(data);
            })
        }
    }

    $scope.changedValue = function () {
        render_worker.postMessage({ "event": "update", "RADAR_RADIUS": $scope.RADAR_RADIUS, "RADAR_SCALE": $scope.RADAR_SCALE, "FOCUS_PLAYER": $scope.FOCUS_PLAYER, "TEAMS": $scope.TEAMS, "LOOTFILTER": $scope.LOOTFILTER, "FOCUSONLY": $scope.FOCUSONLY, "HIDECORPSES": $scope.HIDECORPSES });
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