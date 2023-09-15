//@ts-check

/** @type {WebGLRenderingContext} */
let gl;
let canvas;
let animation;
let triangle;
let square;
let circle;

let aPositionLocation;
let uMMatrixLocation;
let uColorLocation;

function drawTrapezium(mMatrix, colors, rectSize = 1.2) {
  let matrixStack = [];
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.8, 0.5, 1.0]); // for whole trapezium
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [rectSize, 1.0, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-rectSize / 2, 0, 0]);
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [rectSize / 2, 0, 0]);
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawTree(mMatrix, canopyHeight = 0.8) {
  let matrixStack = [];
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, -0.45, 0.0]); // for whole tree

  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#794f4e"); // brown
  mMatrix = mat4.scale(mMatrix, [0.11, 0.6, 1.0]); // stump
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);

  mMatrix = mat4.scale(mMatrix, [1.2, 1.0, 1.0]); // scale all canopy
  mMatrix = mat4.translate(mMatrix, [0.0, canopyHeight, 0.0]); // translate all canopy

  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.7, 1.0, 1.0]);
  colors = hexToColorArray("#439755"); // deep green
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);

  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, 0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.9, 1.0, 1.0]);
  colors = hexToColorArray("#69b15a"); // green
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);

  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, 0.2, 0.0]);
  mMatrix = mat4.scale(mMatrix, [1.0, 1.0, 1.0]);
  colors = hexToColorArray("#80ca5f"); // light green
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);

  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawMountain(mMatrix) {
  let matrixStack = [];
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [1.3, 0.3, 1.0]);

  colors = hexToColorArray("#72503d");
  triangle.draw(mMatrix, colors);
  mMatrix = mat4.translate(mMatrix, [0, 0.5, 0]);
  mMatrix = mat4.rotate(mMatrix, degToRad(10), [0, 0, 1]);
  mMatrix = mat4.translate(mMatrix, [0, -0.5, 0]);
  colors = hexToColorArray("#886c4d");
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawBird(mMatrix) {
  let matrixStack = [];
  let colors;

  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.2, 0.2, 1.0]);
  colors = hexToColorArray("#000000");

  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, -0.5, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.7, 0.7, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.rotate(mMatrix, degToRad(-10), [0, 0, 1]);
  mMatrix = mat4.translate(mMatrix, [-2.5, 0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [5.0, 0.7, 1.0]);
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.rotate(mMatrix, degToRad(10), [0, 0, 1]);
  mMatrix = mat4.translate(mMatrix, [2.5, 0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [5.0, 0.7, 1.0]);
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawHouse(mMatrix) {
  let matrixStack = [];
  let colors;

  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#e0dfe0");
  mMatrix = mat4.scale(mMatrix, [0.8, 0.8, 1.0]);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [1.8, 1, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, 0.8, 0.0]);
  mMatrix = mat4.scale(mMatrix, [1.4, 1.6, 1.0]);
  colors = hexToColorArray("#ec460d");
  drawTrapezium(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);

  colors = hexToColorArray("#ffaa00");
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.5, 0.15, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.25, 0.25, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.5, 0.15, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.25, 0.25, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, -0.2, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.25, 0.6, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawBoat(mMatrix) {
  let matrixStack = [];
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, -0.5, 0.0]);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.2, 0.6, 0.0]);
  mMatrix = mat4.rotate(mMatrix, degToRad(-25), [0, 0, 1]);
  mMatrix = mat4.scale(mMatrix, [0.01, 1.0, 1.0]);
  colors = hexToColorArray("#000000");
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.03, 1.0, 1.0]);
  mMatrix = mat4.translate(mMatrix, [0.0, 0.6, 0.0]);
  colors = hexToColorArray("#000000");
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.rotate(mMatrix, degToRad(180), [0, 0, 1]);
  mMatrix = mat4.scale(mMatrix, [0.6, 0.6, 1.0]);
  colors = hexToColorArray("#e0dfe0");
  drawTrapezium(mMatrix, colors, 1.8);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.8, 0.8, 1.0]);
  mMatrix = mat4.translate(mMatrix, [0.515, 0.8, 0.0]);
  mMatrix = mat4.rotate(mMatrix, degToRad(-90), [0, 0, 1]);
  colors = hexToColorArray("#ff0000");
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawSun(mMatrix, offset = 0) {
  let matrixStack = [];
  const numRays = 10;
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.5, 0.5, 1.0]);
  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#ffdd00");
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  for (let i = 0; i < numRays; i++) {
    let angle = (i * 360) / numRays + offset;
    pushMatrix(matrixStack, mMatrix);
    mMatrix = mat4.rotate(mMatrix, degToRad(angle), [0, 0, 1]);
    mMatrix = mat4.translate(mMatrix, [-0.9, -0.9, 0.0]);
    mMatrix = mat4.rotate(mMatrix, degToRad(-225), [0, 0, 1]);
    mMatrix = mat4.scale(mMatrix, [0.05, 1.0, 1.0]);
    triangle.draw(mMatrix, colors);
    mMatrix = popMatrix(matrixStack);
  }
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawWindmill(mMatrix, offset = 0) {
  const numBlades = 4;
  let matrixStack = [];
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.8, 0.8, 1.0]);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, -0.6, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.05, 1.2, 1.0]);
  colors = hexToColorArray("#000000");
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  colors = hexToColorArray("#c4c44a");
  for (let i = 0; i < numBlades; i++) {
    let angle = (i * 360) / numBlades + offset;
    pushMatrix(matrixStack, mMatrix);
    mMatrix = mat4.rotate(mMatrix, degToRad(angle), [0, 0, 1]);
    mMatrix = mat4.translate(mMatrix, [-0.25, -0.25, 0.0]);
    mMatrix = mat4.rotate(mMatrix, degToRad(-45), [0, 0, 1]);
    mMatrix = mat4.scale(mMatrix, [0.22, 0.9, 1.0]);
    triangle.draw(mMatrix, colors);
    mMatrix = popMatrix(matrixStack);
  }
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.06, 0.06, 1.0]);
  colors = hexToColorArray("#000000");
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawWheel(mMatrix) {
  let matrixStack = [];
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.14, 0.14, 1.0]);
  colors = hexToColorArray("#000000");
  circle.draw(mMatrix, colors);
  mMatrix = mat4.scale(mMatrix, [0.8, 0.8, 1.0]);
  colors = hexToColorArray("#808080");
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawCar(mMatrix) {
  let matrixStack = [];
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.27, -0.2, 0.0]);
  drawWheel(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.27, -0.2, 0.0]);
  drawWheel(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.8, 0.7, 1.0]);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, 0.15, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.7, 1.4, 1.0]);
  colors = hexToColorArray("#bf6b53");
  mMatrix = drawTrapezium(mMatrix, colors, 1.2);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [1.0, 0.9, 1.0]);
  colors = hexToColorArray("#377ede");
  mMatrix = drawTrapezium(mMatrix, colors, 1.5);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawBushes(mMatrix) {
  let matrixStack = [];
  let colors;
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.3, 0.3, 1.0]);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-1.2, -0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [1.0, 0.7, 1.0]);
  colors = hexToColorArray("#50b033");
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [1.1, -0.05, 0.0]);
  mMatrix = mat4.scale(mMatrix, [1.0, 0.7, 1.0]);
  colors = hexToColorArray("#286010");
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [1.4, 1.0, 1.0]);
  colors = hexToColorArray("#42972a");
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

