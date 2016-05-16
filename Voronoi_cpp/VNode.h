#pragma once

#include<memory>
#include<vector>
using namespace std;

#ifndef VNODE_H
#define VNODE_H

class VNode : public enable_shared_from_this<VNode>
{
private:
	shared_ptr<VNode> _Parent; shared_ptr<VNode> _Left; shared_ptr<VNode> _Right;
public:
	void Parent(shared_ptr<VNode> p) { _Parent = p; }
	shared_ptr<VNode> Parent() { return _Parent; }

	void Left(shared_ptr<VNode> p) { _Left = p; p->Parent(shared_from_this()); }
	shared_ptr<VNode> Left() { return _Left; }

	void Right(shared_ptr<VNode> p) { _Right = p; p->Parent(shared_from_this()); }
	shared_ptr<VNode> Right() { return _Right; }

	void Replace(shared_ptr<VNode> ChildOld, shared_ptr<VNode> ChildNew)
	{
		if (_Left->Left() == ChildOld->Left() && _Left->Right() == ChildOld->Right() && _Left->Parent() == ChildOld->Parent())
			_Left = ChildNew;

		else if (_Right->Left() == ChildOld->Left() && _Right->Right() == ChildOld->Right() && _Right->Parent() == ChildOld->Parent())
			_Right = ChildNew;
		else throw new exception("Child not found!");
		ChildOld->Parent(NULL);
	}
	VNode operator=(const VNode& other)
	{
		_Left = other._Left;
		_Right = other._Right;
		_Parent = other._Parent;
		return *this;
	}
	virtual ~VNode(){}
};

#endif