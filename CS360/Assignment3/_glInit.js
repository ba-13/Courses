"use strict";

/**
 * @param {HTMLCanvasElement} canvas
 * @returns {WebGL2RenderingContext}
 */
function initGL(canvas) {
  let gl;
  try {
    gl = canvas.getContext("webgl2");
  } catch (e) {
    alert("WebGL init failed!");
  }
  if (!gl) alert("WebGL init failed!");
  console.log("Received WebGL2 context");
  return gl;
}

/**
 *
 * @param {string} vertexShaderProgram
 * @returns {WebGLShader}
 */
function vertexShaderSetup(vertexShaderProgram) {
  let shader = gl.createShader(gl.VERTEX_SHADER);
  gl.shaderSource(shader, vertexShaderProgram);
  gl.compileShader(shader);
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert(`vCompilation: ${gl.getShaderInfoLog(shader)}`);
    return null;
  }
  console.log("Compiled vertexShader");
  return shader;
}

/**
 *
 * @param {string} fragmentShaderProgram
 * @returns {WebGLShader}
 */
function fragmentShaderSetup(fragmentShaderProgram) {
  let shader = gl.createShader(gl.FRAGMENT_SHADER);
  gl.shaderSource(shader, fragmentShaderProgram);
  gl.compileShader(shader);
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert(`fCompilation: ${gl.getShaderInfoLog(shader)}`);
    return null;
  }
  console.log("Compiled fragmentShader");
  return shader;
}

/**
 * Compiles and links vertex and fragment shaders
 * to a shader program, which is attached, linked and used.
 * @param {string} vertexShaderProgram
 * @param {string} fragmentShaderProgram
 * @returns {WebGLProgram}
 */
function initShaders(vertexShaderProgram, fragmentShaderProgram) {
  let shaderProgram = gl.createProgram();
  console.log("Created Shader Program");
  let vertexShader = vertexShaderSetup(vertexShaderProgram);
  let fragmentShader = fragmentShaderSetup(fragmentShaderProgram);
  console.log("Compiled Shader Programs");
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);
  console.log("Attached and Linked Shader Program");
  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    let vErrorLog = gl.getShaderInfoLog(vertexShader);
    let fErrorLog = gl.getShaderInfoLog(fragmentShader);
    alert(`Vertex Shader attachment status: ${vErrorLog}`);
    alert(`Fragment Shader attachment status: ${fErrorLog}`);
  }
  gl.useProgram(shaderProgram);
  console.log("Using Shader Program");
  return shaderProgram;
}

function initTextures(textureFile) {
  let tex = gl.createTexture();
  tex.image = new Image();
  tex.image.src = textureFile;
  tex.image.onload = function () {
    handleTextureLoaded(tex);
    animate();
  };
  return tex;
}

function handleTextureLoaded(texture) {
  gl.bindTexture(gl.TEXTURE_2D, texture);
  // gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1); // use it to flip Y if needed
  gl.texImage2D(
    gl.TEXTURE_2D, // 2D texture
    0, // mipmap level
    gl.RGB, // internal format
    gl.RGB, // format
    gl.UNSIGNED_BYTE, // type of data
    texture.image // array or <img>
  );

  gl.generateMipmap(gl.TEXTURE_2D);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
  gl.texParameteri(
    gl.TEXTURE_2D,
    gl.TEXTURE_MIN_FILTER,
    gl.LINEAR_MIPMAP_LINEAR
  );
}

let faceImages;

function initCubeMap() {
  let cubeMapPath = "./textures/Nvidia_cubemap/";
  faceImages = [
    {
      target: gl.TEXTURE_CUBE_MAP_POSITIVE_X,
      url: cubeMapPath.concat("posx.jpg"),
    },
    {
      target: gl.TEXTURE_CUBE_MAP_NEGATIVE_X,
      url: cubeMapPath.concat("negx.jpg"),
    },
    {
      target: gl.TEXTURE_CUBE_MAP_POSITIVE_Y,
      url: cubeMapPath.concat("posy.jpg"),
    },
    {
      target: gl.TEXTURE_CUBE_MAP_NEGATIVE_Y,
      url: cubeMapPath.concat("negy.jpg"),
    },
    {
      target: gl.TEXTURE_CUBE_MAP_POSITIVE_Z,
      url: cubeMapPath.concat("posz.jpg"),
    },
    {
      target: gl.TEXTURE_CUBE_MAP_NEGATIVE_Z,
      url: cubeMapPath.concat("negz.jpg"),
    },
  ];
  let cubemapTexture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTexture);

  faceImages.forEach((face) => {
    const { target, url } = face;
    // Upload the canvas to the cubemap face.
    const level = 0;
    const internalFormat = gl.RGBA;
    const width = 512;
    const height = 512;
    const format = gl.RGBA;
    const type = gl.UNSIGNED_BYTE;

    // setup each face so it's immediately renderable
    gl.texImage2D(
      target,
      level,
      internalFormat,
      width,
      height,
      0,
      format,
      type,
      null
    );

    let image = new Image();
    image.src = url;
    image.addEventListener("load", function () {
      // Now that the image has loaded upload it to the texture.
      gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTexture);
      gl.texImage2D(target, level, internalFormat, format, type, image);
      gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
      animate();
    });
  });

  gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
  gl.texParameteri(
    gl.TEXTURE_CUBE_MAP,
    gl.TEXTURE_MIN_FILTER,
    gl.LINEAR_MIPMAP_LINEAR
  );
  return cubemapTexture;
}

function initSkyTextures() {
  let skyTextures = [];
  for (let i = 0; i < 6; i++) {
    skyTextures.push(gl.createTexture());
    skyTextures[i].image = new Image();
    skyTextures[i].image.src = faceImages[i].url;
    skyTextures[i].image.onload = function () {
      handleTextureLoaded(skyTextures[i]);
    };
  }
  return skyTextures;
}
