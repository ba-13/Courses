// @ts-check

/** @type {WebGLRenderingContext} */
let gl;
let canvas;
let cube = [null, null, null];
let sphere = [null, null, null];
/** @type {Cube} */
let cube_flat;
/** @type {Cube} */
let cube_gouraud;
/** @type {Cube} */
let cube_phong;
/** @type {Sphere} */
let sphere_flat;
/** @type {Sphere} */
let sphere_gouraud;
/** @type {Sphere} */
let sphere_phong;
let aPositionLocation;
let aNormalLocation;
let uMMatrixLocation;
let uVMatrixLocation;
let uPMatrixLocation;
let uColorLocation;
let uLightLocation;
let uNMatrixLocation;

let flatShaderProgram;
let gouraudShaderProgram;
let phongShaderProgram;

let vMatrix = mat4.create(); // view matrix
let mMatrix = mat4.create(); // model matrix
let pMatrix = mat4.create(); // projection matrix
let nMatrix = mat4.create();

// specify camera/eye coordinate system parameters
let eyePos = [0.0, 0.0, 4.0];
let COI = [0.0, 0.0, 0.0];
let viewUp = [0.0, 1.0, 0.0];

// lightPos
let lightPos = [5.0, 5.0, 10];

let degree0 = [0.0, 0.0, 0.0];
let degree1 = [0.0, 0.0, 0.0];

let prevMouseX = 0.0;
let prevMouseY = 0.0;
let currActive = -1;

function handleViewPort(a) {
  gl.viewport((a * canvas.width) / 3, 0, canvas.width / 3, canvas.height);
  gl.scissor((a * canvas.width) / 3, 0, canvas.width / 3, canvas.height);
  gl.clearColor(...hexToColorArray(`#${a * 4 + 3}0${a * 4 + 3}0${a * 4 + 3}0`));
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  // set up the view matrix, multiply into the modelview matrix
  mat4.identity(vMatrix);
  vMatrix = mat4.lookAt(eyePos, COI, viewUp, vMatrix);

  //set up perspective projection matrix
  mat4.identity(pMatrix);
  mat4.perspective(50, (1.0 * 2) / 3, 0.1, 1000, pMatrix);

  //set up the model matrix
  mat4.identity(mMatrix);

  // transformations applied here on model matrix
  mMatrix = mat4.rotate(mMatrix, degToRad(degree0[a]), [0, 1, 0]);
  mMatrix = mat4.rotate(mMatrix, degToRad(degree1[a]), [1, 0, 0]);

  // normal matrix
}

function drawScene0() {
  gl.clearColor(...hexToColorArray("#dcd4f2"));
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  let cube_ = cube[0];
  let sphere_ = sphere[0];
  let mStack = [];
  mat4.rotate(mMatrix, degToRad(30), [0.4, 1.0, 0.0]);
  mat4.scale(mMatrix, [0.5, 0.5, 0.5]);
  pushMatrix(mStack, mMatrix);
  mat4.translate(mMatrix, [0.0, 1.5, 0.0]);
  let color = hexToColorArray("#1572aa");
  sphere_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
  pushMatrix(mStack, mMatrix);
  mat4.translate(mMatrix, [0.0, -1.0, 0.0]);
  mat4.scale(mMatrix, [1.8, 3.0, 1.8]);
  color = hexToColorArray("#b6b674");
  cube_.draw(mMatrix, vMatrix, pMatrix, color);
}

