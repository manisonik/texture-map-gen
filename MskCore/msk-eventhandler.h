#pragma once
#include "msk-object.h"

namespace Manisonik {
	template <class A>
	class MEventHandler
	{
	private:
		std::function<void(MObject*, A*)> m_pFuncPtr;

	public:
		template<class T>
		MEventHandler(MObject* sender, void(T::* func)(MObject*, A*)) {
			m_pFuncPtr = std::bind(
				static_cast<void(MObject::*)(MObject*, A*)>(func),
				sender,
				std::placeholders::_1,
				std::placeholders::_2
			);
		}

		~MEventHandler() {}

		void operator()(MObject* sender, A* e)
		{
			m_pFuncPtr(sender, e);
		}

		void operator=(const MEventHandler& e)
		{
			if (this->m_pFuncPtr == nullptr) {
				this->m_pFuncPtr = e.m_pFuncPtr;
			}
		}

		bool operator==(const MEventHandler& e)
		{
			return false;
		}

		bool operator!=(nullptr_t) noexcept
		{
			return this->m_pFuncPtr != nullptr;
		}
	};
}

