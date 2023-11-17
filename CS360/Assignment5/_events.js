let lightXVal = document.getElementById("sliderLightXVal");
let bounceVal = document.getElementById("sliderBounceVal");
let lightXSlider = document.getElementById("sliderLightX");
let bounceSlider = document.getElementById("sliderBounce");

lightXVal.innerHTML = lightXSlider.value;
bounceVal.innerHTML = bounceSlider.value;

lightXSlider.oninput = function () {
  lightXVal.innerHTML = this.value;
  changeLightX(this.value);
};

bounceSlider.oninput = function () {
  bounceVal.innerHTML = this.value;
  changeBounce(this.value);
};

let mouseIsDown = false;

function onMouseDown(event) {
  document.addEventListener("mousemove", onMouseMove, false);
  document.addEventListener("mouseup", onMouseUp, false);
  document.addEventListener("mouseout", onMouseOut, false);

  if (
    event.layerX <= canvas.width &&
    event.layerX >= 0 &&
    event.layerY <= canvas.height &&
    event.layerY >= 0
  ) {
    mouseIsDown = true;
    prevMouseX = event.clientX;
    prevMouseY = canvas.height - event.clientY;
  }
}

function onMouseMove(event) {
  // make mouse interaction only within canvas
  if (
    event.layerX <= canvas.width &&
    event.layerX >= 0 &&
    event.layerY <= canvas.height &&
    event.layerY >= 0
  ) {
    var mouseX = event.clientX;
    var diffX1 = mouseX - prevMouseX;
    prevMouseX = mouseX;
    degree0 = degree0 + diffX1 / 20;

    var mouseY = canvas.height - event.clientY;
    var diffY2 = mouseY - prevMouseY;
    prevMouseY = mouseY;
    degree1 = degree1 + diffY2 / 20;

    drawScene();
  }
}

function onMouseUp(event) {
  mouseIsDown = false;
  document.removeEventListener("mousemove", onMouseMove, false);
  document.removeEventListener("mouseup", onMouseUp, false);
  document.removeEventListener("mouseout", onMouseOut, false);
}

function onMouseOut(event) {
  mouseIsDown = false;
  document.removeEventListener("mousemove", onMouseMove, false);
  document.removeEventListener("mouseup", onMouseUp, false);
  document.removeEventListener("mouseout", onMouseOut, false);
}
