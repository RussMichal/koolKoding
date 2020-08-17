#pragma once

#ifdef MYENGINE_PLATFORM_WINDOWS

extern MYENGINE::Application* MYENGINE::CreateApp();

int main()
{
	auto app = MYENGINE::CreateApp();
	app->Run();
	delete app;

	return 0;
}

#endif //MYENGINE_PLATFORM_WINDOWS