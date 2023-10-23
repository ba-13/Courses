//@ts-check

/**
 * @param {string} hexCode
 * @returns {Array}
 */
function hexToColorArray(hexCode, alpha = 1.0) {
  // Remove the '#' character if present
  hexCode = hexCode.replace("#", "");

  // Parse the hexCode values for red, green, and blue components
  const red = parseInt(hexCode.substring(0, 2), 16);
  const green = parseInt(hexCode.substring(2, 4), 16);
  const blue = parseInt(hexCode.substring(4, 6), 16);

  // Convert the values to the desired format (0.0 to 1.0)
  /** @type {[number, number, number, number]} */
  const colorArray = [red / 255.0, green / 255.0, blue / 255.0, alpha];
  return colorArray;
}

/**
 * @param {any[]} stack
 * @param {any} M
 */
function pushMatrix(stack, M) {
  let copy = mat4.create(M);
  stack.push(copy);
}

/**
 * @param {any[]} stack
 * @returns {any}
 */
function popMatrix(stack) {
  if (stack.length > 0) return stack.pop();
  else console.warn("Stack has no matrix to pop!");
}

/**
 * @param {number} degrees
 * @returns {number}
 */
function degToRad(degrees) {
  return (degrees * Math.PI) / 180;
}