function drawClouds(mMatrix) {
  let matrixStack = [];
  let colors;
  colors = hexToColorArray("#ffffff");
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [0.3, 0.3, 1.0]);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-1.2, -0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [1.5, 0.6, 1.0]);
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [1.5, -0.05, 0.0]);
  mMatrix = mat4.scale(mMatrix, [1.2, 0.9, 1.0]);
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [3, -0.15, 0.0]);
  mMatrix = mat4.scale(mMatrix, [1.2, 0.6, 1.0]);
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.scale(mMatrix, [1.4, 1.1, 1.0]);
  circle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  return mMatrix;
}

let sunOffset = 0;
let boatOffset = -0.1;
let carOffset = -0.5;
let boatIncreases = true;
let windmill1Offset = 35;
let windmill2Offset = 25;
let mMatrix = mat4.create();
mat4.identity(mMatrix);

function drawScene() {
  if (!canvas) alert("Canvas not initialised");
  /** @type {[number, number, number, number]} */
  let colors = hexToColorArray("#ffffff"); // #80cafa sky blue

  let matrixStack = [];

  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(...colors);
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  sunOffset -= 0.3;
  windmill1Offset += 0.8;
  windmill2Offset += 0.85;
  if (boatIncreases) boatOffset += 0.003;
  else boatOffset -= 0.003;
  if (boatOffset > 0.6) boatIncreases = false;
  if (boatOffset < -0.6) boatIncreases = true;

  // drawing from most depth to least
  // eky
  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#80cafa");
  mMatrix = mat4.translate(mMatrix, [0.0, 1.0, 0.0]);
  mMatrix = mat4.scale(mMatrix, [2.0, 2.0, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  // sun
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.65, 0.7, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.3, 0.3, 1.0]);
  drawSun(mMatrix, sunOffset);
  mMatrix = popMatrix(matrixStack);
  // clouds
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.8, 0.4, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.4, 0.4, 1.0]);
  drawClouds(mMatrix);
  mMatrix = popMatrix(matrixStack);
  // birds
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.1, 0.6, 1.0]);
  mMatrix = mat4.scale(mMatrix, [0.08, 0.08, 1.0]);
  drawBird(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.4, 0.8, 1.0]);
  mMatrix = mat4.scale(mMatrix, [0.06, 0.06, 1.0]);
  drawBird(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.1, 0.8, 1.0]);
  mMatrix = mat4.scale(mMatrix, [0.02, 0.02, 1.0]);
  drawBird(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.2, 0.65, 1.0]);
  mMatrix = mat4.scale(mMatrix, [0.05, 0.05, 1.0]);
  drawBird(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0, 0.7, 1.0]);
  mMatrix = mat4.scale(mMatrix, [0.03, 0.03, 1.0]);
  drawBird(mMatrix);
  mMatrix = popMatrix(matrixStack);

  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.0, 0.1, 0.0]);
  // mountains
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.7, -0.05, 0.0]);
  drawMountain(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0, 0.0, 0.0]);
  drawMountain(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.8, -0.15, 0.0]);
  drawMountain(mMatrix);
  mMatrix = popMatrix(matrixStack);
  // land
  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#68e28a");
  mMatrix = mat4.translate(mMatrix, [0.0, -1.15, 0.0]);
  mMatrix = mat4.scale(mMatrix, [2.0, 2.0, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  // path
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.4, -1.05, 0.0]);
  mMatrix = mat4.rotate(mMatrix, degToRad(45), [0, 0, 1]);
  mMatrix = mat4.scale(mMatrix, [2.0, 2.0, 1.0]);
  colors = hexToColorArray("#78b148");
  triangle.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  // river
  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#3395ff");
  mMatrix = mat4.translate(mMatrix, [0.0, -0.33, 0.0]);
  mMatrix = mat4.scale(mMatrix, [2.0, 0.25, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#ffffff");
  mMatrix = mat4.translate(mMatrix, [0, -0.26, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.6, 0.005, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#ffffff");
  mMatrix = mat4.translate(mMatrix, [-0.6, -0.38, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.4, 0.005, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#ffffff");
  mMatrix = mat4.translate(mMatrix, [0.6, -0.42, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.5, 0.005, 1.0]);
  square.draw(mMatrix, colors);
  mMatrix = popMatrix(matrixStack);
  // trees
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.8, 0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.37, 0.37, 1.0]);
  drawTree(mMatrix, 0.7);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.55, 0.16, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.45, 0.45, 1.0]);
  drawTree(mMatrix);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.3, 0.12, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.4, 0.4, 1.0]);
  drawTree(mMatrix, 0.6);
  mMatrix = popMatrix(matrixStack);
  // boat
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [boatOffset, -0.2, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.24, 0.24, 1.0]);
  drawBoat(mMatrix);
  mMatrix = popMatrix(matrixStack);
  // windmill
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.5, -0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.42, 0.42, 1.0]);
  drawWindmill(mMatrix, windmill1Offset);
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.65, -0.1, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.45, 0.45, 1.0]);
  drawWindmill(mMatrix, windmill2Offset);
  mMatrix = popMatrix(matrixStack);
  // bushes front of windmill
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [0.8, -0.55, 0.0]);
  mMatrix = mat4.rotate(mMatrix, degToRad(180), [0, 0, 1]);
  mMatrix = mat4.scale(mMatrix, [0.35, 0.35, 1.0]);
  drawBushes(mMatrix);
  mMatrix = popMatrix(matrixStack);
  // bushes behind house
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.85, -0.75, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.2, 0.2, 1.0]);
  drawBushes(mMatrix);
  mMatrix = popMatrix(matrixStack);
  // bushes behind house
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.28, -0.75, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.22, 0.22, 1.0]);
  drawBushes(mMatrix);
  mMatrix = popMatrix(matrixStack);
  // house
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.6, -0.7, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.31, 0.31, 1.0]);
  drawHouse(mMatrix);
  mMatrix = popMatrix(matrixStack);
  // car
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [carOffset, -0.9, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.31, 0.31, 1.0]);
  drawCar(mMatrix);
  mMatrix = popMatrix(matrixStack);
  mMatrix = popMatrix(matrixStack);
  // bushes below car
  pushMatrix(matrixStack, mMatrix);
  mMatrix = mat4.translate(mMatrix, [-0.3, -1.05, 0.0]);
  mMatrix = mat4.scale(mMatrix, [0.5, 0.5, 1.0]);
  drawBushes(mMatrix);
  mMatrix = popMatrix(matrixStack);
}

