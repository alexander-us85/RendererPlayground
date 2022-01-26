#version 450

layout(push_constant) uniform Push {
    mat4 transform;
    vec3 color;
    int demoKind;
} push;

layout (location = 0) in vec3 fragColor;
layout (location = 0) out vec4 outColor;

void main() {
    if (push.demoKind < 2) {
        outColor = vec4(push.color, 1.0);
    } else {
        outColor = vec4(fragColor, 1.0);
    }
}