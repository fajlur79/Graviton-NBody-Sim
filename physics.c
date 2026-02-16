#include "types.h"
#include "raylib.h"
#include "rlgl.h"
#include <stdlib.h>
#include <stdio.h>

#if defined(__linux__)
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

static unsigned int shaderProgram = 0;
static unsigned int ssbo = 0;
static bool gpu_initialized = false;

static void init_gpu(Particle* p, SimConfig* conf) {
    char* code = LoadFileText("gravity.glsl");
    if (!code) {
        printf("CRITICAL ERROR: Could not find 'gravity.glsl'!\n");
        exit(1);
    }

    unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, (const char**)&code, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        printf("SHADER COMPILATION ERROR:\n%s\n", log);
        exit(1);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shader);
    glLinkProgram(shaderProgram);
    UnloadFileText(code);

    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, conf->particle_count * sizeof(Particle), p, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    gpu_initialized = true;
    printf("GPU Physics Initialized for %d particles.\n", conf->particle_count);
}

void compute_forces(Particle* p, SimConfig* conf){
    if (!gpu_initialized) init_gpu(p, conf);

    glUseProgram(shaderProgram);

    int locG = glGetUniformLocation(shaderProgram, "G");
    int locDt = glGetUniformLocation(shaderProgram, "dt");
    int locCount = glGetUniformLocation(shaderProgram, "count");
    int locSoft = glGetUniformLocation(shaderProgram, "softening");

    glUniform1f(locG, conf->G);
    glUniform1f(locDt, conf->dt);
    glUniform1i(locCount, conf->particle_count);
    glUniform1f(locSoft, conf->softening);

    int num_groups = (conf->particle_count + 255) / 256;
    glDispatchCompute(num_groups, 1, 1);
    
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, conf->particle_count * sizeof(Particle), p);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glUseProgram(0); 
}
