class Cube {
  constructor() {
    this.vertices = [
      // Front face
      -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5,
      // Back face
      -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5,
      // Top face
      -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5,
      // Bottom face
      -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5,
      // Right face
      0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5,
      // Left face
      -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5,
    ];
    this.normals = [
      // Front face
      0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
      // Back face
      0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
      // Top face
      0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
      // Bottom face
      0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
      // Right face
      1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
      // Left face
      -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
    ];
    this.indices = [
      0,
      1,
      2,
      0,
      2,
      3, // Front face
      4,
      5,
      6,
      4,
      6,
      7, // Back face
      8,
      9,
      10,
      8,
      10,
      11, // Top face
      12,
      13,
      14,
      12,
      14,
      15, // Bottom face
      16,
      17,
      18,
      16,
      18,
      19, // Right face
      20,
      21,
      22,
      20,
      22,
      23, // Left face
    ];
    this.texCoords = [
      // Front face
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
      // Back face
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
      // Top face
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
      // Bottom face
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
      // Right face
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
      // Left face
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
    ];
  }

  initBuffer() {
    this.vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.vertices),
      gl.STATIC_DRAW
    );
    this.vertexBufferitemSize = 3;
    this.vertexBuffernumItems = this.vertices.length / 3;

    this.normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.normals),
      gl.STATIC_DRAW
    );
    this.normalBufferitemSize = 3;
    this.normalBuffernumItems = this.vertices.length / 3;

    this.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.bufferData(
      gl.ELEMENT_ARRAY_BUFFER,
      new Uint16Array(this.indices),
      gl.STATIC_DRAW
    );
    this.indexBufferitemSize = 1;
    this.indexBuffernumItems = this.indices.length;

    // texCoord
    this.texCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.texCoords),
      gl.STATIC_DRAW
    );
    this.texCoordBufferitemSize = 2;
    this.texCoordBuffernumItems = this.texCoords.length / 2;
  }

  draw(mMatrix, vMatrix, pMatrix, color, texture, env_texture, levels) {
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.vertexAttribPointer(
      aPositionLoc,
      this.vertexBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aPositionLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.vertexAttribPointer(
      aNormalLoc,
      this.normalBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aNormalLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.vertexAttribPointer(
      aTexCoordLoc,
      this.texCoordBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aTexCoordLoc);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);

    gl.uniform4fv(uColorLoc, color);
    gl.uniform3fv(uLevelsLoc, levels);
    gl.uniformMatrix4fv(uMMatrixLoc, false, mMatrix);
    gl.uniformMatrix4fv(uVMatrixLoc, false, vMatrix);
    gl.uniformMatrix4fv(uPMatrixLoc, false, pMatrix);
    let nMatrix = mat4.transpose(mat4.inverse(mMatrix));
    gl.uniformMatrix4fv(uNMatrixLoc, false, nMatrix);

    // for texture binding
    gl.activeTexture(gl.TEXTURE1); // set texture unit 0 to use
    gl.bindTexture(gl.TEXTURE_2D, texture); // bind the texture object to the texture unit
    gl.uniform1i(uTextureLoc, 1); // pass the texture unit to the shader
    // for env cubemap texture binding
    gl.activeTexture(gl.TEXTURE5); // set texture unit 0 to use
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, env_texture); // bind the texture object to the texture unit
    gl.uniform1i(uCubeMapTexLoc, 5); // pass the texture unit to the shader

    gl.drawElements(gl.TRIANGLES, this.indices.length, gl.UNSIGNED_SHORT, 0);
  }
}

class Sphere {
  constructor() {
    this.indices = [];
    this.vertices = [];
    this.normals = [];
    this.texCoords = [];
    this.nslices = 30;
    this.nstacks = this.nslices;
    this.radius = 1.0;
    this.initSphere(this.nslices, this.nstacks, this.radius);
  }

