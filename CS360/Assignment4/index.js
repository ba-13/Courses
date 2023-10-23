//@ts-check

/** @type {WebGLRenderingContext} */
let gl;
let canvas;
/** @type {Square} */
let square;

let aPositionLocation;
let uMMatrixLocation;
let uColorLocation;
let aTexCoordLocation;
let uForeTextureLocation;
let uBackTextureLocation;
let uTypeLocation;
let uModeLocation;
let uFilterLocation;
let imageContrastLocation;
let imageBrightnessLocation;
let uPostProcessLocation;
let backgroundheightLocation;
let backgroundwidthLocation;

let backgroundTexture;
let foregroundTexture;

let mMatrix = mat4.create();
mat4.identity(mMatrix);

function drawScene() {
  if (!canvas) alert("Canvas not initialised");
  /** @type {[number, number, number, number]} */
  let colors = hexToColorArray("#282828"); // #80cafa sky blue

  let matrixStack = [];

  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(...colors);
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  pushMatrix(matrixStack, mMatrix);
  colors = hexToColorArray("#80cafa");
  mMatrix = mat4.translate(mMatrix, [0.0, 0.0, 0.0]);
  mMatrix = mat4.scale(mMatrix, [2.0, 2.0, 1.0]);
  square.draw(mMatrix, colors, backgroundTexture, foregroundTexture);
  mMatrix = popMatrix(matrixStack);
}

let prevPostProcess = 0;
function postProcessSelection(val) {
  let selectedFilter = document.querySelector(
    `input[name="post-process"]:checked`
  );
  switch (val) {
    case 0:
      if (selectedFilter) selectedFilter.checked = false;
    default:
      if (val == prevPostProcess) {
        prevPostProcess = -1;
        val = 0;
        if (selectedFilter) selectedFilter.checked = false;
      } else prevPostProcess = val;
  }
  if (selectedFilter)
    console.log(`Post-Process Selection: ${selectedFilter.id}, sending ${val}`);
  else console.log(`Post-Process Selection: None selected, sending ${val}`);
  gl.uniform1i(uPostProcessLocation, val);
  drawScene();
}

let prevFilterProcess = 0;
function filterSelection(val) {
  let selectedFilter = document.querySelector(`input[name="filter"]:checked`);
  switch (val) {
    case 0:
      if (selectedFilter) selectedFilter.checked = false;
    default:
      if (val == prevPostProcess) {
        prevPostProcess = -1;
        val = 0;
        if (selectedFilter) selectedFilter.checked = false;
      } else prevPostProcess = val;
  }
  if (selectedFilter)
    console.log(`Filter Selection: ${selectedFilter.id}, sending ${val}`);
  else console.log(`Filter Selection: None selected, sending ${val}`);
  gl.uniform1i(uFilterLocation, val);
  drawScene();
}

let prevModeProcess = 0;
function modeSelection(val) {
  let selectedFilter = document.querySelector(
    `input[name="image-mode"]:checked`
  );
  switch (val) {
    case 0:
      if (selectedFilter) selectedFilter.checked = false;
    default:
      if (val == prevPostProcess) {
        prevPostProcess = -1;
        val = 0;
        if (selectedFilter) selectedFilter.checked = false;
      } else prevPostProcess = val;
  }
  if (selectedFilter)
    console.log(`Mode Selection: ${selectedFilter.id}, sending ${val}`);
  else console.log(`Mode Selection: None selected, sending ${val}`);
  gl.uniform1i(uModeLocation, val);
  drawScene();
}

function contrastSliderProcess(val) {
  val = parseFloat(`${val}`);
  console.log(`Contrast Slider Value: ${val}`);
  gl.uniform1f(imageContrastLocation, val);
  drawScene();
}

function brightnessSliderProcess(val) {
  val = parseFloat(`${val}`);
  console.log(`Brightness Slider Value: ${val}`);
  gl.uniform1f(imageBrightnessLocation, val);
  drawScene();
}

function webGLStart() {
  canvas = document.getElementById("canva");
  initGL(canvas); // initialises gl

  let shaderProgram = initShaders(vertexShaderCode, fragmentShaderCode);
  if (!shaderProgram) alert("shaders program didn't compile/link");

  aPositionLocation = gl.getAttribLocation(shaderProgram, "aPosition");
  aTexCoordLocation = gl.getAttribLocation(shaderProgram, "aTexCoord");
  uTypeLocation = gl.getUniformLocation(shaderProgram, "type");
  gl.uniform1i(uTypeLocation, 0);
  uMMatrixLocation = gl.getUniformLocation(shaderProgram, "uMMatrix");
  uColorLocation = gl.getUniformLocation(shaderProgram, "uColor");
  uForeTextureLocation = gl.getUniformLocation(
    shaderProgram,
    "foreImageTexture"
  );
  uBackTextureLocation = gl.getUniformLocation(
    shaderProgram,
    "backImageTexture"
  );
  uPostProcessLocation = gl.getUniformLocation(
    shaderProgram,
    "backgroundProcess"
  );
  uModeLocation = gl.getUniformLocation(shaderProgram, "imageMode");
  uFilterLocation = gl.getUniformLocation(shaderProgram, "imageColor");
  imageContrastLocation = gl.getUniformLocation(shaderProgram, "imageContrast");
  imageBrightnessLocation = gl.getUniformLocation(
    shaderProgram,
    "imageBrightness"
  );
  backgroundheightLocation = gl.getUniformLocation(shaderProgram, "height");
  backgroundwidthLocation = gl.getUniformLocation(shaderProgram, "width");
  console.log("Got Uniform Locations in shaderProgram");

  gl.enableVertexAttribArray(aPositionLocation);
  gl.enableVertexAttribArray(aTexCoordLocation);
  console.log("Enabled Vertex Array from aPositionLocation");
  console.log("Enabled Vertex Array from aTexCoordLocation");

  square = new Square();
  drawScene();
}
