/**
 * Accepts: \
 *  aPosition, aNormal, aTexCoord \
 *  uMMatrix, uVMatrix, uPMatrix, uNMatrix, \
 * Sends to fragment Shader: \
 *  normal, \
 *  Vmatrix, \
 *  posInEyeSpace
 */
const vertexShaderProgram = `#version 300 es
in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;
uniform mat4 uMMatrix;
uniform mat4 uPMatrix;
uniform mat4 uVMatrix;
uniform mat4 uNMatrix;
out vec3 normal; // interpolate normal per fragment
out mat4 Vmatrix;
out vec3 posInEyeSpace;
out vec2 fragTexCoord;
out vec3 v_worldPosition;
out vec3 v_worldNormal;

void main() {
  float alpha = 20.0;
  mat4 projectionModelView = uPMatrix * uVMatrix * uMMatrix;
  gl_Position = projectionModelView * vec4(aPosition, 1.0);
  gl_PointSize = 3.0;
  posInEyeSpace = (uVMatrix * uMMatrix * vec4(aPosition, 1.0)).xyz;
  normal = normalize(uNMatrix * vec4(aNormal, 0.0)).xyz;
  Vmatrix = uVMatrix;
  fragTexCoord = aTexCoord;
  v_worldPosition = mat3(uMMatrix) * aPosition;
  v_worldNormal = mat3(uNMatrix) * aNormal;
}`;

/**
 * Accepts: \
 *  uColor, uLightPos, imageTexture \
 * Accepts from vertex shader: \
 *  Vmatrix, normal, posInEyeSpace
 */
const fragmentShaderProgram = `#version 300 es
precision mediump float;
out vec4 fragColor;
in mat4 Vmatrix;
in vec3 posInEyeSpace;
in vec3 normal;
in vec2 fragTexCoord;
in vec3 v_worldPosition;
in vec3 v_worldNormal;
uniform vec4 uColor;
uniform vec3 uEyePos;
uniform vec3 uLightPos;
uniform vec3 levels;
uniform sampler2D imageTexture;
uniform samplerCube cubeMapTexture;

void main() {
  float alpha = 100.0;
  vec3 L = normalize((Vmatrix * vec4(uLightPos, 1.0)).xyz - posInEyeSpace);
  vec3 R = normalize(-reflect(L, normal));
  vec3 V = normalize(-posInEyeSpace);
  float cos_theta = max(dot(normal, L), 0.0);
  float cos_phi = max(dot(V, R), 0.0);
  vec3 ldiff = 4.0 * cos_theta * vec3(uColor);
  vec3 lspec = 4.0 * (pow(cos_phi, alpha)) * vec3(1.0, 1.0, 1.0);
  vec3 lamb = 0.5 * (vec3(uColor));
  vec4 phong_color = vec4(ldiff + lamb + lspec, 1.0);
  vec4 texture_color = texture(imageTexture, fragTexCoord);

  vec3 worldNormal = normalize(v_worldNormal);
  vec3 eyeToSurfaceDir = normalize(v_worldPosition - uEyePos);
  vec3 reflectEyeToSurfaceDir = reflect(eyeToSurfaceDir, worldNormal);
  vec3 refractEyeToSurfaceDir = refract(eyeToSurfaceDir, worldNormal, 0.82);

  vec4 env_reflect_color = texture(cubeMapTexture, reflectEyeToSurfaceDir);
  vec4 env_refract_color = texture(cubeMapTexture, refractEyeToSurfaceDir);

  fragColor = levels[0] * phong_color + levels[1] * texture_color + levels[2] * env_refract_color + (1.0 - levels[0] - levels[1] - levels[2]) * env_reflect_color;
  // fragColor = texture_color;
}`;