let animate = function () {
  if (animation) {
    window.cancelAnimationFrame(animation);
  }
  drawScene();
  animation = window.requestAnimationFrame(animate);
};

function changeFillType(fillType) {
  triangle = new Triangle(
    uMMatrixLocation,
    aPositionLocation,
    uColorLocation,
    fillType
  );
  square = new Square(
    uMMatrixLocation,
    aPositionLocation,
    uColorLocation,
    fillType
  );
  circle = new Circle(
    uMMatrixLocation,
    aPositionLocation,
    uColorLocation,
    60,
    fillType
  );

  animate();
}

function onKeyPress(event) {
  const key = event.key;
  switch (key) {
    case "ArrowRight":
      carOffset += 0.04;
      break;
    case "ArrowLeft":
      carOffset -= 0.04;
      break;
  }
  animate();
}

function webGLStart() {
  canvas = document.getElementById("canva");
  initGL(canvas); // initialises gl
  fill_type = {
    point: gl.POINTS,
    line: gl.LINE_LOOP,
    fill: gl.TRIANGLES,
  };

  document.addEventListener("keydown", onKeyPress, false);

  let shaderProgram = initShaders(vertexShaderCode, fragmentShaderCode);
  if (!shaderProgram) alert("shaders program didn't compile/link");

  aPositionLocation = gl.getAttribLocation(shaderProgram, "aPosition");
  uMMatrixLocation = gl.getUniformLocation(shaderProgram, "uMMatrix");
  uColorLocation = gl.getUniformLocation(shaderProgram, "uColor");
  console.log("Got Uniform Locations in shaderProgram");

  gl.enableVertexAttribArray(aPositionLocation);
  console.log("Enabled Vertex Array from aPositionLocation");

  changeFillType("fill"); // default
}
