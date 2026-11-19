var sock = null;
var state = { name: "", games: [], votes: [], current: null, ctype: 0, myVote: -1, view: "vote" };
var tokens = {};
var images = {};

var $ = function (id) { return document.getElementById(id); };

function inGame() { return typeof state.current === "number" && state.current >= 0; }

function slug(name) {
    return name.normalize("NFD").replace(/[̀-ͯ]/g, "")
    .toLowerCase().replace(/ /g, "_").replace(/[^a-z0-9_]/g, "");
}

function imageUrl(name) {
    return "/static/games/" + slug(name) + ".png";
}

function loadImage(name, callback) {
    var s = slug(name);
    if (s in images) return callback(images[s]);
    var img = new Image();
    img.onload = function () { images[s] = img.src; callback(img.src); };
    img.onerror = function () { images[s] = null; callback(null); };
    img.src = imageUrl(name);
}

function send(msg) {
    if (sock && sock.readyState === WebSocket.OPEN) sock.send(msg);
}

function storage(key, value) {
    try {
        if (value === undefined) return localStorage.getItem(key);
        localStorage.setItem(key, value);
    } catch (e) { return null; }
}

function buildGameList() {
    var list = $("games");
    list.textContent = "";
    state.games.forEach(function (name, i) {
        var li = document.createElement("li");
        li.onclick = function () { vote(i); };
        list.appendChild(li);
        loadImage(name, function (url) {
            if (url) {
                var img = document.createElement("img");
                img.src = url;
                img.alt = name;
                li.prepend(img);
            } else {
                li.prepend(document.createTextNode(name));
            }
        });
    });
    drawTokens();
}

function drawTokens() {
    var items = $("games").children;
    for (var i = 0; i < items.length; i++) {
        var li = items[i];
        var count = state.votes[i] || 0;
        var pos = tokens[i] || (tokens[i] = []);
        while (pos.length < count) pos.push({ x: 5 + Math.random() * 85, y: 5 + Math.random() * 75 });
        pos.length = count;

        li.querySelectorAll(".token").forEach(function (t) { t.remove(); });
        pos.forEach(function (p) {
            var t = document.createElement("span");
            t.className = "token";
            t.style.left = p.x + "%";
            t.style.top = p.y + "%";
            li.appendChild(t);
        });
        li.classList.toggle("voted", i === state.myVote);
    }
}

function vote(i) {
    state.myVote = i;
    send("1,0," + i);
    drawTokens();
    if (i === state.current) showView("controller");
}

var controllers = {
    1: function (area) {
        var input = document.createElement("input");
        input.type = "range"; input.min = 0; input.max = 255; input.value = 128;
        input.id = "slider";
        var last = 0;
        input.oninput = function () {
            var now = Date.now();
            if (now - last > 33) { last = now; send("1,1," + input.value); }
        };
        input.onchange = function () { send("1,1," + input.value); };
        area.appendChild(input);
    },
    2: function (area) {
        var pad = document.createElement("div");
        pad.id = "pad";
        var layout = ["", "0:▲", "", "3:◀", "4:●", "1:▶", "", "2:▼", ""];
        layout.forEach(function (cell) {
            if (!cell) { pad.appendChild(document.createElement("span")); return; }
            var b = document.createElement("button");
            var code = cell.split(":")[0];
            b.textContent = cell.split(":")[1];
            b.onpointerdown = function (e) { e.preventDefault(); send("1,1," + code + ",1"); };
            b.onpointerup = b.onpointercancel = b.onpointerleave = function () { send("1,1," + code + ",0"); };
            pad.appendChild(b);
        });
        area.appendChild(pad);
    }
};


function buildController() {
    var area = $("pad-area");
    area.textContent = "";
    var name = state.games[state.current] || "";
    $("bg").style.backgroundImage = "none";
    loadImage(name, function (url) {
        if (url) $("bg").style.backgroundImage = "url(" + JSON.stringify(url) + ")";
    });
        if (controllers[state.ctype]) controllers[state.ctype](area);
        else area.textContent = name + " (no controller for type " + state.ctype + ")";
}


function showView(view) {
    if (view === "controller" && !inGame()) view = "vote";
    state.view = view;
    $("games").classList.toggle("hidden", view !== "vote");
    $("controller").classList.toggle("hidden", view !== "controller");
    $("back").classList.toggle("hidden", view !== "controller");
    $("status").textContent = view === "vote" && inGame()
    ? "Playing: " + state.games[state.current] : "";
}

$("back").onclick = function () { showView("vote"); };

$("name").onclick = function () {
    var n = prompt("Your name", state.name);
    if (n === null) return;
    n = n.replace(/,/g, "").trim().slice(0, 16);
    if (!n) return;
    storage("name", n);
    send("0," + n);
};


function onMessage(data) {
    var parts = data.split(",");
    var type = parts.shift();

    if (type === "0") {
        state.name = parts.join(",");
        $("name").textContent = state.name;
    }

    else if (type === "1") {
        var game = parseInt(parts[0], 10);
        var ctype = parseInt(parts[1], 10);
        if (game !== state.current || ctype !== state.ctype) {
            state.current = game;
            state.ctype = ctype;
            if (game >= 0) buildController();
            showView(game >= 0 ? "controller" : "vote");
        }
    }

    else if (type === "2") {
        if (parts.join(",") !== state.games.join(",")) {
            state.games = parts.filter(function (p) { return p !== ""; });
            tokens = {};
            buildGameList();
            if (inGame()) buildController();
        }
    }

    else if (type === "3") {
        state.votes = parts.map(function (v) { return parseInt(v, 10) || 0; });
        drawTokens();
    }
}

function connect() {
    sock = new WebSocket((location.protocol === "https:" ? "wss://" : "ws://") + location.host + "/ws");
    sock.onopen = function () {
        $("status").textContent = "";
        var saved = storage("name");
        if (saved) send("0," + saved);
    };
        sock.onmessage = function (e) { onMessage(e.data); };
        sock.onclose = function () {
            $("status").textContent = "Disconnected, retrying…";
            state.current = null;
            state.myVote = -1;
            setTimeout(connect, 2000);
        };
}

connect();
