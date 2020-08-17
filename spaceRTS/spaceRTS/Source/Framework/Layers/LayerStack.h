#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace MYENGINE {

	class MYENGINE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void clear();
		void EraseNonStaticLayers();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		int size() { return m_Layers.size(); }
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}