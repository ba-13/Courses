let vertexShaderCode = `#version 300 es
in vec2 aPosition;

void main() {
  gl_Position = vec4(aPosition, 0.0, 1.0);
}`;

let fragmentShaderCode = `#version 300 es
precision mediump float;
out vec4 fragColor;
uniform vec4 uColor;
uniform int uState;
uniform int uBounce;
uniform vec3 uLightPos;
uniform vec3 uCameraPos;

float phong_alpha = 10.0;
float distCanvasCamera = -2.;
// vec3 cameraPos = vec3(0.0, 0.0, 5.0);
float canvWidth = 550.0;
float canvHeight = 550.0;
float BIAS = 0.05;

struct Sphere {
  vec3 center;
  float radius;
  vec3 color;
};

struct Ray {
  vec3 origin;
  vec3 direction;
};

struct Intersections {
  float t1;
  float t2;
};

const int num_spheres = 4;
Sphere sphere[num_spheres];

Intersections intersection(Sphere sp, Ray ry) {
  Intersections result;
  result.t1 = 0., result.t2 = 0.; //! 0. or negative isn't a valid value
  vec3 vecCtoO = (ry.origin - sp.center);
  float a = dot(ry.direction, ry.direction);
  float b = 2.0 * dot(ry.direction, vecCtoO);
  float c = dot(vecCtoO, vecCtoO) - pow(sp.radius, 2.0);
  float D = pow(b, 2.0) - 4.0 * a * c;
  if (D < 0.0) return result;
  result.t1 = (-b + sqrt(D))/(2. * a);
  result.t2 = (-b - sqrt(D))/(2. * a); //* smaller value than t1
  if (result.t2 < 0.0) {
    result.t1 = 0., result.t2 = 0.;
  }
  return result;
}

vec4 phong_shading(vec3 position, vec3 normal, vec3 base_color) {
  vec3 L = normalize(uLightPos - position);
  vec3 R = normalize(-reflect(L, normal));
  vec3 V = normalize(uCameraPos - position);
  float cos_theta = max(dot(normal, L), 0.0);
  float cos_phi = max(dot(V, R), 0.0);
  vec3 ldiff = 0.7 * cos_theta * base_color;
  vec3 lspec = 0.6 * (pow(cos_phi, phong_alpha)) * vec3(1.0, 1.0, 1.0);
  vec3 lamb = 0.2 * base_color;
  return vec4(ldiff + lamb + lspec, 1.0);
}

float is_shadow(vec3 position) {
  Ray secRay;
  secRay.direction = normalize(uLightPos - position);
  secRay.origin = position + BIAS * secRay.direction;
  for (int i=0; i<num_spheres; i++) {
    Intersections its = intersection(sphere[i], secRay);
    if ((its.t1 == 0.) && (its.t2 == 0.)) ;
    else return 0.6;
  }
  return 0.0;
}

void main() {
  float uStatef = float(uState);
  float uBouncef = float(uBounce);
  fragColor = uColor;
  Ray ray;
  ray.origin = uCameraPos;
  vec2 screenPos = gl_FragCoord.xy/vec2(canvWidth, canvHeight);
  ray.direction = normalize(vec3(screenPos * 2.0 - 1.0, distCanvasCamera));

  sphere[0].center = vec3(0.0, 1.0, -1.5);
  sphere[0].radius = 1.5;
  sphere[0].color = vec3(1.0, 0.0, 0.0);
  sphere[1].center = vec3(-1.5, 0.5, 0.6);
  sphere[1].radius = 0.9;
  sphere[1].color = vec3(0.0, 1.0, 0.0);
  sphere[2].center = vec3(1.5, 0.5, 0.6);
  sphere[2].radius = 0.9;
  sphere[2].color = vec3(0.0, 0.0, 1.0);
  sphere[3].center = vec3(0.0, -10.0, -2.0);
  sphere[3].radius = 9.0;
  sphere[3].color = vec3(0.6, 0.6, 0.6);

  float least_depth = 1000000000.0; // max depth assumed for t
  vec4 least_depth_color = uColor;
  for (int i=0; i<num_spheres; i++) {
    Intersections its = intersection(sphere[i], ray);
    if ((its.t1 == 0.) && (its.t2 == 0.)) continue;

    vec3 position = ray.origin + its.t2 * ray.direction;
    vec3 normal = normalize(position - (sphere[i]).center);
    vec3 color = sphere[i].color;
    fragColor = phong_shading(position, normal, color);
    //* check for reflection
    if (uState > 1) {
      Ray incoming_ray = ray;
      vec3 position_here = position;
      vec3 normal_here = normal;
      vec3 color_here = color;
      Ray outgoing_ray;
      for (int bounceCount=1; bounceCount <= uBounce; bounceCount++) {
        //? for this bounce, calculate the reflection vector
        outgoing_ray.direction = normalize(reflect(incoming_ray.direction, normal_here));
        outgoing_ray.origin = position_here + BIAS * outgoing_ray.direction;
        incoming_ray = outgoing_ray;
        //? now iterate over all spheres to get next intersection
        float least_depth_here = 1000000000.0;
        int sphere_idx = -1;
        Intersections its_here;
        for (int j=0; j<num_spheres; j++) {
          Intersections its_tmp = intersection(sphere[j], incoming_ray);
          if ((its_tmp.t1 == 0.) && (its_tmp.t2 == 0.)) continue;
          //* check for least depth
          if (least_depth_here > its_tmp.t2) {
            least_depth_here = its_tmp.t2;
            sphere_idx = j;
            its_here = its_tmp;
          }
        }
        //* use the least depth intersection to get the color
        if (sphere_idx == -1) {
          // no reflection possible
          break;
        }
        position_here = incoming_ray.origin + its_here.t2 * incoming_ray.direction;
        normal_here = normalize(position_here - (sphere[sphere_idx]).center);
        color_here = sphere[sphere_idx].color;
        fragColor = 0.6 * phong_shading(position_here, normal_here, color_here) + 0.4 * fragColor;
      }
    }
    //* check for shadow
    if (uState % 2 == 1) {
      float shadow_component = is_shadow(position);
      fragColor = vec4((1.0 - shadow_component) * fragColor.xyz, 1.0);
    }

    if (least_depth > its.t2) {
      least_depth = its.t2;
      least_depth_color = fragColor;
    }
  }
  fragColor = least_depth_color;
  // fragColor = vec4((uLightPos[0] + 10.0) * 0.05, uBouncef * 0.1, uStatef * 0.25, 1.0);
}`;
