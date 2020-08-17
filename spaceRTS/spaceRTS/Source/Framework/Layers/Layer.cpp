#include "MEpch.h"
#include "Layer.h"

namespace MYENGINE {

	Layer::Layer(const std::string& debugName, const bool& i_staticLayer)
		: m_debugName(debugName)
		, staticLayer(i_staticLayer)
	{
	}

	Layer::~Layer()
	{

	}

}