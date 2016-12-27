/* ========================================================================
File: BehaviorTree.cpp
Revision: 0.1
Creator: Alex J. Champandard, David Collado
Notice:  Code inspired by the Behavior Tree Starter Kit.
======================================================================== */

#include "BehaviorTree.h"

// Behavior functions

Status BehaviorNode::tick()
{
	if (m_eStatus != BH_RUNNING)
	{
		onInitialize();
	}

	m_eStatus = update();

	if (m_eStatus != BH_RUNNING)
	{
		onTerminate(m_eStatus);
	}
	return m_eStatus;
}

void BehaviorNode::reset()
{
	m_eStatus = BH_INVALID;
}

void BehaviorNode::abort()
{
	onTerminate(BH_ABORTED);
	m_eStatus = BH_ABORTED;
}

bool BehaviorNode::isTerminated()
{
	return m_eStatus == BH_SUCCESS || m_eStatus == BH_FAILURE;
}

bool BehaviorNode::isRunning()
{
	return m_eStatus == BH_RUNNING;
}

Status BehaviorNode::getStatus()
{
	return m_eStatus;
}

// Repeat functions

void Repeat::setCount(int count)
{
	m_iLimit = count;
}

void Repeat::onInitialize()
{
	m_iCounter = 0;
}

Status Repeat::update()
{
	for (;;)
	{
		m_pChild->tick();
		if (m_pChild->getStatus() == BH_RUNNING) break;
		if (m_pChild->getStatus() == BH_FAILURE) return BH_FAILURE;
		if (++m_iCounter == m_iLimit) return BH_SUCCESS;
		m_pChild->reset();
	}
	return BH_RUNNING;
}

// Sequence functions

void Sequence::onInitialize()
{
	m_CurrentChild = m_Children.begin();
}

Status Sequence::update()
{
	// Keep going until a child behavior says it's running or failure
	for (;;)
	{
		Status s = (*m_CurrentChild)->tick();

		// If the child fails, or keeps running, do the same.
		if (s != BH_SUCCESS)
		{
			return s;
		}

		// Hit the end of the array, job done!
		if (++m_CurrentChild == m_Children.end())
		{
			return BH_SUCCESS;
		}
	}
}

// Selector functions

void Selector::onInitialize()
{
	m_Current = m_Children.begin();
}

Status Selector::update()
{
	// Keep going until a child behavior says its running.
	for (;;)
	{
		Status s = (*m_Current)->tick();

		// If the child succeeds, or keeps running, do the same.
		if (s != BH_FAILURE)
		{
			return s;
		}

		// Hit the end of the array, it didn't end well...
		if (++m_Current == m_Children.end())
		{
			return BH_FAILURE;
		}
	}
}

// Parallel functions
Status Parallel::update()
{
	size_t iSuccessCount = 0, iFailureCount = 0;

	for (Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		BehaviorNode& b = **it;
		b.tick();

		if (b.getStatus() == BH_SUCCESS)
		{
			++iSuccessCount;
			if (m_eSuccessPolicy == RequireOne)
			{
				return BH_SUCCESS;
			}
		}

		if (b.getStatus() == BH_FAILURE)
		{
			++iFailureCount;
			if (m_eFailurePolicy == RequireOne)
			{
				return BH_FAILURE;
			}
		}
	}

	if (m_eFailurePolicy == RequireAll  &&  iFailureCount == m_Children.size())
	{
		return BH_FAILURE;
	}

	if (m_eSuccessPolicy == RequireAll  &&  iSuccessCount == m_Children.size())
	{
		return BH_SUCCESS;
	}

	return BH_RUNNING;
}

void Parallel::onTerminate(Status)
{
	for (Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		BehaviorNode& b = **it;
		if (b.isRunning())
		{
			b.abort();
		}
		b.reset();
	}
}

// ActiveSelector functions

void ActiveSelector::onInitialize()
{
	m_Current = m_Children.end();
}

Status ActiveSelector::update()
{
	Behaviors::iterator previous = m_Current;

	Selector::onInitialize(); // Sets m_Current to the start
	Status result = Selector::update();

	// If we should change of branch, we abort the previous one
	if (previous != m_Children.end() && m_Current != previous)
	{
		(*previous)->onTerminate(BH_ABORTED);
	}
	return result;
}