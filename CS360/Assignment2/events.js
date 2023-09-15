let lightPosXval = document.getElementById("lightPosXval");
const lightPosXSlider = document.getElementById("lightPosXSlider");
let lightPosYval = document.getElementById("lightPosYval");
const lightPosYSlider = document.getElementById("lightPosYSlider");
let cameraPosZval = document.getElementById("cameraPosZval");
const cameraPosSlider = document.getElementById("cameraPosSlider");

lightPosXval.innerHTML = lightPosXSlider.value / 10.0;
lightPosYval.innerHTML = lightPosYSlider.value / 10.0;
cameraPosZval.innerHTML = cameraPosSlider.value / 10.0;

lightPosXSlider.oninput = function () {
  const val = this.value / 10.0;
  lightPosXval.innerHTML = val;
  lightPos[0] = val;
  drawScene();
};
lightPosYSlider.oninput = function () {
  const val = this.value / 10.0;
  lightPosYval.innerHTML = val;
  lightPos[1] = val;
  drawScene();
};
cameraPosSlider.oninput = function () {
  const val = this.value / 10.0;
  cameraPosZval.innerHTML = val;
  eyePos[2] = val;
  drawScene();
};

function onMouseDown(event) {
  document.addEventListener("mousemove", onMouseMove, false);
  document.addEventListener("mouseup", onMouseUp, false);
  document.addEventListener("mouseout", onMouseOut, false);

  let a = Math.floor(event.layerX / (canvas.width / 3));
  if (
    event.layerX <= canvas.width &&
    event.layerX >= 0 &&
    event.layerY <= canvas.height &&
    event.layerY >= 0
  ) {
    prevMouseX = event.clientX;
    prevMouseY = canvas.height - event.clientY;
    currActive = a;
  }
}

function onMouseMove(event) {
  // make mouse interaction only within canvas
  let a = Math.floor(event.layerX / (canvas.width / 3));
  if (
    event.layerX <= ((a + 1) * canvas.width) / 3 &&
    event.layerX >= (a * canvas.width) / 3 &&
    event.layerY <= canvas.height &&
    event.layerY >= 0 &&
    currActive == a
  ) {
    var mouseX = event.clientX;
    var diffX1 = mouseX - prevMouseX;
    prevMouseX = mouseX;
    degree0[a] = degree0[a] + diffX1 / 5;

    var mouseY = canvas.height - event.clientY;
    var diffY2 = mouseY - prevMouseY;
    prevMouseY = mouseY;
    degree1[a] = degree1[a] - diffY2 / 5;

    drawScene(canvas);
  }
}

function onMouseUp(event) {
  currActive = -1;
  document.removeEventListener("mousemove", onMouseMove, false);
  document.removeEventListener("mouseup", onMouseUp, false);
  document.removeEventListener("mouseout", onMouseOut, false);
}

function onMouseOut(event) {
  currActive = -1;
  document.removeEventListener("mousemove", onMouseMove, false);
  document.removeEventListener("mouseup", onMouseUp, false);
  document.removeEventListener("mouseout", onMouseOut, false);
}
