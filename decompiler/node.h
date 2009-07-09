#ifndef NODE_H
#define NODE_H

#include "instruction.h"

#include <list>

#include <boost/utility.hpp>


struct Node : boost::noncopyable {

	Node *_component;
	Node *_dominator;    // immediate dominator
	Node *_interval;     // header node of the interval this node belongs to
	int _number;         // number in post-order
	std::list<Node*> _in;
	std::list<Node*> _out;

	Node();
	virtual ~Node();

	virtual uint32 address() = 0;
	virtual std::string toString() = 0;
};


struct BasicBlock : public Node {

	std::list<Instruction*> _instructions;

	BasicBlock(std::list<Instruction*>::iterator first, std::list<Instruction*>::iterator last);
	~BasicBlock();

	uint32 address();
	std::string toString();
};


struct DerivedNode : public Node {

	Node *_primitive;     // interval header of the graph from which this graph has been derived

	DerivedNode(Node *primitive);
	~DerivedNode();

	uint32 address();
	std::string toString();
};

#endif
