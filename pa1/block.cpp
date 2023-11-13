/**
 * File: block.cpp
 * Description: student implementation for Block functions
 *              for CPSC 221 2023W1 PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
**/

#include "block.h"
#include <vector>
#include <algorithm>
using namespace std;
/**
 * Creates a square Block that is dimension x dimension pixels in size,
 * starting at (x, y) on img. Assumes that the requested
 * Block fits on the image (inputs are feasible)
**/
void Block::Build(PNG& img, unsigned int x, unsigned int y, unsigned int dimension) {
	// complete your implementation below
    for (unsigned int row = 0; row < dimension; row++) {
		vector<RGBAPixel> currVector;
		for (unsigned int col = 0; col < dimension; col++) {
			currVector.push_back(*img.getPixel(x+col,y+row));
		}
		this->data.push_back(currVector);
	}
}

/**
 * Renders this Block onto img with its upper-left corner at (x, y).
 * Assumes block fits on the image.
 * 
 * @param full - whether to render the entire contents to the target PNG,
 *               or the average color to a single pixel in the target PNG.
**/
void Block::Render(PNG& img, unsigned int x, unsigned int y, bool full) const {
	// complete your implementation below
	if (full) {
		for (unsigned int row = 0; row < Dimension(); row++) {
			for (unsigned int col = 0; col < Dimension(); col++) {
				*img.getPixel(col + x, row + y) = this->data[row][col];
			}
		}
	} else {
		*img.getPixel(x,y) = GetAverageColor();
	}
	
}

/**
 * Flips the Block's pixel data horizontally by a simple reflection
 * across the vertical axis through its middle.
**/
void Block::FlipHorizontal() {
	for(unsigned int x = 0; x < Dimension(); x++) {
		int i = 0;
		int k = Dimension() - 1;
		while (i < k) {
			RGBAPixel temp = data[x][i];
			data[x][i] = data[x][k];
			data[x][k] = temp;
			i++;
			k--;
		}
	}    
}

/**
 * Flips the Block's pixel data vertically by a simple reflection
 * across the horizontal axis through its middle.
**/
void Block::FlipVertical() {
    int i = 0;
    int k = Dimension() - 1;
    while (i < k) {
        vector<RGBAPixel> temp = data[i];
        data[i] = data[k];
        data[k] = temp;
        i++;
        k--;
	}
}

/**
 * Returns the dimension of the block (its width or height).
 */
unsigned int Block::Dimension() const {
	// replace the statement below with your implementation
	return data.size();
}



/**
 * Returns the average color of the pixels in the block's data.
**/
RGBAPixel Block::GetAverageColor() const {
	unsigned int totalRed = 0;
	unsigned int totalGreen = 0;
	unsigned int totalBlue = 0;
	double totalAlpha = 0;
	for (unsigned int row = 0; row < Dimension(); row++) {
		for (unsigned int col = 0; col < Dimension(); col++) {
			RGBAPixel p = this->data[row][col];
			totalRed += p.r;
			totalGreen += p.g;
			totalBlue += p.b;
			totalAlpha += p.a;
		}
	}
	unsigned int dim = Dimension() * Dimension();
	return RGBAPixel(totalRed/dim, totalGreen/dim, totalBlue/dim, totalAlpha/dim);
}



/**
 * Replaces all pixel data in the block with the block's average color.
**/
void Block::FillAverage() {
	RGBAPixel pixel = GetAverageColor();
	for (unsigned int row = 0; row < Dimension(); row++) {
		for (unsigned int col = 0; col < Dimension(); col++) {
			this->data[row][col] = pixel;
		}
	}
}