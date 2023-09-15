//@ts-check

let fill_type;

class Square {
  constructor(
    uMMatrixLocation,
    aPositionLocation,
    uColorLocation,
    fillType = "fill"
  ) {
    this.vertices = new Float32Array([
      0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5,
    ]);
    this.indices = new Uint16Array([0, 1, 2, 0, 2, 3]);
    this.uMMatrixLocation = uMMatrixLocation;
    this.aPositionLocation = aPositionLocation;
    this.uColorLocation = uColorLocation;
    this.fillType = fillType;
    console.log(`Initialised Square in ${this.fillType}`);
  }
  /**
   * @param {Iterable<number>} mMatrix
   * @param {Iterable<number>} color
   */
  draw(mMatrix, color) {
    if (!this.uMMatrixLocation) alert("uMMatrixLocation uninitialized");
    initBuffer(this.vertices, gl.ARRAY_BUFFER);
    gl.vertexAttribPointer(this.aPositionLocation, 2, gl.FLOAT, false, 0, 0); // stride is 0?
    initBuffer(this.indices, gl.ELEMENT_ARRAY_BUFFER);
    gl.uniformMatrix4fv(this.uMMatrixLocation, false, mMatrix);
    gl.uniform4fv(this.uColorLocation, color);
    gl.drawElements(
      fill_type[this.fillType],
      this.indices.length,
      gl.UNSIGNED_SHORT,
      0
    );
  }
}

class Triangle {
  constructor(
    uMMatrixLocation,
    aPositionLocation,
    uColorLocation,
    fillType = "fill"
  ) {
    this.indices = new Uint16Array([0, 1, 2]);
    this.vertices = new Float32Array([0.0, 0.5, -0.5, -0.5, 0.5, -0.5]);
    this.indexBuffer = initBuffer(this.indices, gl.ELEMENT_ARRAY_BUFFER);
    this.positionBuffer = initBuffer(this.vertices, gl.ARRAY_BUFFER);
    this.uMMatrixLocation = uMMatrixLocation;
    this.aPositionLocation = aPositionLocation;
    this.uColorLocation = uColorLocation;
    this.fillType = fillType;
    console.log(`Initialised Triangle in ${this.fillType}`);
  }

  /**
   * @param {Iterable<number>} mMatrix
   * @param {Iterable<number>} color
   */
  draw(mMatrix, color) {
    if (!this.uMMatrixLocation) alert("uMMatrixLocation uninitialized");
    gl.uniformMatrix4fv(this.uMMatrixLocation, false, mMatrix);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.positionBuffer);
    gl.vertexAttribPointer(this.aPositionLocation, 2, gl.FLOAT, false, 0, 0);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.uniform4fv(this.uColorLocation, color);

    gl.drawElements(
      fill_type[this.fillType],
      this.indices.length,
      gl.UNSIGNED_SHORT,
      0
    );
  }
}

class Circle {
  constructor(
    uMMatrixLocation,
    aPositionLocation,
    uColorLocation,
    points = 60,
    fillType = "fill"
  ) {
    this.vertices = new Float32Array((points + 1) * 2).fill(0.0);
    for (let i = 1; i <= points; i += 1) {
      let currIdx = i * 2;
      let currDegrees = ((i - 1) * 360) / points;
      this.vertices[currIdx] = Math.cos(degToRad(currDegrees));
      this.vertices[currIdx + 1] = Math.sin(degToRad(currDegrees));
      // console.log(this.vertices[currIdx], this.vertices[currIdx + 1]);
    }
    this.indices = new Uint16Array(3 * points).fill(0);
    for (let i = 0; i < points; i += 1) {
      let currIdx = i * 3;
      this.indices[currIdx] = 0;
      this.indices[currIdx + 1] = i + 1;
      this.indices[currIdx + 2] = i + 2;
    }
    this.indices[this.indices.length - 1] = 1;
    // console.log(this.indices);
    this.indexBuffer = initBuffer(this.indices, gl.ELEMENT_ARRAY_BUFFER);
    this.positionBuffer = initBuffer(this.vertices, gl.ARRAY_BUFFER);
    this.uMMatrixLocation = uMMatrixLocation;
    this.aPositionLocation = aPositionLocation;
    this.uColorLocation = uColorLocation;
    this.fillType = fillType;
    console.log(`Initialised Circle in ${this.fillType}`);
  }

  /**
   * @param {Iterable<number>} mMatrix
   * @param {Iterable<number>} color
   */
  draw(mMatrix, color) {
    if (!this.uMMatrixLocation) alert("uMMatrixLocation uninitialized");
    gl.uniformMatrix4fv(this.uMMatrixLocation, false, mMatrix);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.positionBuffer);
    gl.vertexAttribPointer(this.aPositionLocation, 2, gl.FLOAT, false, 0, 0);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.uniform4fv(this.uColorLocation, color);

    gl.drawElements(
      fill_type[this.fillType],
      this.indices.length,
      gl.UNSIGNED_SHORT,
      0
    );
  }
}
