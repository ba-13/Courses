//@ts-check

let fragmentShaderCode = `#version 300 es
precision mediump float;
out vec4 fragColor;
in mat4 Vmatrix;
in vec3 posInEyeSpace;
uniform vec4 uColor;
uniform vec3 lightLocation;

void main() {
  vec3 normal=normalize(cross(dFdx(posInEyeSpace),dFdy(posInEyeSpace)));
  vec3 L=normalize(vec3(Vmatrix*vec4(lightLocation,1.0))-posInEyeSpace);
  vec3 R=normalize(-reflect(L,normal));
  vec3 V=normalize(-posInEyeSpace);
  float cos_theta=max(dot(normal,L),0.0);
  vec3 ldiff=cos_theta*vec3(uColor);
  float cos_phi=max(dot(V,R),0.0);
  vec3 lspec=(pow(cos_phi,26.0))*vec3(1.0,1.0,1.0);
  vec3 lamb=0.6*(vec3(uColor));
  fragColor=vec4(ldiff+lamb+lspec,1.0);
}`;

let vertexShaderCode = `#version 300 es
in vec3 aPosition;
uniform mat4 uMMatrix;
uniform mat4 uPMatrix;
uniform mat4 uVMatrix;
uniform mat4 uNMatrix;
out vec3 posInEyeSpace;
out mat4 Vmatrix;

void main() {
  Vmatrix=uVMatrix;
  mat4 projectionModelView;
  mat4 m1;
	m1=uVMatrix*uMMatrix;
	projectionModelView=uPMatrix*m1;
  gl_Position = projectionModelView*vec4(aPosition,1.0);
  gl_PointSize=5.0;
  
  posInEyeSpace = vec3(m1*vec4(aPosition,1.0));
}`;
