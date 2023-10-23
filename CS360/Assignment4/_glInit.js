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
    gl = canvas.getContext("webgl2", { preserveDrawingBuffer: true });
  } catch (e) {
    console.error("Faced error initializing:", e);
  }
  if (!gl) {
    alert("WebGL initialization failed!");
  }
  console.log("Got GL2 Context");
}

/**
 * Decide how a triangle will be defined
 * @param {ArrayBuffer} array
 * @param {number} target
 */
function initBuffer(array, target) {
  let arrayBuffer = gl.createBuffer();
  gl.bindBuffer(target, arrayBuffer);
  gl.bufferData(target, array, gl.STATIC_DRAW);
  return arrayBuffer;
}

function handleTextureLoaded(texture, image) {
  gl.bindTexture(gl.TEXTURE_2D, texture);
  gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1); // use it to flip Y if needed
  gl.texImage2D(
    gl.TEXTURE_2D, // 2D texture
    0, // mipmap level
    gl.RGBA, // internal format
    gl.RGBA, // format
    gl.UNSIGNED_BYTE, // type of data
    image // array or <img>
  );

  gl.generateMipmap(gl.TEXTURE_2D);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
  gl.texParameteri(
    gl.TEXTURE_2D,
    gl.TEXTURE_MIN_FILTER,
    gl.LINEAR_MIPMAP_LINEAR
  );

  drawScene();
}

function initTextures(textureFilePath) {
  var tex = gl.createTexture();
  let image = new Image();
  image.src = textureFilePath;
  image.onload = function () {
    handleTextureLoaded(tex, image);
  };
  return tex;
}
