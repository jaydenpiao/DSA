/**
 * @file qtree.cpp
 * @description student implementation of QTree class used for storing image data
 *              CPSC 221 PA3
 *
 *              SUBMIT THIS FILE
 */

#include "qtree.h"

/**
 * Constructor that builds a QTree out of the given PNG.
 * Every leaf in the tree corresponds to a pixel in the PNG.
 * Every non-leaf node corresponds to a rectangle of pixels
 * in the original PNG, represented by an (x,y) pair for the
 * upper left corner of the rectangle and an (x,y) pair for
 * lower right corner of the rectangle. In addition, the Node
 * stores a pixel representing the average color over the
 * rectangle.
 *
 * The average color for each node in your implementation MUST
 * be determined in constant time. HINT: this will lead to nodes
 * at shallower levels of the tree to accumulate some error in their
 * average color value, but we will accept this consequence in
 * exchange for faster tree construction.
 * Note that we will be looking for specific color values in our
 * autograder, so if you instead perform a slow but accurate
 * average color computation, you will likely fail the test cases!
 *
 * Every node's children correspond to a partition of the
 * node's rectangle into (up to) four smaller rectangles. The node's
 * rectangle is split evenly (or as close to evenly as possible)
 * along both horizontal and vertical axes. If an even split along
 * the vertical axis is not possible, the extra line will be included
 * in the left side; If an even split along the horizontal axis is not
 * possible, the extra line will be included in the upper side.
 * If a single-pixel-wide rectangle needs to be split, the NE and SE children
 * will be null; likewise if a single-pixel-tall rectangle needs to be split,
 * the SW and SE children will be null.
 *
 * In this way, each of the children's rectangles together will have coordinates
 * that when combined, completely cover the original rectangle's image
 * region and do not overlap.
 */
QTree::QTree(const PNG& imIn) {
	this->height = imIn.height();
	this->width = imIn.width();
	root = BuildNode(imIn, {0,0}, {this->width -1, this->height -1});
	
}

/**
 * Overloaded assignment operator for QTrees.
 * Part of the Big Three that we must define because the class
 * allocates dynamic memory. This depends on your implementation
 * of the copy and clear funtions.
 *
 * @param rhs The right hand side of the assignment statement.
 */
