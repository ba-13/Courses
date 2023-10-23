// @ts-check
"use strict";

/** @type {HTMLCanvasElement} */
let canvas;
/** @type {WebGLRenderingContext} */
let gl;

let shaderProgram;
let animation;
let aPositionLoc, aNormalLoc, aTexCoordLoc;
let uMMatrixLoc, uVMatrixLoc, uPMatrixLoc, uNMatrixLoc;
let uColorLoc, uLightLoc, uEyePosLoc;
let uTextureLoc, uObjectTypeLoc, uCubeMapTexLoc;
let uLevelsLoc;

let radius = 9.0;
let eyePos = [null, null, null];
let COI = [0.0, 0.0, 0.0];
let viewUp = [0.0, 1.0, 0.0];
let lightPos = [10.0, 10.0, 10.0];
let degree0 = 0;
let degree1 = 20;
let rateOfChange = 0.4;

/** @type {Cube} */
let cube;
/** @type {Sphere} */
let sphere;
/** @type {Sheet} */
let sheet;
/** @type {MeshObject} */
let kettle;

let rcube_path = "./textures/rcube.png";
let wood_path = "./textures/wood_texture.jpg";
let kettle_path = "./textures/teapot.json";
let rcube_texture;
let wood_texture;
let cubemap_texture;
let sky_textures;

let vMatrix = mat4.create(); // view matrix
let mMatrix = mat4.create(); // model matrix
let pMatrix = mat4.create(); // projection matrix
let matrixStack = [];

function drawSkyBox() {
  let color = hexToColorArray("#fa1300");
  const screendist = 800;
  const disp = 399.5;

  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [0, 0, -disp]);
  mat4.scale(mMatrix, [screendist, screendist, 0]);
  sheet.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    sky_textures[5],
    cubemap_texture
  );
  mMatrix = popMatrix(matrixStack);

  // front face
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [0, 0, disp]);
  mat4.scale(mMatrix, [screendist, screendist, 0]);
  mat4.rotate(mMatrix, degToRad(180), [0, 1, 0]);
  sheet.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    sky_textures[4],
    cubemap_texture
  );
  mMatrix = popMatrix(matrixStack);

  // top face
  pushMatrix(matrixStack, mMatrix);
  mat4.rotate(mMatrix, degToRad(180), [0, 1, 0]);
  mat4.translate(mMatrix, [0, disp, 0]);
  mat4.scale(mMatrix, [screendist, 0, screendist]);
  mat4.rotate(mMatrix, degToRad(90), [1, 0, 0]);
  sheet.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    sky_textures[2],
    cubemap_texture
  );
  mMatrix = popMatrix(matrixStack);

  // bottom face
  pushMatrix(matrixStack, mMatrix);
  mat4.rotate(mMatrix, degToRad(180), [0, 1, 0]);
  mat4.translate(mMatrix, [0, -disp, 0]);
  mat4.scale(mMatrix, [screendist, 0, screendist]);
  mat4.rotate(mMatrix, degToRad(-90), [1, 0, 0]);
  sheet.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    sky_textures[3],
    cubemap_texture
  );
  mMatrix = popMatrix(matrixStack);

  // right face
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [disp, 0, 0]);
  mat4.scale(mMatrix, [0, screendist, screendist]);
  mat4.rotate(mMatrix, degToRad(180), [0, 1, 0]);
  mat4.rotate(mMatrix, degToRad(90), [0, 1, 0]);
  sheet.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    sky_textures[0],
    cubemap_texture
  );
  mMatrix = popMatrix(matrixStack);

  // left face
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [-disp, 0, 0]);
  mat4.scale(mMatrix, [0, screendist, screendist]);
  mat4.rotate(mMatrix, degToRad(180), [0, 1, 0]);
  mat4.rotate(mMatrix, degToRad(-90), [0, 1, 0]);
  sheet.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    sky_textures[1],
    cubemap_texture
  );
  mMatrix = popMatrix(matrixStack);
}

function drawKettle() {
  let color, levels;
  color = hexToColorArray("#78f222");
  levels = [0.0, 0.0, 0.0];
  pushMatrix(matrixStack, mMatrix);
  mat4.scale(mMatrix, [0.23, 0.23, 0.23]);
  kettle.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
}

function drawTable() {
  let color, levels;
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [0.0, -2.0, 0.0]);
  mat4.scale(mMatrix, [7.0, 0.3, 5.0]);
  color = hexToColorArray("#964b00");
  levels = [0.1, 0.4, 0.0];
  sphere.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [-3.0, -6.0, -3.0]);
  mat4.scale(mMatrix, [0.5, 8.0, 0.5]);
  cube.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [-3.0, -6.0, 3.0]);
  mat4.scale(mMatrix, [0.5, 8.0, 0.5]);
  cube.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [3.0, -6.0, -3.0]);
  mat4.scale(mMatrix, [0.5, 8.0, 0.5]);
  cube.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [3.0, -6.0, 3.0]);
  mat4.scale(mMatrix, [0.5, 8.0, 0.5]);
  cube.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
}

