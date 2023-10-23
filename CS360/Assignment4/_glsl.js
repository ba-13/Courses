//@ts-check

let vertexShaderCode = `#version 300 es
in vec2 aPosition;
in vec2 aTexCoord;
uniform mat4 uMMatrix;
out vec2 fragTexCoord;

void main() {
  gl_Position = uMMatrix*vec4(aPosition, 0.0, 1.0);
  fragTexCoord = aTexCoord;
  gl_PointSize = 2.0;
}`;

let fragmentShaderCode = `#version 300 es
precision mediump float;
out vec4 fragColor;
in vec2 fragTexCoord;
uniform int imageMode;
uniform int imageColor;
uniform float imageContrast;
uniform float imageBrightness;
uniform int backgroundProcess;
uniform sampler2D backImageTexture;
uniform sampler2D foreImageTexture;
uniform vec4 color;

void main() {
  vec4 backgroundTex =  texture(backImageTexture, fragTexCoord);
  vec4 foregroundTex =  texture(foreImageTexture, fragTexCoord);

  if (backgroundProcess == 1) {
    //! smoothen
    mat3 kernel = mat3(vec3(1.0,1.0,1.0),
                       vec3(1.0,1.0,1.0),
                       vec3(1.0,1.0,1.0)) * 0.1111;
    float res_r = 0.0;
    float res_g = 0.0;
    float res_b = 0.0;
    float offset = 0.003;
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        vec4 neighbour = texture(backImageTexture, 
                                 fragTexCoord + vec2(float(i)*offset,
                                 float(j)*offset));

        res_r += neighbour.r * kernel[i+1][j+1];
        res_g += neighbour.g * kernel[i+1][j+1];
        res_b += neighbour.b * kernel[i+1][j+1];
      }
    }
    backgroundTex.r = res_r;
    backgroundTex.g = res_g;
    backgroundTex.b = res_b;
  }
  else if (backgroundProcess == 2){
    //! sharpen
    mat3 kernel = mat3(vec3(0.0,-1.0,0.0),
                       vec3(-1.0,5.0,-1.0),
                       vec3(0.0,-1.0,0.0));
    float res_r = 0.0;
    float res_g = 0.0;
    float res_b = 0.0;
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        vec4 neighbour = texture(backImageTexture, 
                                 fragTexCoord + vec2(float(i)*0.003,
                                 float(j)*0.003));

        res_r += neighbour.r * kernel[i+1][j+1];
        res_g += neighbour.g * kernel[i+1][j+1];
        res_b += neighbour.b * kernel[i+1][j+1];
      }
    }
    backgroundTex.r = res_r;
    backgroundTex.g = res_g;
    backgroundTex.b = res_b;
  }
  else if (backgroundProcess == 3) {
    //! gradient
    float offset = 0.004;
    vec2 up = fragTexCoord + vec2(-1.0,0.0) * offset;
    vec2 down = fragTexCoord + vec2(1.0,0.0) * offset;
    vec2 right = fragTexCoord + vec2(0.0,1.0) * offset;
    vec2 left = fragTexCoord + vec2(0.0,-1.0) * offset;
    vec4 dy = (texture(backImageTexture,up)-texture(backImageTexture,down))*0.5;
    vec4 dx = (texture(backImageTexture,right)-texture(backImageTexture,left))*0.5;
    vec4 gradientMag = sqrt(dx*dx + dy*dy);
    float temp = backgroundTex.a;
    backgroundTex = gradientMag;
    backgroundTex.a = temp;
  }
  else if (backgroundProcess == 4){
    //! laplacian
    mat3 kernel = mat3(vec3(0.0,-1.0,0.0),
                       vec3(-1.0,4.0,-1.0),
                       vec3(0.0,-1.0,0.0));
    float res_r = 0.0;
    float res_g = 0.0;
    float res_b = 0.0;
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        vec4 neighbour = texture(backImageTexture, 
                                 fragTexCoord + vec2(float(i)*0.003,
                                 float(j)*0.003));

        res_r += neighbour.r * kernel[i+1][j+1];
        res_g += neighbour.g * kernel[i+1][j+1];
        res_b += neighbour.b * kernel[i+1][j+1];
      }
    }
    backgroundTex.r = res_r;
    backgroundTex.g = res_g;
    backgroundTex.b = res_b;
  }
  
  if (imageColor == 1) {
    //! grayscale
    float wred = 0.2126;
    float wgreen = 0.7152;
    float wblue = 0.0722;

    backgroundTex.x = backgroundTex.x * wred + backgroundTex.y * wgreen + backgroundTex.z * wblue;
    backgroundTex.y = backgroundTex.x * wred + backgroundTex.y * wgreen + backgroundTex.z * wblue;
    backgroundTex.z = backgroundTex.x * wred + backgroundTex.y * wgreen + backgroundTex.z * wblue;

    foregroundTex.x = foregroundTex.x * wred + foregroundTex.y * wgreen + foregroundTex.z * wblue;
    foregroundTex.y = foregroundTex.x * wred + foregroundTex.y * wgreen + foregroundTex.z * wblue;
    foregroundTex.z = foregroundTex.x * wred + foregroundTex.y * wgreen + foregroundTex.z * wblue;
  }
  else if (imageColor == 2) {
    //! sepia
    float sepiaR = 0.393*backgroundTex.r + 0.769*backgroundTex.g + 0.189*backgroundTex.b;
    float sepiaG = 0.349*backgroundTex.r + 0.686*backgroundTex.g + 0.168*backgroundTex.b;
    float sepiaB = 0.272*backgroundTex.r + 0.534*backgroundTex.g + 0.131*backgroundTex.b;
    
    backgroundTex.x = sepiaR;
    backgroundTex.y = sepiaG;
    backgroundTex.z = sepiaB;

    sepiaR = 0.393*foregroundTex.r + 0.769*foregroundTex.g + 0.189*foregroundTex.b;
    sepiaG = 0.349*foregroundTex.r + 0.686*foregroundTex.g + 0.168*foregroundTex.b;
    sepiaB = 0.272*foregroundTex.r + 0.534*foregroundTex.g + 0.131*foregroundTex.b;

    foregroundTex.x = sepiaR;
    foregroundTex.y = sepiaG;
    foregroundTex.z = sepiaB;
  }

  backgroundTex.r = 0.5 + (imageContrast + 1.0) * (backgroundTex.r - 0.5);
  backgroundTex.g = 0.5 + (imageContrast + 1.0) * (backgroundTex.g - 0.5);
  backgroundTex.b = 0.5 + (imageContrast + 1.0) * (backgroundTex.b - 0.5);

  foregroundTex.r = 0.5 + (imageContrast + 1.0) * (foregroundTex.r - 0.5);
  foregroundTex.g = 0.5 + (imageContrast + 1.0) * (foregroundTex.g - 0.5);
  foregroundTex.b = 0.5 + (imageContrast + 1.0) * (foregroundTex.b - 0.5);

  backgroundTex.r += imageBrightness;
  backgroundTex.g += imageBrightness;
  backgroundTex.b += imageBrightness;

  foregroundTex.r += imageBrightness;
  foregroundTex.g += imageBrightness;
  foregroundTex.b += imageBrightness;

  if (imageMode == 1) {
    //! background only
    fragColor = backgroundTex;
  }
  else if (imageMode == 2) {
    //! alpha blending
    float alpha = foregroundTex.w;
    vec4 finalcolor;
    finalcolor.r = alpha*foregroundTex.r + (1.0 - alpha) * backgroundTex.r;
    finalcolor.g = alpha*foregroundTex.g + (1.0 - alpha) * backgroundTex.g;
    finalcolor.b = alpha*foregroundTex.b + (1.0 - alpha) * backgroundTex.b;
    finalcolor.w = 1.0;
    fragColor = finalcolor;
  }
}`;
