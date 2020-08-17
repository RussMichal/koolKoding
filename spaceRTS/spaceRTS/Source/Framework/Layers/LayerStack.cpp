#include "MEpch.h"
#include "LayerStack.h"

namespace MYENGINE {

	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::clear()
	{
		for (Layer* layer : m_Layers)
			delete layer;
		m_Layers.clear();
		m_LayerInsert = m_Layers.begin();
	}

	void LayerStack::EraseNonStaticLayers()
	{
		for (int i=0; i<m_Layers.size();i++)
			if (!m_Layers[i]->staticLayer)
			{
				delete m_Layers[i];
				m_Layers.erase(m_Layers.begin()+i);
				i--;
			}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

}