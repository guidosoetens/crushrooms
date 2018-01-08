var controllerDiv;
var buttons = [];
var slots = [];
const backBtnIndex = 5;
const sendBtnIndex = 6;
var currentSlotIndex = 0;

function getCurrentCommand() {
    var result = "";
    for(var i=0; i < currentSlotIndex && i < slots.length; ++i) {
        result += slots[i].cmdData;
    }
    return result;
}

function addControllerButton(txt, cb) {
    var btn = document.createElement("button");
    btn.className = "btnController";
    btn.innerHTML = txt;
    controllerDiv.appendChild(btn);
    buttons[buttons.length] = btn;
    btn.onclick = cb;
}

function addControllerSlot() {
    var slot = document.createElement("div");
    slot.className = "divSlot";
    slot.innerHTML = "";
    slot.cmdData = "";
    controllerDiv.appendChild(slot);
    slots[slots.length] = slot;
}

function addCommand(txt, cmdTxt) {
    if(currentSlotIndex < slots.length) {
        slots[currentSlotIndex].innerHTML = txt;
        slots[currentSlotIndex].cmdData = cmdTxt;
        ++currentSlotIndex;
    }
    console.log(getCurrentCommand());
}

function removeLastCommand() {
    var removeIdx = currentSlotIndex - 1;
    if(removeIdx >= 0) {
        slots[removeIdx].innerHTML = "";
        slots[removeIdx].cmdData = "";
        --currentSlotIndex;
    }
    console.log(getCurrentCommand());
}

function done() {

}

function initController() {
    controllerDiv = document.getElementById('controllerDiv');
 
    addControllerButton("&#xf060;", function() { addCommand("&#xf060;", "L"); } );
    addControllerButton("&#xf062;", function() { addCommand("&#xf062;", "U"); });
    addControllerButton("&#xf063;", function() { addCommand("&#xf063;", "D"); });
    addControllerButton("&#xf061;", function() { addCommand("&#xf061;", "R"); });
    addControllerButton("🅐", function() { addCommand("🅐", "A"); });
    addControllerButton("🅑", function() { addCommand("🅑", "B"); });
    addControllerButton("&#xf137;", function() { removeLastCommand(); });
    addControllerButton("&#xf00c;", function() { done(); });

    while(slots.length < 6)
        addControllerSlot();

    // controllerDiv.onclick = function () {
    //     console.log("click!");
    // };

    updateControllerLayout();
}

function updateControllerLayout() {

    var width = controllerDiv.offsetWidth;
    var height = controllerDiv.offsetHeight - 100; //reduce size to compensate for bottom overlay

    var btmHeight = height * (3.0 / 5.0);
    var btmHorStep = width / 6.0;
    var btnSize = Math.min(btmHeight / 2.0, btmHorStep);
    var btnMargin = .05 * btnSize;
    var visBtnSize = btnSize - 2 * btnMargin;

    const offsets_x = [ .25, 1.25, 1.25, 2.25, 3.75, 4.75, 3.75, 4.75 ];

    for(var i=0; i<buttons.length; ++i) {

        var y = (height - .5 * btmHeight) - .5 * btnSize + btnMargin;
        if(i > 5)
            y = .5 * (height - btmHeight) - .5 * btnSize + btnMargin;
        if(i == 1)
            y -= .5 * btnSize;
        if(i == 2)
            y += .5 * btnSize;

        buttons[i].style.width = visBtnSize + "px";
        buttons[i].style.height = visBtnSize + "px";
        buttons[i].style.top = y + "px";
        buttons[i].style.left = (offsets_x[i] * btmHorStep + btnMargin) + "px";
        buttons[i].style.borderRadius = (.5 * visBtnSize) + "px";
        buttons[i].style.fontSize = (.5 * visBtnSize) + "px";
        //buttons[i].style.borderBottom = (.02 * visBtnSize) + "px solid #555";
        buttons[i].style.boxShadow = (.02 * visBtnSize) + "px " + (.05 * visBtnSize) + "px rgba(0,0,0,0.2)";
    }

    var slotWidth = width * .08;
    var slotHeight = btnSize;
    var slotOffsetX = .25 * btmHorStep + btnMargin;
    var slotOffsetY = .5 * (height - btmHeight) - .5 * btnSize;
    var minSlotSize = Math.min(slotWidth, slotHeight);
    var stepMargin = .1 * slotWidth;

    for(var i=0; i<slots.length; ++i) {
        slots[i].style.width = slotWidth + "px";
        slots[i].style.height = slotHeight + "px";
        slots[i].style.top = slotOffsetY + "px";
        slots[i].style.left = (slotOffsetX + (slotWidth + stepMargin) * i) + "px";
        slots[i].style.borderRadius = (.1 * minSlotSize) + "px";
        slots[i].style.fontSize = (.8 * minSlotSize) + "px";
        slots[i].style.boxShadow = (.02 * minSlotSize) + "px " + (.05 * minSlotSize) + "px rgba(0,0,0,0.2)";
        slots[i].style.lineHeight = slots[i].style.height;
    }
}