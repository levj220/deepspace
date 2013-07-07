/*
 * QuadTree.h
 *
 *  Created on: Jul 7, 2013
 *      Author: smolin
 */

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class QuadTree
{
public:
	QuadTree();
	virtual ~QuadTree();

	QuadTree *ul;
	QuadTree *ur;
	QuadTree *ll;
	QuadTree *lr;

	QuadTree *N;
	QuadTree *S;
	QuadTree *E;
	QuadTree *W;

	glm::vec3 ne;
	glm::vec3 nw;
	glm::vec3 se;
	glm::vec3 sw;

};

#endif /* QUADTREE_H_ */
