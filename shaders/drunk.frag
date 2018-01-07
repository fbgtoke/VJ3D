#version 330 core

uniform sampler2D tex;

in vec2 texcoordFrag;
out vec4 color;

uniform vec2 resolution;           // viewport resolution (in pixels)
uniform int time;                 // shader playback time (in seconds)

#define scale_uv(uv, scale, center) ((uv - center) * scale + center)

void main() {
    vec2 uv = texcoordFrag;
    
    float t = time * 0.001;
    vec2 center = vec2(
        sin(t * 1.25 + 75.0 + uv.y * 0.5) + sin(t * 2.75 - 18.0 - uv.x * 0.25),
        sin(t * 1.75 - 125.0 + uv.x * 0.25) + sin(t * 2.25 + 4.0 - uv.y * 0.5)
    ) * 0.25 + 0.5;
    
    float z = sin((t + 234.5) * 3.0) * 0.05 + 0.75;
    
    vec2 uv2 = scale_uv(uv, z, center);
    
    color = texture(tex, uv2);
    
    float vignette = 1.0 - distance(uv, vec2(0.5));
    color = mix(color, color * vignette, sin((t + 80.023) * 2.0) * 0.75);
}
