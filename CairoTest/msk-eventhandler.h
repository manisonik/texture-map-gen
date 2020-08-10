#pragma once
#include "msk-object.h"
#include "msk-eventargs.h"

template <class A>
class EventHandler
{
private:
	std::function<void(MskObject*, A*)> m_pFuncPtr;

public:
	template<class T>
	EventHandler(MskObject* sender, void(T::* func)(MskObject*, A*)) {
		m_pFuncPtr = std::bind(
			static_cast<void(MskObject::*)(MskObject*, A*)>(func),
			sender,
			std::placeholders::_1,
			std::placeholders::_2
		);
	}

	~EventHandler() {}

	void operator()(MskObject * sender, A * e)
	{
		m_pFuncPtr(sender, e);
	}

	void operator=(const EventHandler & e)
	{
		if (this->m_pFuncPtr == nullptr) {
			this->m_pFuncPtr = e.m_pFuncPtr;
		}
	}

	bool operator==(const EventHandler & e)
	{
		return false;
	}

	bool operator!=(nullptr_t) noexcept
	{
		return this->m_pFuncPtr != nullptr;
	}
};

/*
template <class A>
class EventHandler {
private:
	MskObject* m_ClassPtr;
	void(MskObject::* m_FuncPtr)(MskObject*, A*);

public:
	int id;

	EventHandler() : id{ 0 } { }

	template<class T>
	EventHandler(T* o, void(T::* func)(MskObject*, A*)) {
		m_ClassPtr = o;
		m_FuncPtr = (void(MskObject::*)(MskObject*, A*))(func);
	}

	void operator()(MskObject* sender, A* e) {
		(m_ClassPtr->*m_FuncPtr)(sender, e);
	}

	void operator=(const EventHandler &func) {
		if (this->m_FuncPtr == nullptr) {
			this->m_FuncPtr = func.m_FuncPtr;
		}
	}

	bool operator==(const EventHandler &del) {
		return this->id == del.id;
	}
	bool operator!=(nullptr_t) {
		return this->m_FuncPtr != nullptr;
	}
};
*/
