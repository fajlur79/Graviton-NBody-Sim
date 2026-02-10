#ifndef BARNES_HUT_H
#define BARNES_HUT_H

#include "types.h"
#include <stdbool.h>

typedef struct {
	float x, y;
	float size;
}BoundingBox;


typedef struct QuadNode {
    BoundingBox bounds;
    
    float total_mass;
    Vec2 center_of_mass;
    
    
    struct QuadNode* nw;
    struct QuadNode* ne; 
    struct QuadNode* sw; 
    struct QuadNode* se; 
  
    Particle* particle;
    bool is_leaf;
} QuadNode;

QuadNode* bh_create_node(float x, float y, float size);

void bh_insert(QuadNode* node, Particle* p);

void bh_compute_mass_distribution(QuadNode* node);

void bh_calculate_force(Particle* p, QuadNode* root, float G, float theta, float softening);

//void bh_free_tree(QuadNode* node);
void bh_reset_pool();

#endif
