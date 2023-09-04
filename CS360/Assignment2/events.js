let lightPosXval = document.getElementById("lightPosXval");
const lightPosSlider = document.getElementById("lightPosSlider");
let cameraPosZval = document.getElementById("cameraPosZval");
const cameraPosSlider = document.getElementById("cameraPosSlider");

lightPosXval.innerHTML = lightPosSlider.value;
cameraPosZval.innerHTML = cameraPosSlider.value;

lightPosSlider.oninput = function () {
  lightPosXval.innerHTML = this.value;
  lightPos[0] = this.value / 10.0;
  drawScene();
};
cameraPosSlider.oninput = function () {
  cameraPosZval.innerHTML = this.value;
  console.log("slider:", this.value);
  eyePos[2] = this.value / 1.0;
  drawScene();
};

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
    degree0 = degree0 + diffX1 / 5;

    var mouseY = canvas.height - event.clientY;
    var diffY2 = mouseY - prevMouseY;
    prevMouseY = mouseY;
    degree1 = degree1 - diffY2 / 5;

    drawScene(canvas);
  }
}

function onMouseUp(event) {
  document.removeEventListener("mousemove", onMouseMove, false);
  document.removeEventListener("mouseup", onMouseUp, false);
  document.removeEventListener("mouseout", onMouseOut, false);
}

function onMouseOut(event) {
  document.removeEventListener("mousemove", onMouseMove, false);
  document.removeEventListener("mouseup", onMouseUp, false);
  document.removeEventListener("mouseout", onMouseOut, false);
}