QTree& QTree::operator=(const QTree& rhs) {
    if (this == &rhs) {
        return *this;
    }
    copyTree(root, rhs.root);
    return *this;
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * For up-scaled images, no color interpolation will be done;
 * each rectangle is fully rendered into a larger rectangular region.
 *
 * @param scale multiplier for each horizontal/vertical dimension
 * @pre scale > 0
 */
PNG QTree::Render(unsigned int scale) const {
	PNG curr(this->width * scale, this->height * scale);
	renderTree(curr, root, scale);
	return curr;
}

/**
 *  Prune function trims subtrees as high as possible in the tree.
 *  A subtree is pruned (cleared) if all of the subtree's leaves are within
 *  tolerance of the average color stored in the root of the subtree.
 *  NOTE - you may use the distanceTo function found in RGBAPixel.h
 *  Pruning criteria should be evaluated on the original tree, not
 *  on any pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this one.
 *
 * @param tolerance maximum RGBA distance to qualify for pruning
 * @pre this tree has not previously been pruned, nor is copied from a previously pruned tree.
 */
void QTree::Prune(double tolerance) {
	// ADD YOUR IMPLEMENTATION BELOW
	pruneNode(this->root, tolerance);
}

/**
 *  FlipHorizontal rearranges the contents of the tree, so that
 *  its rendered image will appear mirrored across a vertical axis.
 *  This may be called on a previously pruned/flipped/rotated tree.
 *
 *  After flipping, the NW/NE/SW/SE pointers must map to what will be
 *  physically rendered in the respective NW/NE/SW/SE corners, but it
 *  is no longer necessary to ensure that 1-pixel wide rectangles have
 *  null eastern children
 *  (i.e. after flipping, a node's NW and SW pointers may be null, but
 *  have non-null NE and SE)
 * 
 *  You may want a recursive helper function for this one.
 */
void QTree::FlipHorizontal() {
	flipTree(this->root);
	this->root->upLeft= {0,0};
	this->root->lowRight = {this->width -1, this->height -1};
}

/**
 *  RotateCCW rearranges the contents of the tree, so that its
 *  rendered image will appear rotated by 90 degrees counter-clockwise.
 *  This may be called on a previously pruned/flipped/rotated tree.
 *
 *  Note that this may alter the dimensions of the rendered image, relative
 *  to its original dimensions.
 *
 *  After rotation, the NW/NE/SW/SE pointers must map to what will be
 *  physically rendered in the respective NW/NE/SW/SE corners, but it
 *  is no longer necessary to ensure that 1-pixel tall or wide rectangles
 *  have null eastern or southern children
 *  (i.e. after rotation, a node's NW and NE pointers may be null, but have
 *  non-null SW and SE, or it may have null NW/SW but non-null NE/SE)
 *
 *  You may want a recursive helper function for this one.
 */
void QTree::RotateCCW() {
	rotateNode(this->root);
    std::swap(this->width, this->height);
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current QTree object. Complete for PA3.
 * You may want a recursive helper function for this one.
 */
void QTree:: Clear() {
	destroyTree(root);
}

/**
 * Copies the parameter other QTree into the current QTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other The QTree to be copied.
 */
void QTree::Copy(const QTree& other) {
	copyTree(this->root, other.root);
	this->height = other.height;
	this->width = other.width;
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param img reference to the original input image.
 * @param ul upper left point of current node's rectangle.
 * @param lr lower right point of current node's rectangle.
 */
Node* QTree::BuildNode(const PNG& img, pair<unsigned int, unsigned int> ul, pair<unsigned int, unsigned int> lr) {
	unsigned int x1 = ul.first;
    unsigned int y1 = ul.second;
    unsigned int x2 = lr.first;
    unsigned int y2 = lr.second;

    if (x1 == x2 && y1 == y2) {
        Node* curr = new Node(ul, lr, *img.getPixel(x1, y1));
        return curr;
    }

    Node* curr = new Node(ul, lr, RGBAPixel(0, 0, 0));
    unsigned int midX = (x1 + x2) / 2;
    unsigned int midY = (y1 + y2) / 2;

    curr->NW = BuildNode(img, ul, {midX, midY});

    if (x1 != x2) {
        curr->NE = BuildNode(img, {midX + 1, y1}, {x2, midY});
    }
    if (y1 != y2) {
        curr->SW = BuildNode(img, {x1, midY + 1}, {midX, y2});
    }
    if (x1 != x2 && y1 != y2) {
        curr->SE = BuildNode(img, {midX + 1, midY + 1}, lr);
    }
    curr->avg = getAvColor(curr);
    return curr;
}

/*********************************************************/
/*** IMPLEMENT YOUR OWN PRIVATE MEMBER FUNCTIONS BELOW ***/
/*********************************************************/

RGBAPixel QTree::getAvColor(Node* curr) {
	int r = 0;
	int g = 0;
	int b = 0;
	int totalArea = (curr->lowRight.first - curr->upLeft.first +1) * (curr->lowRight.second - curr->upLeft.second +1);

	if (curr->NW) {
		int nwArea = (curr->NW->lowRight.first - curr->NW->upLeft.first +1) * (curr->NW->lowRight.second - curr->NW->upLeft.second +1);
		r += curr->NW->avg.r * nwArea;
		g += curr->NW->avg.g * nwArea;
		b += curr->NW->avg.b * nwArea;
	}
	if (curr->NE) {
		int neArea = (curr->NE->lowRight.first - curr->NE->upLeft.first+1) * (curr->NE->lowRight.second - curr->NE->upLeft.second+1);
		r += curr->NE->avg.r * neArea;
		g += curr->NE->avg.g * neArea;
		b += curr->NE->avg.b * neArea;
	}
	if (curr->SW) {
		int swArea = (curr->SW->lowRight.first - curr->SW->upLeft.first+1)* (curr->SW->lowRight.second - curr->SW->upLeft.second+1);
		r += curr->SW->avg.r * swArea;
		g += curr->SW->avg.g * swArea;
		b += curr->SW->avg.b * swArea;
	}
	if (curr->SE) {
		int seArea = (curr->SE->lowRight.first - curr->SE->upLeft.first+1) * (curr->SE->lowRight.second - curr->SE->upLeft.second+1);
		r += curr->SE->avg.r * seArea;
		g += curr->SE->avg.g * seArea;
		b += curr->SE->avg.b * seArea;
	}

	RGBAPixel p(r/totalArea, g/totalArea, b/totalArea);
	return p;
}

void QTree::copyTree(Node*& destination, const Node* source) {
    if (source == nullptr) {
        destination = nullptr;
        return;
    }

    destination = new Node(source->upLeft, source->lowRight, source->avg);

    copyTree(destination->NW, source->NW);
    copyTree(destination->NE, source->NE);
    copyTree(destination->SW, source->SW);
    copyTree(destination->SE, source->SE);
}

void QTree::destroyTree(Node* &curr) {
    if (curr == nullptr) {
        return;
    }

    destroyTree(curr->NW);
    destroyTree(curr->NE);
    destroyTree(curr->SW);
    destroyTree(curr->SE);

    delete curr;
    curr = nullptr; 
}

void QTree::renderTree(PNG& image, const Node* current, unsigned int scale) const{
    if (current == nullptr) {
        return;
    }

    if (current->NW == nullptr && current->NE == nullptr && current->SW == nullptr && current->SE == nullptr) {
        for (unsigned int x = 0; x < scale; ++x) {
            for (unsigned int y = 0; y < scale; ++y) {
                *image.getPixel(scale * (current->upLeft.first) + x, scale * (current->upLeft.second) + y) = current->avg;
            }
        }
    } else {
        renderTree(image, current->NW, scale);
        renderTree(image, current->NE, scale);
        renderTree(image, current->SW, scale);
        renderTree(image, current->SE, scale);
    }
}

void QTree::flipTree(Node*& current) {

	if (current == nullptr) {
        return;
    }

    // Swap the child nodes to mirror the image horizontally
    std::swap(current->NW, current->NE);
    std::swap(current->SW, current->SE);

    // Correctly update the x-coordinates of the node's region
    unsigned int originalUpLeftX = current->upLeft.first;
    unsigned int originalLowRightX = current->lowRight.first;

    current->upLeft.first = this->width - originalLowRightX - 1;
    current->lowRight.first = this->width - originalUpLeftX - 1;

    // Apply the process recursively to all child nodes
    flipTree(current->NW);
    flipTree(current->NE);
    flipTree(current->SW);
    flipTree(current->SE);

}

void QTree::rotateNode(Node*& current) {
    if (current == nullptr) {
        return;
    }

    // Rotate the child nodes
    Node* temp = current->NW;
    current->NW = current->NE;
    current->NE = current->SE;
    current->SE = current->SW;
    current->SW = temp;

    // Update the coordinates of the current node
    auto oldUpLeft = current->upLeft;
    auto oldLowRight = current->lowRight;

    current->upLeft.first = oldUpLeft.second;
    current->upLeft.second = this->width - oldLowRight.first - 1;

    current->lowRight.first = oldLowRight.second;
    current->lowRight.second = this->width - oldUpLeft.first - 1;

    // Recursively rotate child nodes
    rotateNode(current->NW);
    rotateNode(current->NE);
    rotateNode(current->SW);
    rotateNode(current->SE);
}

bool QTree::shouldPrune(Node* root, double tolerance) const {
    if (root == nullptr) {
        return true;
    }
    return checkTolerance(root, root->avg, tolerance);
}

bool QTree::checkTolerance(Node* root, RGBAPixel avgColor, double tolerance) const {
    if (root == nullptr) {
        return true;
    }
    if (root->NW == nullptr && root->NE == nullptr && root->SW == nullptr && root->SE == nullptr) {
        // Leaf node
        return root->avg.distanceTo(avgColor) <= tolerance;
    }

    // Check all children
    return checkTolerance(root->NW, avgColor, tolerance) &&
           checkTolerance(root->NE, avgColor, tolerance) &&
           checkTolerance(root->SW, avgColor, tolerance) &&
           checkTolerance(root->SE, avgColor, tolerance);
}

void QTree::pruneNode(Node*& root, double tolerance) {
	if (root == nullptr || (root->NW == nullptr && root->NE == nullptr && root->SW == nullptr && root->SE == nullptr)) {
        // If the node is null or a leaf, there's nothing to prune
        return;
    }

    if (shouldPrune(root, tolerance)) {
        // Prune the subtree
        clearSubtree(root->NW);
        clearSubtree(root->NE);
        clearSubtree(root->SW);
        clearSubtree(root->SE);
    } else {
        // Recursively prune child nodes
        pruneNode(root->NW, tolerance);
        pruneNode(root->NE, tolerance);
        pruneNode(root->SW, tolerance);
        pruneNode(root->SE, tolerance);
    }
}

void QTree::clearSubtree(Node*& root) {
	if (root == nullptr) {
		return;
	}
	clearSubtree(root->NW);
	clearSubtree(root->NE);
	clearSubtree(root->SW);
	clearSubtree(root->SE);
	delete root;
	root = nullptr;
}