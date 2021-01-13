const SPRITE_RADIUS = 8;
const SPRITE_DIAMETER = SPRITE_RADIUS * 2;
const char_radius = 14;

var colours = [
    ['#4d4dff', 'triup'],
    ['#4d4dff', 'tridown'],
    ['#4d4dff', 'square'],
    ['#2F4F4F', 'triup'],
    ['#2F4F4F', 'tridown'],
    ['#2F4F4F', 'square'],
    ['#380474', 'triup'],
    ['#380474', 'tridown'],
    ['#380474', 'square'],
    ['#3B5323', 'square'],
    ['#3333ff', 'triup'],
    ['#3333ff', 'tridown'],
    ['#3333ff', 'square']
];

onmessage = function (e) {
    var data = e.data;
    var event = data.event;
    switch (event) {
        case "init":
            init(data);
            break;
        case "render":
            render(data)
            break;
        case "update":
            update(data)
            break;
    }
}

var canvas, ctx, RADAR_RADIUS, RADAR_SCALE;
var canvassprite, ctxsprite;
var canvasoverlay, ctxoverlay;

var focusedplayer;
var FOCUS_PLAYER, TEAMS, LOOTFILTER, FOCUSONLY, HIDECORPSES;

var ENEMYTEAMSRUNTIME = [];

var FPS = 0;
var LASTFPS = 0;
setInterval(function () {
    LASTFPS = FPS;
    FPS = 0;
}, 1000);

function generateGuidelines(ctxoverlay) {
    ctxoverlay.beginPath()
    drawCircle(ctxoverlay, [RADAR_RADIUS, RADAR_RADIUS], RADAR_RADIUS);
    drawCircle(ctxoverlay, [RADAR_RADIUS, RADAR_RADIUS], RADAR_RADIUS / 2);
    drawCircle(ctxoverlay, [RADAR_RADIUS, RADAR_RADIUS], RADAR_RADIUS / 4);
    drawCircle(ctxoverlay, [RADAR_RADIUS, RADAR_RADIUS], RADAR_RADIUS / 8);
    drawLine(ctxoverlay, [RADAR_RADIUS, RADAR_RADIUS * 2], [RADAR_RADIUS, 0]);
    drawLine(ctxoverlay, [0, RADAR_RADIUS], [RADAR_RADIUS * 2, RADAR_RADIUS]);
    ctxoverlay.strokeStyle = "#3B5323";
    ctxoverlay.fillStyle = "#3B5323";
    ctxoverlay.closePath();
    ctxoverlay.stroke()
}

function disableSmoothing(ctx) {
    ctx.mozImageSmoothingEnabled = false;
    ctx.webkitImageSmoothingEnabled = false;
    ctx.msImageSmoothingEnabled = false;
    ctx.imageSmoothingEnabled = false;
}

function update(data) {
    if (data.RADAR_RADIUS !== undefined) {
        if (data.RADAR_RADIUS != RADAR_RADIUS) {
            RADAR_RADIUS = data.RADAR_RADIUS;
            canvas.width = RADAR_RADIUS * 2;
            canvas.height = RADAR_RADIUS * 2;
            canvasoverlay.width = RADAR_RADIUS * 2;
            canvasoverlay.height = RADAR_RADIUS * 2;

            generateGuidelines(ctxoverlay);
        }
    }
    if (data.RADAR_SCALE !== undefined) RADAR_SCALE = data.RADAR_SCALE;
    if (data.FOCUS_PLAYER !== undefined) FOCUS_PLAYER = data.FOCUS_PLAYER;
    if (data.TEAMS !== undefined) TEAMS = data.TEAMS;
    if (data.LOOTFILTER !== undefined) LOOTFILTER = data.LOOTFILTER;
    if (data.FOCUSONLY !== undefined) FOCUSONLY = data.FOCUSONLY;
    if (data.HIDECORPSES !== undefined) HIDECORPSES = data.HIDECORPSES;
}

