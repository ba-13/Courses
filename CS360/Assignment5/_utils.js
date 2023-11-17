//@ts-check

/**
 * @param {string} hexCode
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
 * @param {number} degrees
 */
function degToRad(degrees) {
  return (degrees * Math.PI) / 180;
}
