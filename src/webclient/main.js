var socket;
var isIdle = true;
var isClosed = false;
var closedMessage = "";
var totalCommand = "";

function Button(txt, cmd) {
    this.x = 0;
    this.y = 0;
    this.radius = 0;
    this.txt = txt;
    this.isPressed = false;
    this.command = cmd;
}

function updateButton(button, x, y, rad) {
    button.x = x;
    button.y = y;
    button.radius = rad;
}

var buttons = [];
var txts = [ "◄", "▲", "▼", "►", "A", "B", "⟲", "✔" ];
var cmd = [ "L", "U", "D", "R", "A", "B", "x", "x" ];
for (var i = 0; i < txts.length; i++) {
    buttons[i] = new Button(txts[i], cmd[i]);
}

function alignContent() {
    var msgDiv = document.getElementById('msgDiv');
    msgDiv.style.left = ((window.innerWidth - 400) / 2) + "px";
    msgDiv.style.top = ((window.innerHeight - 100) / 2) + "px";  
    updateCanvas();
};

function onJoinClicked() {
    var name = document.getElementById('tbUsername').value;
    if(name.length > 0) {
      var message = "login-user:" + name;
      sendToServer(message);
      document.getElementById('overlayDiv').style.visibility = 'collapse';
    }
};

function onCanvasClick(event) {

    if(isIdle || isClosed)
        return;

    var x = event.clientX;
    var y = event.clientY;

    var idx = -1;
    var minDist = 10000.0;
    for (var i = 0; i < buttons.length; i++) {
        var dx = buttons[i].x - x;
        var dy = buttons[i].y - y;
        var dist = Math.sqrt(dx * dx + dy * dy);
        if(dist < buttons[i].radius && dist < minDist) {
            idx = i;
            minDist = dist;
        }
    }

    if(idx >= 0) {
        if(idx < 6) {
            if(totalCommand.length < 6) {
                totalCommand += buttons[idx].command;
                var message = "user-cmd:" + cmd;
                socket.send(message);
            }
        }
        else if(idx == 6) {
            var n = totalCommand.length;
            if(n > 0)
                totalCommand = totalCommand.substr(0, n-1);
        }
        else {
            socket.send("user-done");
            isIdle = true;
        }
    }

    updateCanvas();
}

function drawButton(ctx, btn) {
    ctx.fillStyle = btn.isPressed ? "#555" : "#aaa";
    ctx.strokeStyle = '#777';
    ctx.beginPath();
    ctx.arc(btn.x, btn.y, .9 * btn.radius, 0, 2*Math.PI);
    ctx.fill();
    ctx.lineWidth = 5;
    ctx.stroke();

    ctx.fillStyle = "#000";
    ctx.font = (btn.radius) + "px Arial";
    ctx.textAlign="center";
    ctx.fillText(btn.txt, btn.x, btn.y + .3 * radius);
}

function updateCanvas() {
    var c = document.getElementById("controllerCanvas");
    c.width = window.innerWidth;
    c.height = window.innerHeight;

    var ctx = c.getContext("2d");
    ctx.clearRect(0, 0, c.width, c.height);

    if(isClosed) {
        ctx.font = "30px Arial";
        ctx.fillText(closedMessage, 10, 50);
    }
    else if(isIdle) {
        ctx.font = "30px Arial";
        ctx.fillText("Waiting for Crushrooms game!", 10, 50);
    }
    else {
        ctx.font = "30px Arial";
        ctx.fillText(totalCommand, 300, 100);
    
        var btmHeight = c.height * (2.0 / 3.0);
        var btmHorStep = c.width / 7.0;
        radius = .5 * Math.min(btmHeight / 2.0, btmHorStep);
    
        updateButton(buttons[0], btmHorStep, btmHeight, radius);
        updateButton(buttons[1],2 * btmHorStep, btmHeight - radius, radius);
        updateButton(buttons[2],2 * btmHorStep, btmHeight + radius, radius);
        updateButton(buttons[3],3 * btmHorStep, btmHeight, radius);
        updateButton(buttons[4],5 * btmHorStep, btmHeight, radius);
        updateButton(buttons[5],6 * btmHorStep, btmHeight, radius);
        updateButton(buttons[6],5 * btmHorStep, btmHeight / 3.0, radius);
        updateButton(buttons[7],6 * btmHorStep, btmHeight / 3.0, radius);
    
        for (var i = 0; i < buttons.length; i++) {
            drawButton(ctx, buttons[i]);
        }
    }
}

window.onresize = function(event) {
    alignContent();
};

window.onload = function() {
    alignContent();
    var elem = document.getElementById('tbUsername');
    elem.onkeypress = function(e) { if (e.keyCode == '13') onJoinClicked(); };
    prepareConnection();
    // var tbContainer = document.getElementById('tbContainer');
    // tbContainer.style.visibility = 'inherit';
    // document.getElementById('tbState').innerHTML = "Join Crushrooms!";

    var controllerCanvas = document.getElementById('controllerCanvas');
    controllerCanvas.onclick = onCanvasClick;//.addEventListener("click", printMousePos);
};

function getParam(param) {
  var paramsStr = window.location.search.substring(1);
  var params = paramsStr.split("&");
  for (var i = 0; i < params.length; i++) {
    var tuple = params[i].split("=");
    if(tuple[0].localeCompare(param) == 0)
      return tuple[1];
  }
  return "";
}

function prepareConnection() {
    var ip = getParam("ip");
    socket = new WebSocket("ws://" + ip + ":9093");

    socket.onerror = function(error) {
        isClosed = true;
        closedMessage = 'Server Error: ' + error;
        updateCanvas();
    };

    socket.onopen = function(event) {
        var tbContainer = document.getElementById('tbContainer');
        tbContainer.style.visibility = 'inherit';
        document.getElementById('tbState').innerHTML = "Join Crushrooms!";
    };

    socket.onmessage = function(event) {
        var message = event.data;
        if(message.match("^start-cmd")) {
            isIdle = false;
            totalCommand = "";
            updateCanvas();
          }
          if(message.match("^end-msg")) {
            isIdle = true;
            updateCanvas();
          }
    };

    socket.onclose = function(event) {
        isClosed = true;
        closedMessage = 'Server Closed';
        updateCanvas();
    };
};

function sendToServer(message) {
    socket.send(message);
};