function init(data) {
    canvas = data.canvas;
    ctx = data.canvas.getContext('2d');
    canvassprite = data.sprite;
    ctxsprite = data.sprite.getContext('2d');
    canvasoverlay = data.overlay;
    ctxoverlay = data.overlay.getContext('2d');
    RADAR_RADIUS = data.RADAR_RADIUS;
    RADAR_SCALE = data.RADAR_SCALE;
    FOCUS_PLAYER = data.FOCUS_PLAYER;
    TEAMS = data.TEAMS;
    LOOTFILTER = data.LOOTFILTER;
    FOCUSONLY = data.FOCUSONLY;
    HIDECORPSES = data.HIDECORPSES;

    canvas.width = RADAR_RADIUS * 2;
    canvas.height = RADAR_RADIUS * 2;

    canvassprite.width = colours.length * SPRITE_DIAMETER;
    canvassprite.height = SPRITE_DIAMETER;

    canvasoverlay.width = RADAR_RADIUS * 2;
    canvasoverlay.height = RADAR_RADIUS * 2;

    disableSmoothing(ctx);

    disableSmoothing(ctxsprite);

    disableSmoothing(ctxoverlay);

    for (colour in colours) {
        var x = (colour * SPRITE_DIAMETER) + SPRITE_RADIUS;
        var y = SPRITE_RADIUS;

        ctxsprite.beginPath();
        if (colours[colour][1] == "triup" || colours[colour][1] == "tridown") {
            var r = colours[colour][1] == "triup" ? SPRITE_RADIUS : -SPRITE_RADIUS;
            ctxsprite.moveTo(x, y - r);
            ctxsprite.lineTo(x + r, y + r);
            ctxsprite.lineTo(x - r, y + r);
        }
        else if (colours[colour][1] == "square") {
            ctxsprite.rect(x - SPRITE_RADIUS, y - SPRITE_RADIUS, SPRITE_DIAMETER, SPRITE_DIAMETER);
        }
        ctxsprite.fillStyle = colours[colour][0];
        ctxsprite.closePath();
        ctxsprite.fill();
    }

    generateGuidelines(ctxoverlay);
}

function RotatePoint(point, center, angle) {
    angle = (angle * (Math.PI / 180));

    cosTheta = Math.cos(angle);
    sinTheta = Math.sin(angle);

    newx = cosTheta * (point[0] - center[0]) - sinTheta * (point[1] - center[1])
    newy = sinTheta * (point[0] - center[0]) + cosTheta * (point[1] - center[1])

    newx += center[0];
    newy += center[1];

    return [newx, newy];
}

function w2s(focusedplayer, cplayer, invertx = true) {
    /* get relative screenpos to focused player */
    var screenpos = [focusedplayer.coordinates[0] - cplayer.coordinates[0], focusedplayer.coordinates[2] - cplayer.coordinates[2]];

    /* scale */
    screenpos[0] *= RADAR_SCALE;
    screenpos[1] *= RADAR_SCALE;

    /* x coordinate getting inverted fix */
    if (invertx)
        screenpos[0] *= -1;

    /* move it into position relative to center */
    screenpos[0] += RADAR_RADIUS;
    screenpos[1] += RADAR_RADIUS;

    /* rotate it based on current player viewangle */
    screenpos = RotatePoint(screenpos, [RADAR_RADIUS, RADAR_RADIUS], -focusedplayer.viewangle[0]);

    /* floor */
    screenpos[0] = Math.floor(screenpos[0]);
    screenpos[1] = Math.floor(screenpos[1]);

    return screenpos;
}

/* World distance helper */
function wdistance(focusedplayer, cplayer) {
    var screenpos = [focusedplayer.coordinates[0] - cplayer.coordinates[0], focusedplayer.coordinates[2] - cplayer.coordinates[2]];
    var distance = Math.sqrt(Math.pow(screenpos[0], 2) + Math.pow(screenpos[1], 2));
    distance = Math.round(distance);

    return distance;
}

