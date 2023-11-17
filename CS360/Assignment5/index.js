//@ts-check

/** @type {WebGLRenderingContext} */
let gl;
let canvas;
let square;

let aPositionLoc;
let uColorLoc;
let uStateLoc;
let uLightLoc;
let uBounceLoc;
let uCameraLoc;

let radius = 5.0;
let degree0 = 0;
let degree1 = 90;

let cameraPos = [null, null, null];
let lightPos = [0.0, 100.0, 20.0];
let mapping = {
  0: "Phong",
  1: "Phong + Shadow",
  2: "Phong + Reflection",
  3: "Phong + Shadow + Reflection",
};
let state = 0;
let bounce = 1;

class Square {
  constructor() {
    this.vertices = new Float32Array([
      1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0,
      // 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5,
    ]);
    this.indices = new Uint16Array([0, 1, 2, 0, 2, 3]);
    console.log(`Initialised Square`);
  }
  /**
   * @param {Iterable<number>} color
   */
  draw(color) {
    initBuffer(this.vertices, gl.ARRAY_BUFFER);
    gl.vertexAttribPointer(aPositionLoc, 2, gl.FLOAT, false, 0, 0); // stride is 0?
    initBuffer(this.indices, gl.ELEMENT_ARRAY_BUFFER);
    gl.uniform4fv(uColorLoc, color);
    gl.drawElements(gl.TRIANGLES, this.indices.length, gl.UNSIGNED_SHORT, 0);
  }
}

function drawScene() {
  if (!canvas) alert("Canvas not initialised");
  /** @type {[number, number, number, number]} */
  let colors = hexToColorArray("#282828");

  let deg0 = degToRad(degree0);
  let deg1 = degToRad(degree1);
  cameraPos = [
    radius * Math.cos(deg0) * Math.cos(deg1),
    radius * Math.sin(deg0) * Math.cos(deg1),
    radius * Math.sin(deg1),
  ];
  // console.log(`deg0: ${degree0} deg1: ${degree1}`);

  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(...colors);
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  // send uniforms
  gl.uniform1i(uStateLoc, state);
  gl.uniform1i(uBounceLoc, bounce);
  gl.uniform3fv(uLightLoc, new Float32Array(lightPos));
  gl.uniform3fv(uCameraLoc, new Float32Array(cameraPos));
  colors = hexToColorArray("#000000");
  square.draw(colors);
}

function changeState(st) {
  state = st;
  console.log(`Switching to ${mapping[st]}`);
  drawScene();
}

function changeBounce(bnc) {
  bounce = bnc;
  // console.log(`Shifting to bounce ${bnc}`);
  drawScene();
}

function changeLightX(pos) {
  lightPos[0] = pos;
  // console.log(`Changing lightX to ${lightPos[0]}`);
  drawScene();
}

function webGLStart() {
  canvas = document.getElementById("canva");
  initGL(canvas); // initialises gl

  let shaderProgram = initShaders(vertexShaderCode, fragmentShaderCode);
  if (!shaderProgram) alert("shaders program didn't compile/link");
  document.addEventListener("mousedown", onMouseDown, false);

  aPositionLoc = gl.getAttribLocation(shaderProgram, "aPosition");
  uColorLoc = gl.getUniformLocation(shaderProgram, "uColor");
  uStateLoc = gl.getUniformLocation(shaderProgram, "uState");
  uLightLoc = gl.getUniformLocation(shaderProgram, "uLightPos");
  uCameraLoc = gl.getUniformLocation(shaderProgram, "uCameraPos");
  uBounceLoc = gl.getUniformLocation(shaderProgram, "uBounce");
  gl.enableVertexAttribArray(aPositionLoc);

  square = new Square();
  drawScene();
}
