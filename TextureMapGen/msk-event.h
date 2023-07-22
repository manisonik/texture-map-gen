#pragma once
#include "msk-eventhandler.h"
/*
class Event
{
private:
	std::vector<std::unique_ptr<EventHandler>> handlers;

public:
	Event() {}
	~Event() {}

	Event &operator+=(const EventHandler &handler) {
		handlers.push_back(std::unique_ptr<EventHandler>(new EventHandler{ handler }));
		return *this;
	}

	Event &operator-=(const EventHandler &handler) {
		for (auto to_remove = handlers.begin(); to_remove != this->handlers.end(); ++to_remove) {
			if (*(*to_remove) == handler) {
				this->handlers.erase(to_remove);
				break;
			}
		}
		return *this;
	}

	void operator()(MskObject* sender, EventArgs* e) {
		for (auto func = handlers.begin();func != this->handlers.end(); ++func) {
			if (*func != nullptr) {
				(*(*func))(sender, e);
			}
		}
	}
};*/

template<class A>
class Event
{
private:
	std::vector<std::unique_ptr<EventHandler<A>>> handlers;

public:
	Event() {}
	~Event() {}

	Event &operator+=(const EventHandler<A> &handler) {
		handlers.push_back(std::unique_ptr<EventHandler<A>>(new EventHandler<A>{ handler }));
		return *this;
	}

	Event &operator-=(const EventHandler<A> &handler) {
		for (auto to_remove = handlers.begin(); to_remove != this->handlers.end(); ++to_remove) {
			if (*(*to_remove) == handler) {
				this->handlers.erase(to_remove);
				break;
			}
		}
		return *this;
	}

	void operator()(MskObject* sender, A* e) {
		for (auto func = handlers.begin(); func != this->handlers.end(); ++func) {
			if (*func != nullptr) {
				(*(*func))(sender, e);
			}
		}
	}
};

/*class Event {
	std::vector<EventHandler> handlers;

	void notifyHandlers(MskObject* sender, EventArgs& e) {
		for (auto func = this->handlers.begin(); func != this->handlers.end(); ++func) {
			if (*func != nullptr && (func)->id != 0) {
				((*func))(sender, e);
			}
		}
	}
public:
	void addHandler(const EventHandler &handler) {
		this->handlers.push_back(EventHandler{ handler });
	}

	void removeHandler(const EventHandler &handler) {
		std::vector<EventHandler>::iterator to_remove = this->handlers.begin();
		for (; to_remove != this->handlers.end(); ++to_remove) {
			if ((*to_remove) == handler) {
				this->handlers.erase(to_remove);
				break;
			}
		}
	}

	void operator()(MskObject* sender, EventArgs& e) {
		this->notifyHandlers(sender, e);
	}

	Event &operator+=(const EventHandler &handler) {
		this->addHandler(handler);

		return *this;
	}

	Event &operator-=(const EventHandler &handler) {
		this->removeHandler(handler);

		return *this;
	}
};*/

