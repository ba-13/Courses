class Cube {
  constructor(
    aPositionLocation,
    uColorLocation,
    aNormalLocation,
    uMMatrixLocation,
    uVMatrixLocation,
    uPMatrixLocation
  ) {
    this.aPositionLocation = aPositionLocation;
    this.uColorLocation = uColorLocation;
    this.aNormalLocation = aNormalLocation;
    this.uMMatrixLocation = uMMatrixLocation;
    this.uVMatrixLocation = uVMatrixLocation;
    this.uPMatrixLocation = uPMatrixLocation;
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
  }

  initBuffer() {
    this.vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.vertices),
      gl.STATIC_DRAW
    );
    this.vertexBuffer.itemSize = 3;
    this.vertexBuffer.numItems = this.vertices.length / 3;

    this.normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.normals),
      gl.STATIC_DRAW
    );
    this.normalBuffer.itemSize = 3;
    this.normalBuffer.numItems = this.vertices.length / 3;

    this.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.bufferData(
      gl.ELEMENT_ARRAY_BUFFER,
      new Uint16Array(this.indices),
      gl.STATIC_DRAW
    );
    this.indexBuffer.itemSize = 1;
    this.indexBuffer.numItems = this.indices.length;
    console.log("Initialized cube buffer");
  }

  draw(mMatrix, vMatrix, pMatrix, color) {
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.vertexAttribPointer(
      this.aPositionLocation,
      this.vertexBuffer.itemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.vertexAttribPointer(
      this.aNormalLocation,
      this.normalBuffer.itemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);

    gl.uniform3fv(uLightLocation, lightPos);
    gl.uniform4fv(this.uColorLocation, color);
    gl.uniformMatrix4fv(this.uMMatrixLocation, false, mMatrix);
    gl.uniformMatrix4fv(this.uVMatrixLocation, false, vMatrix);
    gl.uniformMatrix4fv(this.uPMatrixLocation, false, pMatrix);

    gl.drawElements(gl.TRIANGLES, this.indices.length, gl.UNSIGNED_SHORT, 0);
  }
}

class Sphere {
  constructor(
    aPositionLocation,
    uColorLocation,
    aNormalLocation,
    uMMatrixLocation,
    uVMatrixLocation,
    uPMatrixLocation
  ) {
    this.aPositionLocation = aPositionLocation;
    this.uColorLocation = uColorLocation;
    this.aNormalLocation = aNormalLocation;
    this.uMMatrixLocation = uMMatrixLocation;
    this.uVMatrixLocation = uVMatrixLocation;
    this.uPMatrixLocation = uPMatrixLocation;
    this.indices = [];
    this.vertices = [];
    this.normals = [];
    this.nslices = 12;
    this.nstacks = this.nslices;
    this.radius = 1.0;
    this.initSphere(this.nslices, this.nstacks, this.radius);
  }

  initSphere(nslices, nstacks, radius) {
    let theta1, theta2;

    // SOUTH POLE
    for (let i = 0; i < nslices; i++) {
      this.vertices.push(0);
      this.vertices.push(-radius);
      this.vertices.push(0);

      this.normals.push(0);
      this.normals.push(-1.0);
      this.normals.push(0);
    }

    for (let j = 1; j < nstacks - 1; j++) {
      theta1 = (j * 2 * Math.PI) / nslices - Math.PI / 2;
      for (let i = 0; i < nslices; i++) {
        theta2 = (i * 2 * Math.PI) / nslices;
        this.vertices.push(radius * Math.cos(theta1) * Math.cos(theta2));
        this.vertices.push(radius * Math.sin(theta1));
        this.vertices.push(radius * Math.cos(theta1) * Math.sin(theta2));

        this.normals.push(Math.cos(theta1) * Math.cos(theta2));
        this.normals.push(Math.sin(theta1));
        this.normals.push(Math.cos(theta1) * Math.sin(theta2));
      }
    }

    // NORTH POLE
    for (let i = 0; i < nslices; i++) {
      this.vertices.push(0);
      this.vertices.push(radius);
      this.vertices.push(0);

      this.normals.push(0);
      this.normals.push(1.0);
      this.normals.push(0);
    }

    // setup the connectivity and indices
    for (let j = 0; j < nstacks - 1; j++)
      for (let i = 0; i <= nslices; i++) {
        let mi = i % nslices;
        let mi2 = (i + 1) % nslices;
        let idx = (j + 1) * nslices + mi;
        let idx2 = j * nslices + mi;
        let idx3 = j * nslices + mi2;
        let idx4 = (j + 1) * nslices + mi;
        let idx5 = j * nslices + mi2;
        let idx6 = (j + 1) * nslices + mi2;

        this.indices.push(idx);
        this.indices.push(idx2);
        this.indices.push(idx3);
        this.indices.push(idx4);
        this.indices.push(idx5);
        this.indices.push(idx6);
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
    this.vertexBuffer.itemSize = 3;
    this.vertexBuffer.numItems = this.nslices * this.nstacks;

    this.normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array(this.normals),
      gl.STATIC_DRAW
    );
    this.normalBuffer.itemSize = 3;
    this.normalBuffer.numItems = this.nslices * this.nstacks;

    this.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.bufferData(
      gl.ELEMENT_ARRAY_BUFFER,
      new Uint32Array(this.indices),
      gl.STATIC_DRAW
    );
    this.indexBuffer.itemSize = 1;
    this.indexBuffer.numItems = (this.nstacks - 1) * 6 * (this.nslices + 1);
    console.log("Initialized sphere buffer");
  }

  draw(mMatrix, vMatrix, pMatrix, color) {
    gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexBuffer);
    gl.vertexAttribPointer(
      this.aPositionLocation,
      this.vertexBuffer.itemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.bindBuffer(gl.ARRAY_BUFFER, this.normalBuffer);
    gl.vertexAttribPointer(
      this.aNormalLocation,
      this.normalBuffer.itemSize,
      gl.FLOAT,
      false,
      0,
      0
    );
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);

    gl.uniform4fv(this.uColorLocation, color);
    gl.uniformMatrix4fv(this.uMMatrixLocation, false, mMatrix);
    gl.uniformMatrix4fv(this.uVMatrixLocation, false, vMatrix);
    gl.uniformMatrix4fv(this.uPMatrixLocation, false, pMatrix);

    gl.drawElements(gl.TRIANGLES, this.indices.length, gl.UNSIGNED_INT, 0);
  }
}
