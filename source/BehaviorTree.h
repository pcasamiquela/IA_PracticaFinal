/* ========================================================================
File: BehaviorTree.h
Revision: 0.1
Creator: Alex J. Champandard, David Collado
Notice:  Code inspired by the Behavior Tree Starter Kit.
======================================================================== */

#pragma once

#include <vector>

// ============================================================================

enum Status
	/**
	* Return values of and valid states for behaviors.
	*/
{
	BH_INVALID,
	BH_SUCCESS,
	BH_FAILURE,
	BH_RUNNING,
	BH_ABORTED, // Used to abort nodes in Parallel
};

class BehaviorNode
	/**
	* Base class for actions, conditions and composites.
	*/
{
public:
	virtual Status update() = 0;

	virtual void onInitialize() {}
	virtual void onTerminate(Status) {}

	BehaviorNode()
		: m_eStatus(BH_INVALID)
	{}

	virtual ~BehaviorNode()
	{}

	Status BehaviorNode::tick();
	void BehaviorNode::reset();
	void BehaviorNode::abort();
	bool BehaviorNode::isTerminated();
	bool BehaviorNode::isRunning();
	Status BehaviorNode::getStatus();

private:
	Status m_eStatus;
};

// ============================================================================
// Leaf Node

template <class T>
class LeafNode : public BehaviorNode
{
public:
	typedef void(T::*POINTER_FUNCTION_INIT)();
	typedef Status(T::*POINTER_FUNCTION_UPDATE)();
	typedef void(T::*POINTER_FUNCTION_TERMINATE)();

	LeafNode(T *instance, POINTER_FUNCTION_INIT initFunction,
		POINTER_FUNCTION_UPDATE updateFunction,
		POINTER_FUNCTION_TERMINATE terminateFunction)
		: BehaviorNode()
		, m_instance(instance)
		, m_initFunction(initFunction)
		, m_updateFunction(updateFunction)
		, m_terminateFunction(terminateFunction)
	{
	}

	virtual void onInitialize() override
	{
		if (m_initFunction != nullptr)
		{
			(m_instance->*m_initFunction)();
		}
	}

	virtual void onTerminate(Status) override
	{
		if (m_terminateFunction != nullptr)
		{
			(m_instance->*m_terminateFunction)();
		}
	}

	virtual Status update() override
	{
		if (m_updateFunction != nullptr)
		{
			return (m_instance->*m_updateFunction)();
		}
		else
		{
			return Status::BH_SUCCESS;
		}
	}

private:
	POINTER_FUNCTION_INIT m_initFunction;
	POINTER_FUNCTION_UPDATE m_updateFunction;
	POINTER_FUNCTION_TERMINATE m_terminateFunction;

	T* m_instance; // Instance Pointer
};

// ============================================================================

class Decorator : public BehaviorNode
{
protected:
	BehaviorNode* m_pChild;

public:
	Decorator(BehaviorNode* child) : m_pChild(child) {}
};

class Repeat : public Decorator
{
public:
	Repeat(BehaviorNode* child)
		: Decorator(child)
	{
	}

	void setCount(int count);
	void onInitialize();
	Status update();

protected:
	int m_iLimit;
	int m_iCounter;
};

// ============================================================================

class Composite : public BehaviorNode
{
public:
	void addChild(BehaviorNode* child) { m_Children.push_back(child); }
	void clearChildren() { m_Children.clear(); }

protected:
	typedef std::vector<BehaviorNode*> Behaviors;
	Behaviors m_Children;
};

class Sequence : public Composite
{
protected:
	virtual ~Sequence()
	{
	}

	virtual void onInitialize();
	virtual Status update();

	Behaviors::iterator m_CurrentChild;
};

// ============================================================================

class Selector : public Composite
{
public:
	virtual ~Selector()
	{
	}
protected:
	virtual void onInitialize();
	virtual Status update();

	Behaviors::iterator m_Current;
};

// ----------------------------------------------------------------------------

class Parallel : public Composite
{
public:
	enum Policy
	{
		RequireOne,
		RequireAll,
	};

	Parallel(Policy forSuccess, Policy forFailure)
		: m_eSuccessPolicy(forSuccess)
		, m_eFailurePolicy(forFailure)
	{
	}

	virtual ~Parallel() {}

protected:
	Policy m_eSuccessPolicy;
	Policy m_eFailurePolicy;

	virtual Status update();
	virtual void onTerminate(Status);
};

class Monitor : public Parallel
{
public:
	Monitor()
		: Parallel(Parallel::RequireAll, Parallel::RequireOne)
	{
	}

	void addCondition(BehaviorNode* condition)
	{
		m_Children.insert(m_Children.begin(), condition);
	}

	void addAction(BehaviorNode* action)
	{
		m_Children.push_back(action);
	}
};


// ============================================================================

class ActiveSelector : public Selector
{
protected:
	virtual void onInitialize();
	virtual Status update();
};

class BehaviorTree
{
public:
	// Constructor
	BehaviorTree(BehaviorNode* root)
		: m_root(root)
	{ }

	void tick()
	{
		m_root->tick();
	}
protected:
	BehaviorNode* m_root;
};

