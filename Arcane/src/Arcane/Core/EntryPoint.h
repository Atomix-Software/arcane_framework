#pragma once

#ifdef ARC_PLATFORM_WINDOWS

extern Arcane::Application* Arcane::CreateApplication();

int main(int argc, char* argv[])
{
	Arcane::Log::Init();

	ARC_PROFILE_BEGIN_SESSION("Startup", "ArcaneProfile-Startup.json");
	auto app = Arcane::CreateApplication();
	ARC_PROFILE_END_SESSION();

	ARC_PROFILE_BEGIN_SESSION("Runtime", "ArcaneProfile-Runtime.json");
	app->Run();
	ARC_PROFILE_END_SESSION();

	ARC_PROFILE_BEGIN_SESSION("Shutdown", "ArcaneProfile-Shutdown.json");
	delete app;
	ARC_PROFILE_END_SESSION();

	return 0;
}

#endif