  initSphere(nslices, nstacks, radius) {
    for (let i = 0; i <= nslices; i++) {
      let angle = (i * Math.PI) / nslices;
      let comp1 = Math.sin(angle);
      let comp2 = Math.cos(angle);

      for (let j = 0; j <= nstacks; j++) {
        let phi = (j * 2 * Math.PI) / nstacks;
        let comp3 = Math.sin(phi);
        let comp4 = Math.cos(phi);

        let xcood = comp4 * comp1;
        let ycoord = comp2;
        let zcoord = comp3 * comp1;
        let utex = 1 - j / nstacks;
        let vtex = 1 - i / nslices;

        this.vertices.push(radius * xcood, radius * ycoord, radius * zcoord);
        this.normals.push(xcood, ycoord, zcoord);
        this.texCoords.push(utex, vtex);
      }
    }

    // now compute the indices here
    for (let i = 0; i < nslices; i++) {
      for (let j = 0; j < nstacks; j++) {
        let id1 = i * (nstacks + 1) + j;
        let id2 = id1 + nstacks + 1;

        this.indices.push(id1, id2, id1 + 1);
        this.indices.push(id2, id2 + 1, id1 + 1);
      }
    }
  }

  initBuffer() {
    this.vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.vertices),
      gl.STATIC_DRAW
    );
    this.vertexBufferitemSize = 3;
    this.vertexBuffernumItems = this.nslices * this.nstacks;

    this.normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.normals),
      gl.STATIC_DRAW
    );
    this.normalBufferitemSize = 3;
    this.normalBuffernumItems = this.nslices * this.nstacks;

    this.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.bufferData(
      gl.ELEMENT_ARRAY_BUFFER,
      new Uint32Array(this.indices),
      gl.STATIC_DRAW
    );
    this.indexBufferitemSize = 1;
    this.indexBuffernumItems = (this.nstacks - 1) * 6 * (this.nslices + 1);

    // texture coordinates
    this.texCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.texCoords),
      gl.STATIC_DRAW
    );
    this.texCoordBufferitemSize = 2;
    this.texCoordBuffernumItems = this.texCoords.length / 2;
  }

  draw(mMatrix, vMatrix, pMatrix, color, texture, env_texture, levels) {
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.vertexAttribPointer(
      aPositionLoc,
      this.vertexBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aPositionLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.vertexAttribPointer(
      aNormalLoc,
      this.normalBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aNormalLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.vertexAttribPointer(
      aTexCoordLoc,
      this.texCoordBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aTexCoordLoc);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);

    gl.uniform4fv(uColorLoc, color);
    gl.uniform3fv(uLevelsLoc, levels);
    gl.uniformMatrix4fv(uMMatrixLoc, false, mMatrix);
    gl.uniformMatrix4fv(uVMatrixLoc, false, vMatrix);
    gl.uniformMatrix4fv(uPMatrixLoc, false, pMatrix);
    let nMatrix = mat4.transpose(mat4.inverse(mMatrix));
    gl.uniformMatrix4fv(uNMatrixLoc, false, nMatrix);

    // for texture binding
    gl.activeTexture(gl.TEXTURE1);
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.uniform1i(uTextureLoc, 1);
    // for env cubemap texture binding
    gl.activeTexture(gl.TEXTURE5);
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, env_texture);
    gl.uniform1i(uCubeMapTexLoc, 5);

    gl.drawElements(gl.TRIANGLES, this.indices.length, gl.UNSIGNED_INT, 0);
  }
}

class Sheet {
  constructor() {
    this.vertices = [
      -0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.5, 0.5, 0.0, -0.5, 0.5, 0.0,
    ];
    this.normals = [0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0];
    this.texCoords = [1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0];
    this.indices = [0, 1, 2, 0, 2, 3];
  }

