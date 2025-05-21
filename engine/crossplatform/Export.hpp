#pragma once

/// windows is stupid as shit and needs to manually
/// export dll functions :pensive:
/// one of the many reasons to hate msvc
#if defined(_WIN32)
#define ENGINE_CROSSPLATFORM_EXPORT __declspec(dllexport) 
#else
#define ENGINE_CROSSPLATFORM_EXPORT
#endif