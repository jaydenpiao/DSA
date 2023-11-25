/**
 * @file qtree-private.h
 * @description student declaration of private QTree functions
 *              CPSC 221 PA3
 *
 *              SUBMIT THIS FILE.
 * 
 *				Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

// begin your declarations below


RGBAPixel getAvColor(Node* nd);
void copyTree(Node*& destination, const Node* source);
void destroyTree(Node* &curr);
void renderTree(PNG& image,const Node* current, unsigned int scale) const;
void flipTree(Node*& current);
void rotateNode(Node*& current);
bool shouldPrune(Node* node, RGBAPixel avgColor, double tolerance) const;
bool checkTolerance(Node* node, RGBAPixel avgColor, double tolerance) const;
void pruneNode(Node*& node, double tolerance);
void clearSubtree(Node*& node);
bool isLeaf(Node* node) const;