function drawScene1() {
  gl.clearColor(...hexToColorArray("#f4d7d9"));
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  let cube_ = cube[1];
  let sphere_ = sphere[1];
  let mStack = [];
  let color = null;
  mat4.scale(mMatrix, [0.5, 0.5, 0.5]);
  mat4.translate(mMatrix, [0.5, -1.5, 0.0]);
  pushMatrix(mStack, mMatrix);
  color = hexToColorArray("#a2a2a2");
  mat4.scale(mMatrix, [1.2, 1.2, 1.2]);
  mat4.translate(mMatrix, [0.2, 0.0, 0.0]);
  sphere_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
  mat4.rotate(mMatrix, degToRad(-30), [-0.4, 0.5, 1.0]);
  mat4.translate(mMatrix, [-1.6, 0.2, -0.1]);
  pushMatrix(mStack, mMatrix);
  mat4.scale(mMatrix, [1.2, 1.2, 1.2]);
  color = hexToColorArray("#51dd13");
  cube_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
  mat4.translate(mMatrix, [0.0, 1.4, 0.0]);
  pushMatrix(mStack, mMatrix);
  mat4.scale(mMatrix, [0.8, 0.8, 0.8]);
  color = hexToColorArray("#a2a2a2");
  sphere_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
  mat4.rotate(mMatrix, degToRad(-45), [1.0, -1.0, 1.0]);
  mat4.translate(mMatrix, [0.0, 1.2, 0.0]);
  pushMatrix(mStack, mMatrix);
  mat4.scale(mMatrix, [0.8, 0.8, 0.8]);
  color = hexToColorArray("#51dd13");
  cube_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
  mat4.rotate(mMatrix, degToRad(-45), [1.0, 0.0, 0.0]);
  mat4.translate(mMatrix, [-0.8, 0.0, 0.0]);
  pushMatrix(mStack, mMatrix);
  mat4.scale(mMatrix, [0.4, 0.4, 0.4]);
  color = hexToColorArray("#a2a2a2");
  sphere_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
}

function sphereOnBoard(mMatrix, colorBoard, colorSphere, middle = false) {
  let color;
  let mStack = [];
  let cube_ = cube[2];
  let sphere_ = sphere[2];
  if (middle) {
    mat4.translate(mMatrix, [0.0, 0.2, 0.0]);
  }
  pushMatrix(mStack, mMatrix);
  if (middle) {
    mat4.scale(mMatrix, [0.9, 0.9, 0.9]);
    mat4.translate(mMatrix, [0.0, -0.1, 0.0]);
  }
  color = hexToColorArray(colorSphere);
  sphere_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
  pushMatrix(mStack, mMatrix);
  mat4.translate(mMatrix, [0.0, -1.1, 0.0]);
  mat4.scale(mMatrix, [6.0, 0.2, 1.3]);
  color = hexToColorArray(colorBoard);
  cube_.draw(mMatrix, vMatrix, pMatrix, color);
  mMatrix = popMatrix(mStack);
}
function drawScene2() {
  gl.clearColor(...hexToColorArray("#d3f8d7"));
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  let cube_ = cube[2];
  let sphere_ = sphere[2];
  let mStack = [];
  let color = null;
  mat4.scale(mMatrix, [0.3, 0.3, 0.3]);
  mat4.translate(mMatrix, [0.0, 3.0, 0.0]);
  mat4.rotate(mMatrix, degToRad(30), [0.2, 0.8, 0.0]);
  pushMatrix(mStack, mMatrix);
  sphereOnBoard(mMatrix, "#da2145", "#7878aa"); // top
  mMatrix = popMatrix(mStack);
  pushMatrix(mStack, mMatrix);
  mat4.rotate(mMatrix, degToRad(180), [0.0, 0.0, 1.0]);
  mat4.translate(mMatrix, [0.0, 6.1, 0.0]);
  sphereOnBoard(mMatrix, "#da2145", "#51dd13"); // bottom
  mMatrix = popMatrix(mStack);
  pushMatrix(mStack, mMatrix);
  mat4.rotate(mMatrix, degToRad(90), [0.0, 1.0, 0.0]);
  mat4.translate(mMatrix, [0.0, -2.2, 1.8]);
  sphereOnBoard(mMatrix, "#13cda5", "#a89845", true); // right
  mMatrix = popMatrix(mStack);
  pushMatrix(mStack, mMatrix);
  mat4.rotate(mMatrix, degToRad(90), [0.0, 1.0, 0.0]);
  mat4.translate(mMatrix, [0.0, -2.2, -1.8]);
  sphereOnBoard(mMatrix, "#b9b242", "#d324a4", true); // left
  mMatrix = popMatrix(mStack);
  pushMatrix(mStack, mMatrix);
  mat4.scale(mMatrix, [0.9, 0.9, 0.9]);
  mat4.translate(mMatrix, [-2.0, -4.5, 0.0]);
  color = hexToColorArray("#6a559f");
  sphere_.draw(mMatrix, vMatrix, pMatrix, color); // left sphere
  mMatrix = popMatrix(mStack);
  pushMatrix(mStack, mMatrix);
  mat4.scale(mMatrix, [0.9, 0.9, 0.9]);
  mat4.translate(mMatrix, [2.0, -4.5, 0.0]);
  color = hexToColorArray("#11b9ac");
  sphere_.draw(mMatrix, vMatrix, pMatrix, color); // right sphere
  mMatrix = popMatrix(mStack);
}

