// Contents of DLLDefines.h
#ifndef _LibCommon_DLLDEFINES_H_
#define _LibCommon_DLLDEFINES_H_

/* Cmake will define LibCommon_EXPORTS on Windows when it
configures to build a shared library. If you are going to use
another build system on windows or create the visual studio
projects by hand you need to define LibCommon_EXPORTS when
building a DLL on windows.
*/
// We are using the Visual Studio Compiler and building Shared libraries

#if defined (_WIN32)
#if defined(LibCommon_EXPORTS)
#define  EXPORT __declspec(dllexport)
#else
#define  EXPORT __declspec(dllimport)
#endif /* LibCommon_EXPORTS */
#else /* defined (_WIN32) */
#define EXPORT
#endif

#endif /* _LibCommon_DLLDEFINES_H_ */