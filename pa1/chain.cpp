/**
 * File: chain.cpp
 * Description: student implementation for Chain functions
 *              for CPSC 221 2023W1 PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
**/

#include <math.h>
#include "block.h"
#include "chain.h"
using namespace std;
/**
 * Constructs a Chain from an input image
 * Each Node will cover a nodedimension x nodedimension (in pixels)
 * region in the source image.
 * @param img the input image
 * @param nodedimension number of pixels along the width and height of the Block region
 * @pre input image's width and height are evenly divisible by nodedimension
**/
Chain::Chain(PNG& img, unsigned int nodedimension) {
	int nodeWidth = img.width()/nodedimension;
	int nodeHeight = img.height()/nodedimension;
	Node* curr = NULL;
	this->length_ = nodeHeight * nodeWidth;

	for(int row= 0; row < nodeHeight; row++) {
		for (int col= 0; col < nodeWidth; col++) {
			Block* block = new Block;
			block->Build(img, col * nodedimension, row * nodedimension, nodedimension);

			if (curr == NULL) {
				curr = new Node(*block);
				this->NW = curr;
			
			} else {
				Node* temp = curr;
				curr = new Node(*block);
				temp->next= curr;
				curr->prev= temp;
			}
			delete block;
			this->SE = curr;
		}
	}
}

/**
 * Renders the Chain's pixel data into an output PNG.
 * The PNG should be sized according to the number of nodes
 * specified by the cols parameter.
 * It is possible that there may not be enough nodes to fill
 * the bottom row of the PNG. If this happens, just render
 * as many nodes as there are, from left to right, and leave
 * the rest of the row as opaque white pixels.
 * @pre this Chain is not empty
 * @param cols the number of Blocks to use for the width of the image
 * @param full whether to render all pixel data into a full-size image,
 *             or block averages into a tiny image
**/
PNG Chain::Render(unsigned int cols, bool full) {
	// replace the line below with your implementation

	if(length_ == 0 || cols == 0) {
		PNG png;
		return png;
	}

	int rows = (cols + length_ - 1) / cols;
	Node* node = NW;
	int dimension = NW->data.Dimension();

	if(full) {
		PNG png(cols * dimension, rows * dimension);
		for (int row = 0; row < rows; row++) {
			for (unsigned int col = 0; col < cols; col++) {
				if (node == NULL) {
					WhiteBlock(png, col * dimension, row * dimension, dimension);
				} else {
					node->data.Render(png, col * dimension, row * dimension, true);
					node = node->next;
				}
			}
		}
		return png;
	} else {
		PNG png(cols, rows);
		for (int row = 0; row < rows; row++) {
			for (unsigned int col = 0; col < cols; col++) {
				if (node == NULL) {
				*png.getPixel(col, row) = RGBAPixel(255, 255, 255, 1);
				} else {
				*png.getPixel(col, row) = node->data.GetAverageColor();
				node = node->next;
				}
			}
		}
		return png;
	}
}

/**
 * Inserts a new Node containing ndata at the back of the Chain
**/
void Chain::InsertBack(const Block& ndata) {
	Node* newNode = new Node(ndata);
	Node* temp = this->SE;
	temp->next = newNode;
	newNode->prev = temp;
	this->SE = newNode;
}

/**
 * Reverses the structure of the list. This must be done by
 * pointer assignments. You may not deallocate or allocate any Nodes.
 * Example:
 *	before:	NW -> A <-> B <-> C <-> D <-> E <-> F <-> G <-> H <- SE
 *  
 *  after:	NW -> H <-> G <-> F <-> E <-> D <-> C <-> B <-> A <- SE
**/
void Chain::Reverse() {
	if (length_ <= 1) {
        return;
    }

    Node* curr = NW;
    Node* temp = nullptr;
    SE = NW;

    while (curr != nullptr) {
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;
        curr = curr->prev;
    }

    NW = temp->prev;
}

/**
 * Rearranges the Node structure and internal pixel data to be flipped over a vertical axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *  before, flipping with 3 columns (2 rows):
 *
 *		NW -> A> <-> B> <-> C> <->
 *            D> <-> E> <-> F> <- SE
 * 
 *  after, visualized with 3 columns (2 rows):
 * 
 *		NW -> <C <-> <B <-> <A <->
 *            <F <-> <E <-> <D <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
**/
void Chain::FlipHorizontal(unsigned int cols) {
    if(length_ % cols != 0) {
        return;
    }
    
    unsigned int rows = length_ / cols;
    Node* start = NW;
	Node* nextHead;
	Node* prevHead = nullptr;
	Node* tail;
	

    for(unsigned int i = 0; i < rows; ++i) {
		nextHead= reverseRow(start, cols);
		if (i==0) {
			Node* temp = start;
			while (temp->prev != nullptr){
				temp =temp-> prev;
			}
			NW = temp;
		} 
		if (i== rows -1) {
			SE = start;
		}
		
		if (prevHead != NULL) {
			tail =start;
			for(unsigned int i=1; i < cols; i++) {
				tail= tail->prev;
			}
			prevHead->next = tail;
		}
		
		prevHead = start;
		start = nextHead;
    }
}