/* Drawing */
function drawCircle(control, center, radius) {
    control.moveTo(center[0] + radius, center[1]);
    control.arc(center[0], center[1], radius, 0, Math.PI * 2, true);
};

/* Drawing */
function drawLine(control, start, end) {
    control.moveTo(start[0], start[1]);
    control.lineTo(end[0], end[1]);
}

/* Drawing */
function drawText(control, center, text, color = "#380474", align = "center", size = char_radius) {
    control.fillStyle = color;
    control.font = "Bold " + size + "px Arial";
    control.textAlign = align;
    control.fillText(text, center[0], center[1]);
}

function drawTextOutline(control, center, text, color = "#380474", align = "center", size = char_radius) {
    control.fillStyle = color;
    ctx.strokeStyle = "green";
    control.font = size + "px Arial";
    control.textAlign = align;
    control.strokeText(text, center[0], center[1]);
    control.fillText(text, center[0], center[1]);
}

/* Render */
function drawSprite(control, center, spriteindex) {
    control.drawImage(canvassprite, SPRITE_DIAMETER * spriteindex, 0, SPRITE_DIAMETER, SPRITE_DIAMETER, center[0] - SPRITE_RADIUS / 2, center[1] - SPRITE_RADIUS / 2, SPRITE_RADIUS, SPRITE_RADIUS);
}

