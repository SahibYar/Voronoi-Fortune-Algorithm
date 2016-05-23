#pragma once

#include<memory>
#include<vector>
using namespace std;

#ifndef VNODE_H
#define VNODE_H

class VNode : public enable_shared_from_this<VNode>
{
private:
	shared_ptr<VNode> _Parent = nullptr, _Left = nullptr, _Right = nullptr;
public:
	void Parent(const shared_ptr<VNode> &p) 
	{ 
		_Parent = p; 
	}
	const shared_ptr<VNode> &Parent() const { return _Parent; }

	void Left(const shared_ptr<VNode> &p)
	{
		_Left = p;
		p->Parent(shared_from_this());
	}
	const shared_ptr<VNode> &Left() const { return _Left; }

	void Right(const shared_ptr<VNode> &p)
	{
		_Right = p;
		p->Parent(shared_from_this());
	}
	const shared_ptr<VNode> &Right() const { return _Right; }
	
	VNode()
	{
		_Parent = nullptr, _Left = nullptr, _Right = nullptr;
	}
	void Replace(const shared_ptr<VNode> &ChildOld, const shared_ptr<VNode> &ChildNew)
	{
		if (Left() == ChildOld)
			Left(ChildNew);
		else if (Right() == ChildOld)
			Right(ChildNew);
		else throw new exception("Child not found!");

		ChildOld->Parent(nullptr);
	}
	virtual ~VNode(){}
};

#endif