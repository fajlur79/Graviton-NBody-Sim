#version 430

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct Particle{
    float px, py, pz; float pad1;
    float vx, vy, vz; float pad2;
    float ax, ay, az; float pad3;
    float ppx, ppy, ppz; float pad4;
    float mass;
    float radius;
    uint color;
    uint is_fixed;
};


layout(std430, binding = 0) buffer ParticleBuffer {
    Particle p[];
};

uniform float G;
uniform float dt;
uniform int count;
uniform float softening;

shared vec4 cachePos[256];

void main() {
   uint i = gl_GlobalInvocationID.x;
   uint local_i = gl_LocalInvocationID.x;

   if (i >= count) return;

   vec3 my_pos = vec3(p[i].px, p[i].py, p[i].pz);
   vec3 acc = vec3(0.0);


   uint num_tiles = (count + 255) / 256;

   for (int tile = 0; tile < num_tiles; tile++){
	uint srcIdx = (tile * 256) + local_i;

	if (srcIdx < count ) {
	    cachePos[local_i] = vec4(p[srcIdx].px, p[srcIdx].py, p[srcIdx].pz, p[srcIdx].mass);
	}else{
		cachePos[local_i] = vec4(0.0);
	}
	barrier();

	for (int j = 0; j < 256; j++){
	    vec4 other = cachePos[j];
	    float other_mass = other.w;
	    
	    if(other_mass == 0.0) continue;
	    vec3 r_vec = other.xyz - my_pos;
	    float distSq = dot(r_vec, r_vec);
	    float softSq = softening * softening;

	    float dist = sqrt(distSq + softSq);
	    float f = (G * other_mass) / (dist * dist * dist);

	    acc += r_vec * f;
	}
	barrier();
    }
    if (p[i].is_fixed == 0) {
        
        p[i].ppx = my_pos.x;
        p[i].ppy = my_pos.y;
        p[i].ppz = my_pos.z;

        vec3 vel = vec3(p[i].vx, p[i].vy, p[i].vz);
        vel += acc * dt;
        
        vec3 pos = my_pos + vel * dt;
        
        p[i].vx = vel.x; p[i].vy = vel.y; p[i].vz = vel.z;
        p[i].px = pos.x; p[i].py = pos.y; p[i].pz = pos.z;
    }
}
