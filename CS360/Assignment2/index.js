//@ts-check

/** @type {WebGLRenderingContext} */
let gl;
let canvas;
/** @type {Cube} */
let cube;
/** @type {Sphere} */
let sphere;

let aPositionLocation;
let aNormalLocation;
let uMMatrixLocation;
let uVMatrixLocation;
let uPMatrixLocation;
let uColorLocation;
let uLightLocation;

let vMatrix = mat4.create(); // view matrix
let mMatrix = mat4.create(); // model matrix
let pMatrix = mat4.create(); // projection matrix

// specify camera/eye coordinate system parameters
let eyePos = [0.0, 0.0, 3.0];
let COI = [0.0, 0.0, 0.0];
let viewUp = [0.0, 1.0, 0.0];

// lightPos
let lightPos = [-1.3, 0.3, 3.0];

let degree1 = 0.0;
let degree0 = 0.0;

let prevMouseX = 0.0;
let prevMouseY = 0.0;

function drawScene() {
  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(0.9, 0.9, 0.95, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  // set up the view matrix, multiply into the modelview matrix
  mat4.identity(vMatrix);
  vMatrix = mat4.lookAt(eyePos, COI, viewUp, vMatrix);
  console.log(eyePos);

  //set up perspective projection matrix
  mat4.identity(pMatrix);
  mat4.perspective(50, 1.0, 0.1, 0, pMatrix);

  //set up the model matrix
  mat4.identity(mMatrix);
  mMatrix = mat4.rotate(mMatrix, degToRad(degree0), [0, 1, 0]);
  mMatrix = mat4.rotate(mMatrix, degToRad(degree1), [1, 0, 0]);
  // Now draw the cube
  let color = [0.8, 0.0, 0.0, 1.0]; // specify color for the cube
  cube.draw(mMatrix, vMatrix, pMatrix, color);
}

function webGLStart() {
  canvas = document.getElementById("canva");
  initGL(canvas); // initialises gl

  document.addEventListener("mousedown", onMouseDown, false);

  let shaderProgram = initShaders(vertexShaderCode, fragmentShaderCode);

  if (!shaderProgram) alert("shaders program didn't compile/link");

  aPositionLocation = gl.getAttribLocation(shaderProgram, "aPosition");
  aNormalLocation = gl.getAttribLocation(shaderProgram, "aNormal");
  uMMatrixLocation = gl.getUniformLocation(shaderProgram, "uMMatrix");
  uVMatrixLocation = gl.getUniformLocation(shaderProgram, "uVMatrix");
  uPMatrixLocation = gl.getUniformLocation(shaderProgram, "uPMatrix");
  uColorLocation = gl.getUniformLocation(shaderProgram, "uColor");
  uLightLocation = gl.getUniformLocation(shaderProgram, "lightLocation");
  console.log("Got Locations in shaderProgram");

  gl.enableVertexAttribArray(aPositionLocation);
  gl.enableVertexAttribArray(aNormalLocation);
  gl.enable(gl.DEPTH_TEST);
  console.log("Enabled Vertex Array from aPositionLocation");

  cube = new Cube(
    aPositionLocation,
    uColorLocation,
    aNormalLocation,
    uMMatrixLocation,
    uVMatrixLocation,
    uPMatrixLocation
  );

  sphere = new Sphere(
    aPositionLocation,
    uColorLocation,
    aNormalLocation,
    uMMatrixLocation,
    uVMatrixLocation,
    uPMatrixLocation
  );

  cube.initBuffer();
  sphere.initBuffer();

  // gl.depthFunc(gl.LEQUAL);

  drawScene();
}