/**
 * Rearranges the Node structure and internal pixel data to be flipped over a horizontal axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *	before, flipping with 3 columns (2 rows):
 *                         
 *      NW -> A <-> B <-> C <->
 *            D <-> E <-> F <- SE
 * 
 *  after, visualized with 3 columns (2 rows):
 * 
 *      NW -> D <-> E <-> F <->
 *            A <-> B <-> C <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
**/


void Chain::FlipVertical(unsigned int cols) {
	
if (length_ <= 1 || cols == 0 || length_ % cols != 0) {
        return;
    }
	Node* temp = NW;
	while(temp!= NULL) {
		temp->data.FlipHorizontal();
		temp->data.FlipVertical();
		temp= temp->next;
	}
    Reverse();
	 
    unsigned int rows = length_ / cols;
    Node* start = NW;
	Node* nextHead;
	Node* prevHead = nullptr;
	Node* tail;
	

    for(unsigned int i = 0; i < rows; ++i) {
		nextHead= reverseRow(start, cols);
		if (i==0) {
			Node* temp = start;
			while (temp->prev != nullptr){
				temp =temp-> prev;
			}
			NW = temp;
		} 
		if (i== rows -1) {
			SE = start;
		}
		
		if (prevHead != NULL) {
			tail =start;
			for(unsigned int i=1; i < cols; i++) {
				tail= tail->prev;
			}
			prevHead->next = tail;
		}
		
		prevHead = start;
		start = nextHead;
    }


}

/**
 * Produces a blocky, pixellated effect (in a rendered image) by replacing each
 * block's pixel data with the average color of the block.
**/
void Chain::Blockify() {
	Node* curr = NW;
	while(curr != NULL) {
		curr->data.FillAverage();
		curr = curr->next;
	}
}

/**
 * Destroys all dynamically allocated memory associated with
 * this Chain object. Called by destructor and operator=.
 * You must complete its implementation for PA1.
**/
void Chain::Clear() {
	Node* curr = NW; 
    
    while (curr != nullptr) {
        Node* toDelete = curr;
        curr = curr->next; 
        delete toDelete;
    }
    
    NW = nullptr;
    SE = nullptr;
    length_ = 0;
    
}

/**
 * Copies the parameter other Chain into the current Chain.
 * Does not free any memory. Called by copy constructor and
 * operator=.
 * You must complete its implementation for PA1.
 * @param other The Chain to be copied.
**/
void Chain::Copy(const Chain& other) {
    if (other.NW == nullptr) {
        NW = nullptr;
        SE = nullptr;
        length_ = 0;
        return;
    }
    
    NW = new Node(other.NW->data);
    Node* currOther = other.NW->next;
    Node* currThis = NW;
    
    while (currOther != nullptr) {
        Node* newNode = new Node(currOther->data);
        currThis->next = newNode;
        newNode->prev = currThis;
        currThis = currThis->next;
        currOther = currOther->next;
    }

    SE = currThis;
    length_ = other.length_;
    
}

/**
 * If you have declared any private helper functions in chain_private.h,
 * add your completed implementations below.
**/

void Chain::WhiteBlock(PNG& png, int x, int y, int dim) {
	for (int row = 0; row < dim; row++) {
		for(int col = 0; col < dim; col++) {
			RGBAPixel* pixel = png.getPixel(x+col, y+col);
			pixel->r = 255;
			pixel->g = 255;
			pixel->b = 255;
			pixel->a = 1;
		}
	}
}

Node* Chain::reverseRow(Node* start, int cols) {
		if (cols <= 1 || start == nullptr) {
        return start;
    }

    Node* curr = start;
    Node* temp = nullptr;
	//tail is the previous curr
	Node* tail;

    for(int i = 0; i< cols; i++) {
		curr->data.FlipHorizontal();
		//swap prev and next of curr
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;

		//increment tail and curr
		tail = curr;
        curr = curr->prev;
    }
	//after row is done, swap the head->next and tail->prev
	temp= tail->prev;
	tail->prev = start->next;
	start->next = temp;

	//set nexthead->prev to start
	if (curr != nullptr){ 
		curr->prev = start;
	}

	return curr;
}