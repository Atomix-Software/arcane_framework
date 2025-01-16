#pragma once

#include "Arcane/Core/Base.h"
#include "Arcane/Core/Timestep.h"
#include "Arcane/Events/Event.h"

namespace Arcane
{

	class ARC_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnImGuiRender() {}

		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}