function initShaderLocations(shaderProgram) {
  // get locations of attributes and uniforms declared in the shader
  aPositionLocation = gl.getAttribLocation(shaderProgram, "aPosition");
  aNormalLocation = gl.getAttribLocation(shaderProgram, "aNormal");
  uMMatrixLocation = gl.getUniformLocation(shaderProgram, "uMMatrix");
  uVMatrixLocation = gl.getUniformLocation(shaderProgram, "uVMatrix");
  uPMatrixLocation = gl.getUniformLocation(shaderProgram, "uPMatrix");
  uNMatrixLocation = gl.getUniformLocation(shaderProgram, "uNMatrix");
  uColorLocation = gl.getUniformLocation(shaderProgram, "objColor");
  uLightLocation = gl.getUniformLocation(shaderProgram, "lightLocation");

  // enable the attribute arrays
  gl.enableVertexAttribArray(aPositionLocation);
  gl.enableVertexAttribArray(aNormalLocation);
  gl.enable(gl.DEPTH_TEST);
  gl.enable(gl.SCISSOR_TEST);
}

function initShaderBuffers(shaderProgram, a) {
  initShaderLocations(shaderProgram);
  cube[a] = new Cube(
    aPositionLocation,
    uColorLocation,
    aNormalLocation,
    uMMatrixLocation,
    uVMatrixLocation,
    uPMatrixLocation,
    uNMatrixLocation
  );
  sphere[a] = new Sphere(
    aPositionLocation,
    uColorLocation,
    aNormalLocation,
    uMMatrixLocation,
    uVMatrixLocation,
    uPMatrixLocation,
    uNMatrixLocation
  );
  cube[a].initBuffer();
  sphere[a].initBuffer();
}

function drawScene() {
  handleViewPort(0);
  gl.useProgram(flatShaderProgram);
  initShaderBuffers(flatShaderProgram, 0);
  drawScene0();
  handleViewPort(1);
  gl.useProgram(gouraudShaderProgram);
  initShaderBuffers(gouraudShaderProgram, 1);
  drawScene1();
  handleViewPort(2);
  gl.useProgram(phongShaderProgram);
  initShaderBuffers(phongShaderProgram, 2);
  drawScene2();
}

// This is the entry point from the html
function webGLStart() {
  canvas = document.getElementById("canva");
  document.addEventListener("mousedown", onMouseDown, false);
  // initialize WebGL
  initGL(canvas);
  flatShaderProgram = initShaders(vertexShaderCode_flat, fragShaderCode_flat);
  gouraudShaderProgram = initShaders(
    vertexShaderCode_gouraud,
    fragShaderCode_gouraud
  );
  phongShaderProgram = initShaders(
    vertexShaderCode_phong,
    fragShaderCode_phong
  );

  drawScene();
}
