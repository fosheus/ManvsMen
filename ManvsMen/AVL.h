#pragma once
#include <iostream>
#include<cctype>
#include <stdlib.h>
#include <conio.h>

using namespace std;

struct node
{
	int element;
	node *left;
	node *right;
	int height;
};
typedef struct node *nodeptr;
class AVL
{
public:
	void insert(int, nodeptr &);
	void del(int, nodeptr &);
	int deletemin(nodeptr &);
	bool find(int, nodeptr &);
	int findtop(nodeptr &);
	nodeptr findmin(nodeptr);
	nodeptr findmax(nodeptr);
	bool isempty(nodeptr &);
	void makeempty(nodeptr &);
	void copy(nodeptr &, nodeptr &);
	nodeptr nodecopy(nodeptr &);
	void preorder(nodeptr);
	void inorder(nodeptr);
	void postorder(nodeptr);
	int bsheight(nodeptr);
	nodeptr srl(nodeptr &);
	nodeptr drl(nodeptr &);
	nodeptr srr(nodeptr &);
	nodeptr drr(nodeptr &);
	int max(int, int);
	int nonodes(nodeptr);
};

