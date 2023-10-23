//@ts-check

class Square {
  constructor() {
    this.vertices = new Float32Array([
      0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5,
    ]);
    this.indices = new Uint16Array([0, 1, 2, 0, 2, 3]);
    this.texCoords = new Float32Array([1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0]);
    console.log(`Initialised Square`);
  }
  /**
   * @param {Iterable<number>} mMatrix
   * @param {Iterable<number>} color
   */
  draw(mMatrix, color, backTexture = null, foreTexture = null) {
    if (!uMMatrixLocation) alert("uMMatrixLocation uninitialized");
    initBuffer(this.vertices, gl.ARRAY_BUFFER);
    gl.vertexAttribPointer(aPositionLocation, 2, gl.FLOAT, false, 0, 0); // stride is 0?
    initBuffer(this.indices, gl.ELEMENT_ARRAY_BUFFER);
    initBuffer(this.texCoords, gl.ARRAY_BUFFER);
    gl.vertexAttribPointer(aTexCoordLocation, 2, gl.FLOAT, false, 0, 0);
    gl.uniformMatrix4fv(uMMatrixLocation, false, mMatrix);
    gl.uniform4fv(uColorLocation, color);
    let type = 0;
    // for backTexture binding
    if (backTexture) {
      gl.activeTexture(gl.TEXTURE0);
      gl.bindTexture(gl.TEXTURE_2D, backTexture);
      gl.uniform1i(uBackTextureLocation, 0);
      type += 1;
    }
    // for foreTexture binding
    if (foreTexture) {
      gl.activeTexture(gl.TEXTURE1);
      gl.bindTexture(gl.TEXTURE_2D, foreTexture);
      gl.uniform1i(uForeTextureLocation, 1);
      type += 2;
    }
    gl.uniform1i(uTypeLocation, type);
    console.log(`Drawing square with type ${type}`);

    gl.drawElements(gl.TRIANGLES, this.indices.length, gl.UNSIGNED_SHORT, 0);
  }
}