  initBuffer() {
    this.vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.vertices),
      gl.STATIC_DRAW
    );
    this.vertexBufferitemSize = 3;
    this.vertexBuffernumItems = this.vertices.length / 3;

    this.normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.normals),
      gl.STATIC_DRAW
    );
    this.normalBufferitemSize = 3;
    this.normalBuffernumItems = this.normals.length / 3;

    this.texCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.texCoords),
      gl.STATIC_DRAW
    );
    this.texCoordBufferitemSize = 2;
    this.texCoordBuffernumItems = this.texCoords.length / 2;

    this.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.bufferData(
      gl.ELEMENT_ARRAY_BUFFER,
      new Uint32Array(this.indices),
      gl.STATIC_DRAW
    );
    this.indexBufferitemSize = 1;
    this.indexBuffernumItems = this.indices.length;
  }

  draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    texture,
    env_texture,
    levels = [0.0, 1.0, 0.0]
  ) {
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.vertexAttribPointer(
      aPositionLoc,
      this.vertexBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aPositionLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.vertexAttribPointer(
      aNormalLoc,
      this.normalBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aNormalLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.vertexAttribPointer(
      aTexCoordLoc,
      this.texCoordBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aTexCoordLoc);

    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.uniform3fv(uLevelsLoc, levels);
    gl.uniform4fv(uColorLoc, color);
    gl.uniformMatrix4fv(uMMatrixLoc, false, mMatrix);
    gl.uniformMatrix4fv(uVMatrixLoc, false, vMatrix);
    gl.uniformMatrix4fv(uPMatrixLoc, false, pMatrix);
    let nMatrix = mat4.transpose(mat4.inverse(mMatrix));
    gl.uniformMatrix4fv(uNMatrixLoc, false, nMatrix);

    // for texture binding
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.uniform1i(uTextureLoc, 0);
    // for env cubemap texture binding
    gl.activeTexture(gl.TEXTURE5);
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, env_texture);
    gl.uniform1i(uCubeMapTexLoc, 5);

    gl.drawElements(gl.TRIANGLES, this.indexBuffernumItems, gl.UNSIGNED_INT, 0);
  }
}

class MeshObject {
  constructor(input_JSON) {
    this.input_JSON = input_JSON;
  }

  processObject(objData) {
    this.vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(objData.vertexPositions),
      gl.STATIC_DRAW
    );
    this.vertexBufferitemSize = 3;
    this.vertexBuffernumItems = objData.vertexPositions.length / 3;

    this.normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(objData.vertexNormals),
      gl.STATIC_DRAW
    );
    this.normalBufferitemSize = 3;
    this.normalBuffernumItems = objData.vertexNormals.length / 3;

    this.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.bufferData(
      gl.ELEMENT_ARRAY_BUFFER,
      new Uint32Array(objData.indices),
      gl.STATIC_DRAW
    );
    this.indexBufferitemSize = 1;
    this.indexBuffernumItems = objData.indices.length;

    this.texCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(objData.vertexTextureCoords),
      gl.STATIC_DRAW
    );
    this.texCoordBufferitemSize = 2;
    this.texCoordBuffernumItems = objData.vertexTextureCoords.length / 2;
  }

  initObject() {
    // XMLHttpRequest objects are used to interact with servers
    // It can be used to retrieve any type of data, not just XML.
    let request = new XMLHttpRequest();
    request.open("GET", this.input_JSON);
    // MIME: Multipurpose Internet Mail Extensions
    // It lets users exchange different kinds of data files
    request.overrideMimeType("application/json");
    request.onreadystatechange = () => {
      //request.readyState == 4 means operation is done
      if (request.readyState == 4) {
        this.processObject(JSON.parse(request.responseText));
      }
    };
    request.send();
  }

  draw(
    mMatrix,
    vMatrix,
    pMatrix,
    color,
    texture,
    env_texture,
    levels = [0.0, 0.0, 0.0]
  ) {
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.vertexAttribPointer(
      aPositionLoc,
      this.vertexBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aPositionLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.vertexAttribPointer(
      aNormalLoc,
      this.normalBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.enableVertexAttribArray(aNormalLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.texCoordBuffer);
    gl.vertexAttribPointer(
      aTexCoordLoc,
      this.texCoordBufferitemSize,
      gl.FLOAT,
      false,
      0,
      0
    );

    gl.enableVertexAttribArray(aTexCoordLoc);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);

    gl.uniform3fv(uLevelsLoc, levels);
    gl.uniform4fv(uColorLoc, color);
    gl.uniformMatrix4fv(uMMatrixLoc, false, mMatrix);
    gl.uniformMatrix4fv(uVMatrixLoc, false, vMatrix);
    gl.uniformMatrix4fv(uPMatrixLoc, false, pMatrix);
    let nMatrix = mat4.transpose(mat4.inverse(mMatrix));
    gl.uniformMatrix4fv(uNMatrixLoc, false, nMatrix);

    // for texture binding
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.uniform1i(uTextureLoc, 0);
    // for env cubemap texture binding
    gl.activeTexture(gl.TEXTURE5);
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, env_texture);
    gl.uniform1i(uCubeMapTexLoc, 5);

    gl.drawElements(gl.TRIANGLES, this.indexBuffernumItems, gl.UNSIGNED_INT, 0);
  }
}
