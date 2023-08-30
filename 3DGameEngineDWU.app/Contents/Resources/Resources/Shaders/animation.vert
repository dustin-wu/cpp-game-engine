#version 330 core

const int MAX_JOINTS = 50; // max joints allowed in a skeleton
const int MAX_WEIGHTS = 3; // max number of joints that can affect a vertex

// All in object space
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;
layout (location = 3) in ivec3 jointIndices;
layout (location = 4) in vec3 weights;

uniform mat4 model, view, projection;
uniform mat4 jointTransforms[MAX_JOINTS];

out vec3 worldSpace_pos;
out vec3 worldSpace_norm;
out vec2 tex_coord;
out vec3 vertColor;

void main() {

    vec4 totalLocalPos = vec4(0.0);
    vec4 totalNormal = vec4(0.0);

    for(int i = 0; i < MAX_WEIGHTS; i++){
        mat4 jointTransform = jointTransforms[jointIndices[i]];
        vec4 posePosition = jointTransform * vec4(pos, 1.0);
        totalLocalPos += posePosition * weights[i];

        vec4 worldNormal = jointTransform * vec4(norm, 0.0);
        totalNormal += worldNormal * weights[i];
    }

    tex_coord = uv;
    vertColor = vec3(0); // unused
    worldSpace_norm = vec3(transpose(inverse(model))*vec4(vec3(totalNormal), 0.0));
    worldSpace_pos = vec3(model*vec4(vec3(totalLocalPos), 1.0));
    gl_Position = projection*view*model*vec4(vec3(totalLocalPos), 1.0);
//    gl_Position = projection*view*model*vec4(vec3(pos), 1.0);
}