/* Main Render */
function render(data) {
    data = data.message;

    const players = data.players;
    const groups = data.groups;
    const loot = data.loot;

    /* clear full canvas */
    ctx.clearRect(0, 0, RADAR_RADIUS * 2, RADAR_RADIUS * 2);

    /* guide lines */
    ctx.drawImage(canvasoverlay, 0, 0, RADAR_RADIUS * 2, RADAR_RADIUS * 2, 0, 0, RADAR_RADIUS * 2, RADAR_RADIUS * 2);

    /* Find Focused Player from id */
    for (let player of players) {
        if (player.id == FOCUS_PLAYER) {
            focusedplayer = player;
            drawSprite(ctx, [RADAR_RADIUS, RADAR_RADIUS], 6);
            break;
        }
    }

    /* if no player is focused select the first one in array */
    if (focusedplayer == undefined) {
        focusedplayer = players[0];
    }

    var HVLoot = false;

    ctx.beginPath()
    for (let item of loot) {
        let distance = wdistance(focusedplayer, item);
        let screenpos = w2s(focusedplayer, item);

        let focus = LOOTFILTER[item.signature] !== undefined ? LOOTFILTER[item.signature]["enable"] : false;
        if (focus) { /* Render high value line */
            if (!item.corpse)
                drawLine(ctx, [RADAR_RADIUS, RADAR_RADIUS], screenpos);

            HVLoot = true;
        }
        else if (FOCUSONLY) {
            continue;
        }

        if (HIDECORPSES && item.corpse)
            continue;

        /* render */
        if (distance * RADAR_SCALE > RADAR_RADIUS)
            continue;

        /* render elevation, if scav is higher or lower show that by an up or downward triangle */
        if (Math.abs(focusedplayer.coordinates[1] - item.coordinates[1]) <= 2) /* check if altitude is equal with tolerance of 5 */
            drawSprite(ctx, screenpos, focus ? 8 : 5);
        else if (focusedplayer.coordinates[1] > item.coordinates[1])
            drawSprite(ctx, screenpos, focus ? 7 : 4);
        else
            drawSprite(ctx, screenpos, focus ? 6 : 3);

        drawText(ctx, [screenpos[0], screenpos[1] - SPRITE_RADIUS * 2], item.name, focus ? "#380474" : "#2F4F4F");
    }
    ctx.strokeStyle = "#380474";
    ctx.closePath();
    ctx.stroke()

    let groupcount = 0;

    /*
    for (let group of groups) {
        // if focused player has a group add all the people to teamed to be drawn later
        groupcount++;
        if (focusedplayer == undefined) continue;
        if (group.id == focusedplayer.groupid) {
            for (let player of group.members) {
                TEAMS[player] = { "enable": true };
            }
            break;
        }
    }
    */

    ctx.beginPath()
    for (let player of players) {
        if (player.groupid == "" || player.groupid == focusedplayer.groupid) continue;
        for (let inplayer of players) {
            if (player.groupid == inplayer.groupid)
            {
                let screenposa = w2s(focusedplayer, player);
                let screenposb = w2s(focusedplayer, inplayer);
                drawLine(ctx, screenposa, screenposb);
            }
        }
    }
    ctx.strokeStyle = "#FF0000";
    ctx.closePath();
    ctx.stroke()

    let enemyCount = 0;
    let scavCount = 0;

    /* iterate over all players */
    ctx.beginPath()
    for (let player of players) {
        if (player.id == FOCUS_PLAYER)
            continue;

        let distance = wdistance(focusedplayer, player);
        let screenpos = w2s(focusedplayer, player);

        let team = TEAMS[player.id] !== undefined ? TEAMS[player.id]["enable"] : false;
        if (team)
            drawLine(ctx, [RADAR_RADIUS, RADAR_RADIUS], screenpos);
        else if (!player.isscav)
            enemyCount++;
        else
            scavCount++;

        /* render */
        if (distance * RADAR_SCALE > RADAR_RADIUS)
            continue;

        {
            let facingpos = RotatePoint([screenpos[0], screenpos[1] + SPRITE_RADIUS * 5], screenpos, -((player.viewangle[0] - focusedplayer.viewangle[0] - 180) * -1));
            if (player.id != FOCUS_PLAYER)
                drawLine(ctx, screenpos, facingpos)
        }

        if (Math.abs(focusedplayer.coordinates[1] - player.coordinates[1]) <= 2)
            drawSprite(ctx, screenpos, team ? 8 : player.isscav ? 2 : 12);
        else if (focusedplayer.coordinates[1] > player.coordinates[1])
            drawSprite(ctx, screenpos, team ? 7 : player.isscav ? 1 : 11);
        else
            drawSprite(ctx, screenpos, team ? 6 : player.isscav ? 0 : 10);

        drawText(ctx, [screenpos[0], screenpos[1] - SPRITE_RADIUS * 4], player.name + " : " + player.membertype + " (" + parseFloat(Math.abs(focusedplayer.coordinates[1] - player.coordinates[1])).toFixed(2) + ") (" + distance + "m)", team ? "#380474" : player.isscav ? "#4d4dff" : "#3333ff");
        
        // We could draw the player weapon but it got a bit cluttered
        //drawText(ctx, [screenpos[0], screenpos[1] - SPRITE_RADIUS * 2], player.weapon, team ? "#380474" : player.isscav ? "#4d4dff" : "#3333ff", "center", char_radius - 2);
    }
    ctx.strokeStyle = "white";
    ctx.closePath();
    ctx.stroke()

    /* Render some basic info */
    drawText(ctx, [50, 100], "x" + RADAR_SCALE, "#380474", "left", 15);
    drawText(ctx, [50, 80], "SCAVs Alive: " + scavCount, "#380474", "left", 15);
    drawText(ctx, [50, 60], "Enemies Alive: " + enemyCount + " Groups: " + groupcount, "#380474", "left", 15);
    drawText(ctx, [50, 40], "FPS: " + LASTFPS + " - Token: " + data.token, "#380474", "left", 15);
    if (HVLoot)
        drawText(ctx, [50, 20], "HV loot detected!", "#380474", "left", 15);

    if (!data.host)
        drawText(ctx, [RADAR_RADIUS, RADAR_RADIUS], "HOST OFFLINE", "#380474", "center", 50);

    if (data.host && !data.start)
        drawText(ctx, [RADAR_RADIUS, RADAR_RADIUS], "WAITING FOR GAME", "#380474", "center", 50);

    exit();
}

function exit() {
    FPS++;
    const bitmap = canvas.transferToImageBitmap();
    postMessage({ msg: 'render', bitmap });
}