function drawScene() {
  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(...hexToColorArray("#282828"));
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  let color, levels;

  if (!mouseIsDown) degree0 = (degree0 - rateOfChange) % 360;

  let deg0 = degToRad(degree0);
  let deg1 = degToRad(degree1);
  eyePos = [
    radius * Math.cos(-deg1) * Math.cos(deg0),
    radius * Math.sin(deg1),
    radius * Math.cos(-deg1) * Math.sin(deg0),
  ];

  gl.uniform3fv(uEyePosLoc, eyePos);
  gl.uniform3fv(uLightLoc, lightPos);

  mat4.identity(mMatrix);
  mat4.identity(pMatrix);
  mat4.identity(vMatrix);
  vMatrix = mat4.lookAt(eyePos, COI, viewUp, vMatrix);
  mat4.perspective(50, 2.0, 0.1, 1000, pMatrix);

  pushMatrix(matrixStack, mMatrix);
  drawSkyBox();
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [0.0, 0.1, -1.5]);
  drawKettle();
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  drawTable();
  mMatrix = popMatrix(matrixStack);

  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [3.0, -0.9, 0.0]);
  mat4.scale(mMatrix, [0.8, 0.8, 0.8]);
  color = hexToColorArray("#3f1298");
  levels = [0.5, 0.0, 0.0];
  sphere.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [-0.0, -0.9, 2.0]);
  mat4.scale(mMatrix, [0.8, 0.8, 0.8]);
  color = hexToColorArray("#1f9e45");
  levels = [0.5, 0.0, 0.0];
  sphere.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    wood_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [2.0, -1.2, 2.0]);
  color = hexToColorArray("#191919");
  levels = [0.0, 1.0, 0.0];
  cube.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    rcube_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
  pushMatrix(matrixStack, mMatrix);
  mat4.translate(mMatrix, [-3.0, -0.4, 1.0]);
  mat4.scale(mMatrix, [1.3, 2.5, 1.3]);
  levels = [0.0, 0.0, 1.0];
  cube.draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    rcube_texture,
    cubemap_texture,
    levels
  );
  mMatrix = popMatrix(matrixStack);
}

let animate = function () {
  if (animation) {
    window.cancelAnimationFrame(animation);
  }
  drawScene();
  animation = window.requestAnimationFrame(animate);
};

function initLocations() {
  aPositionLoc = gl.getAttribLocation(shaderProgram, "aPosition");
  aNormalLoc = gl.getAttribLocation(shaderProgram, "aNormal");
  aTexCoordLoc = gl.getAttribLocation(shaderProgram, "aTexCoord");
  uMMatrixLoc = gl.getUniformLocation(shaderProgram, "uMMatrix");
  uVMatrixLoc = gl.getUniformLocation(shaderProgram, "uVMatrix");
  uPMatrixLoc = gl.getUniformLocation(shaderProgram, "uPMatrix");
  uNMatrixLoc = gl.getUniformLocation(shaderProgram, "uNMatrix");
  uColorLoc = gl.getUniformLocation(shaderProgram, "uColor");
  uLightLoc = gl.getUniformLocation(shaderProgram, "uLightPos");
  uEyePosLoc = gl.getUniformLocation(shaderProgram, "uEyePos");
  uTextureLoc = gl.getUniformLocation(shaderProgram, "imageTexture");
  uCubeMapTexLoc = gl.getUniformLocation(shaderProgram, "cubeMapTexture");
  uLevelsLoc = gl.getUniformLocation(shaderProgram, "levels");
}

function initShaderBuffers() {
  cube = new Cube();
  sphere = new Sphere();
  sheet = new Sheet();
  kettle = new MeshObject(kettle_path);
  cube.initBuffer();
  sphere.initBuffer();
  sheet.initBuffer();
  kettle.initObject();
}

function webGLStart() {
  canvas = document.getElementById("canva");
  canvas.addEventListener(
    "wheel",
    (event) => {
      radius += event.deltaY * 0.005;
      radius = Math.max(2.0, radius);
      radius = Math.min(12.0, radius);
      event.preventDefault();
      animate();
    },
    false
  );

  if (!canvas) alert(`Canvas not found!`);
  gl = initGL(canvas);
  document.addEventListener("mousedown", onMouseDown, false);

  shaderProgram = initShaders(vertexShaderProgram, fragmentShaderProgram);
  initLocations();
  initShaderBuffers();

  gl.enable(gl.DEPTH_TEST);

  rcube_texture = initTextures(rcube_path);
  wood_texture = initTextures(wood_path);
  cubemap_texture = initCubeMap();
  sky_textures = initSkyTextures();
}
