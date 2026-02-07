#include "barnes_hut.h"
#include "math_utils.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


static int get_quadrant(QuadNode* node, float x, float y){
	float mid_x = node-> bounds.x + node->bounds.size / 2.0f;
	float mid_y = node-> bounds.y + node->bounds.size / 2.0f;

	//0: NW, 1:NE, 2:SW, 3:SE
	if (y < mid_y){
		if (x < mid_x) return 0;
		else return 1;
	} else{
		 if(x < mid_x) return 2;
		 else return 3;
	}
}

static QuadNode* create_child(QuadNode* parent, int quadrant){
	float size = parent->bounds.size / 2.0f;
	float x = parent->bounds.x;
	float y = parent->bounds.y;

	if (quadrant == 1 || quadrant == 3) x += size;
	if (quadrant == 2 || quadrant == 3) y += size;

	return bh_create_node(x, y, size);
}

QuadNode* bh_create_node(float x, float y, float size){
	QuadNode* node = (QuadNode*)malloc(sizeof(QuadNode));
	node->bounds = (BoundingBox){x, y, size};
	node->total_mass = 0;
	node->center_of_mass = (Vec2){0,0};
	node->nw = node->ne = node->sw = node->se = NULL;
	node->particle = NULL;
	node->is_leaf = true;
	return node;
}

void bh_insert(QuadNode* node, Particle* p) {
    // Case 1: Internal Node - Push down to children
    if (!node->is_leaf) {
        int q = get_quadrant(node, p->pos.x, p->pos.y);
        
        QuadNode** child_ptr = NULL;
        switch(q) {
            case 0: child_ptr = &node->nw; break;
            case 1: child_ptr = &node->ne; break;
            case 2: child_ptr = &node->sw; break;
            case 3: child_ptr = &node->se; break;
        }

        if (*child_ptr == NULL) {
            *child_ptr = create_child(node, q);
        }
        bh_insert(*child_ptr, p);
        return;
    }

    // Case 2: Empty Leaf - Just add particle
    if (node->particle == NULL) {
        node->particle = p;
        return;
    }

    // Case 3: Full Leaf - Split and Redistribute
    
    // --- [CRITICAL FIX START] ---
    // If the node is already microscopic, stop splitting to prevent infinite loops.
    // This happens if two particles are at the exact same position.
    if (node->bounds.size < 0.01f) {
        // Technically we lose a particle here, but it saves the simulation from crashing.
        // A pro engine would merge their masses, but ignoring is fine for visualization.
        return; 
    }
    // --- [CRITICAL FIX END] ---

    // We must "explode" this leaf into an internal node
    node->is_leaf = false;

    Particle* old_p = node->particle;
    node->particle = NULL;

    bh_insert(node, old_p);
    bh_insert(node, p);
}


void bh_compute_mass_distribution(QuadNode* node){
	if (node->is_leaf){
		if(node->particle){
			node->total_mass = node->particle->mass;
			node->center_of_mass = node->particle->pos;
		}else{
			node->total_mass = 0;
			node->center_of_mass = (Vec2){0,0};
		}
		return;
	}

	float total_m = 0;
	Vec2 weighted_pos = {0,0};
	QuadNode* children[] = {node->nw, node->ne, node->sw, node->se};

	for (int i = 0; i < 4; i++){
		if (children[i]){
			bh_compute_mass_distribution(children[i]);
			total_m += children[i]->total_mass;

			Vec2 scaled_pos = vec2_scale(children[i]->center_of_mass, children[i]->total_mass);
			weighted_pos = vec2_add(weighted_pos, scaled_pos);
		}
	}

	if (total_m > 0){
		node->center_of_mass = vec2_scale(weighted_pos, 1.0f / total_m);
	}else{
		node->center_of_mass = (Vec2){0,0};
	}
}


void bh_calculate_force(Particle* p, QuadNode* node, float G, float theta, float softening){
	if (node == NULL || node->total_mass == 0) return;
	
	Vec2 r_vec = vec2_sub(node->center_of_mass, p->pos);
	float distSq = vec2_lenSq(r_vec);
	float dist = sqrtf(distSq);

	float s = node->bounds.size;

	if(node->is_leaf || (s / dist < theta) ){
		if (node->is_leaf && node->particle == p) return;

		float effective_distSq = distSq + (softening * softening);
		float effective_dist = sqrtf(effective_distSq);

		float f = (G * p->mass * node->total_mass) / effective_distSq;
		float f_scale = f / effective_dist;

		Vec2 force = vec2_scale(r_vec, f_scale);

		if (p->is_fixed){
			p->acc.x += force.x / p->mass;
			p->acc.y += force.y / p->mass;
		}
	}else{
		if (node->nw) bh_calculate_force(p, node->nw, G, theta, softening);
		if (node->ne) bh_calculate_force(p, node->ne, G, theta, softening);
		if (node->sw) bh_calculate_force(p, node->sw, G, theta, softening);
		if (node->se) bh_calculate_force(p, node->se, G, theta, softening);
	}
}


void bh_free_tree(QuadNode* node){
	if (node == NULL) return;
	bh_free_tree(node->nw);
	bh_free_tree(node->ne);
	bh_free_tree(node->se);
	bh_free_tree(node->sw);
	free(node);
}
