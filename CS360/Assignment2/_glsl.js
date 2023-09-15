// @ts-check

const fragShaderCode_flat = `#version 300 es
precision mediump float;
out vec4 fragColor;
in mat4 Vmatrix;
in vec3 posInEyeSpace;
uniform vec4 objColor;
uniform vec3 lightLocation;

void main() {
  float alpha = 16.0;
  vec3 normal = normalize(cross(dFdx(posInEyeSpace), dFdy(posInEyeSpace)));
  vec3 L = normalize((Vmatrix * vec4(lightLocation, 1.0)).xyz - posInEyeSpace);
  vec3 R = normalize(-reflect(L, normal));
  vec3 V = normalize(-posInEyeSpace);
  float cos_theta = max(dot(normal, L), 0.0);
  float cos_phi = max(dot(V, R), 0.0);
  vec3 ldiff = 1.0 * cos_theta * vec3(objColor);
  vec3 lspec = 1.0 * (pow(cos_phi, alpha)) * vec3(1.0, 1.0, 1.0);
  vec3 lamb = 0.2 * (vec3(objColor));
  fragColor = vec4(ldiff + lamb + lspec, 1.0);
}`;

const vertexShaderCode_flat = `#version 300 es
in vec3 aPosition;
uniform mat4 uMMatrix;
uniform mat4 uPMatrix;
uniform mat4 uVMatrix;
uniform mat4 uNMatrix;
out mat4 Vmatrix;
out vec3 posInEyeSpace;

void main() {
  mat4 projectionModelView;
	projectionModelView = uPMatrix * uVMatrix * uMMatrix;
  gl_Position = projectionModelView * vec4(aPosition, 1.0);
  gl_PointSize = 3.0;
  posInEyeSpace = (uVMatrix * uMMatrix * vec4(aPosition, 1.0)).xyz;
  Vmatrix = uVMatrix;
}`;

const fragShaderCode_gouraud = `#version 300 es
precision mediump float;
out vec4 fragColor;
in vec4 uColor;

void main() {
  fragColor = uColor;
}`;

const vertexShaderCode_gouraud = `#version 300 es
in vec3 aPosition;
in vec3 aNormal;
uniform mat4 uMMatrix;
uniform mat4 uPMatrix;
uniform mat4 uVMatrix;
uniform mat4 uNMatrix;
uniform vec3 lightLocation;
uniform vec4 objColor;
out vec4 uColor;

void main() {
  float alpha = 20.0;
  mat4 projectionModelView = uPMatrix * uVMatrix * uMMatrix;
  gl_Position = projectionModelView * vec4(aPosition, 1.0);
  gl_PointSize = 3.0;
  vec3 posInEyeSpace = (uVMatrix * uMMatrix * vec4(aPosition, 1.0)).xyz;
  vec3 normal = normalize(uNMatrix * vec4(aNormal, 0.0)).xyz;
  vec3 L = normalize(vec3(uVMatrix * vec4(lightLocation, 1.0)) - posInEyeSpace);
  vec3 R = normalize(-reflect(L, normal));
  vec3 V = normalize(-posInEyeSpace);
  float cos_theta = max(dot(normal, L), 0.0);
  float cos_phi = max(dot(V, R), 0.0);
  vec3 ldiff = 1.0 * cos_theta * vec3(objColor);
  vec3 lspec = 1.0 * (pow(cos_phi, alpha)) * vec3(1.0, 1.0, 1.0);
  vec3 lamb = 0.2 * (vec3(objColor));
  uColor = vec4(ldiff + lamb + lspec, 1.0);
}`;

const fragShaderCode_phong = `#version 300 es
precision mediump float;
out vec4 fragColor;
in mat4 Vmatrix;
in vec3 posInEyeSpace;
in vec3 normal;
uniform vec4 objColor;
uniform vec3 lightLocation;

void main() {
  float alpha = 16.0;
  vec3 L = normalize((Vmatrix * vec4(lightLocation, 1.0)).xyz - posInEyeSpace);
  vec3 R = normalize(-reflect(L, normal));
  vec3 V = normalize(-posInEyeSpace);
  float cos_theta = max(dot(normal, L), 0.0);
  float cos_phi = max(dot(V, R), 0.0);
  vec3 ldiff = 1.0 * cos_theta * vec3(objColor);
  vec3 lspec = 1.0 * (pow(cos_phi, alpha)) * vec3(1.0, 1.0, 1.0);
  vec3 lamb = 0.2 * (vec3(objColor));
  fragColor = vec4(ldiff + lamb + lspec, 1.0);
}`;

const vertexShaderCode_phong = `#version 300 es
in vec3 aPosition;
in vec3 aNormal;
uniform mat4 uMMatrix;
uniform mat4 uPMatrix;
uniform mat4 uVMatrix;
uniform mat4 uNMatrix;
out vec3 normal; // interpolate normal per fragment
out mat4 Vmatrix;
out vec3 posInEyeSpace;

void main() {
  float alpha = 20.0;
  mat4 projectionModelView = uPMatrix * uVMatrix * uMMatrix;
  gl_Position = projectionModelView * vec4(aPosition, 1.0);
  gl_PointSize = 3.0;
  posInEyeSpace = (uVMatrix * uMMatrix * vec4(aPosition, 1.0)).xyz;
  normal = normalize(uNMatrix * vec4(aNormal, 0.0)).xyz;
  Vmatrix = uVMatrix;
}`;
