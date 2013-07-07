/*
 * QuadTree.h
 *
 *  Created on: Jul 7, 2013
 *      Author: smolin
 */

#ifndef QUADTREE_H_
#define QUADTREE_H_

class QuadTree
{
public:
	QuadTree();
	virtual ~QuadTree();

	QuadTree *ul;
	QuadTree *ur;
	QuadTree *ll;
	QuadTree *lr;


};

#endif /* QUADTREE_H_ */
