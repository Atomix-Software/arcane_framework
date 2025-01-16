#pragma once

#include "Arcane/Core/Layer.h"

#include "Arcane/Events/ApplicationEvent.h"
#include "Arcane/Events/KeyEvent.h"
#include "Arcane/Events/MouseEvent.h"

namespace Arcane
{

	class ARC_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		inline void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents;
		float m_Time;

	};

}
