#pragma once

#include "msk-object.h"
#include "msk-eventhandler.h"

namespace Manisonik {
	template<class A>
	class MEvent
	{
	private:
		std::vector<std::unique_ptr<MEventHandler<A>>> handlers;

	public:
		MEvent() {}
		~MEvent() {}

		MEvent& operator+=(const MEventHandler<A>& handler) {
			handlers.push_back(std::unique_ptr<MEventHandler<A>>(new MEventHandler<A>{ handler }));
			return *this;
		}

		MEvent& operator-=(const MEventHandler<A>& handler) {
			for (auto to_remove = handlers.begin(); to_remove != this->handlers.end(); ++to_remove) {
				if (*(*to_remove) == handler) {
					this->handlers.erase(to_remove);
					break;
				}
			}
			return *this;
		}

		void operator()(MObject* sender, A* e) {
			for (auto func = handlers.begin(); func != this->handlers.end(); ++func) {
				if (*func != nullptr) {
					(*(*func))(sender, e);
				}
			}
		}
	};
}