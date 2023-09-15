//@ts-check

/**
 * @param {string} vertexShaderCode
 */
function vertexShaderSetup(vertexShaderCode) {
  let shader = gl.createShader(gl.VERTEX_SHADER);
  gl.shaderSource(shader, vertexShaderCode);
  gl.compileShader(shader);
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert(`Compilation: ${gl.getShaderInfoLog(shader)}`);
    return null;
  }
  console.log("Compiled vertexShader");
  return shader;
}

/**
 * @param {string} fragmentShaderCode
 */
function fragmentShaderSetup(fragmentShaderCode) {
  let shader = gl.createShader(gl.FRAGMENT_SHADER);
  gl.shaderSource(shader, fragmentShaderCode);
  gl.compileShader(shader);
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert(`Compilation: ${gl.getShaderInfoLog(shader)}`);
    return null;
  }
  console.log("Compiled fragmentShader");
  return shader;
}

/**
 * @param {string} vertexShaderCode
 * @param {string} fragmentShaderCode
 */
function initShaders(vertexShaderCode, fragmentShaderCode) {
  let shaderProgram = gl.createProgram();
  console.log("Created shaderProgram");

  let vertexShader = vertexShaderSetup(vertexShaderCode);
  let fragmentShader = fragmentShaderSetup(fragmentShaderCode);

  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  console.log("Attach shaders to shaderProgram");
  gl.linkProgram(shaderProgram);
  console.log("Link shaderProgram");

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert(gl.getShaderInfoLog(vertexShader));
    alert(gl.getShaderInfoLog(fragmentShader));
  }

  gl.useProgram(shaderProgram);
  console.log("Using shaderProgram");
  return shaderProgram;
}

function initGL(canvas) {
  try {
    gl = canvas.getContext("webgl2");
  } catch (e) {
    console.error("Faced error initializing:", e);
  }
  if (!gl) {
    alert("WebGL initialization failed!");
  }
  console.log("Got GL2 Context");
}

/**
 * Decide how a buffer is allocated to a vertex
 * @param {ArrayBuffer} array
 * @param {number} target
 */
function initBuffer(array, target, index, rowLen) {
  let arrayBuffer = gl.createBuffer();
  gl.bindBuffer(target, arrayBuffer);
  gl.bufferData(target, array, gl.STATIC_DRAW);
  if (index !== null)
    gl.vertexAttribPointer(index, rowLen, gl.FLOAT, false, 0, 0);
  return arrayBuffer;
}
