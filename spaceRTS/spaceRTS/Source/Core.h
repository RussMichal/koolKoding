#pragma once

#ifdef MYENGINE_PLATFORM_WINDOWS
	#ifdef MYENGINE_BUILD_DLL
		#define MYENGINE_API __declspec(dllexport)
	#else
		#define MYENGINE_API __declspec(dllimport)
	#endif
#else
	#error MyEngine supports windows only!
#endif

#define BIT(x) (1<<(x))
#define NUM(x) (log((float)x)/log(2.0f))

#ifdef ME_ENABLE_ASSERTS
	#define ME_ASSERT(x, ...) { if(!(x)) { ME_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ME_CORE_ASSERT(x, ...) { if(!(x)) { ME_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ME_ASSERT(x, ...)
	#define ME_CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT(x) std::bind(&x,this,std::placeholders::_1)
