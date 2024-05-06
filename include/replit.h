#pragma once

#if !(defined(_WIN32) || defined(_WIN64)  || defined(__APPLE__ ))

#ifndef REPLIT
#define REPLIT

#define REPLIT_WARNING \
"WARNING: You are using the REPLIT macro, thereby it is assumed you are using \
Replit to compile this project, and all graphics operations will be disabled. \
If you are not on Replit, remove the definition of the REPLIT macro in include/replit.h"

#if _MSC_VER && !__INTEL_COMPILER // thanks https://stackoverflow.com/questions/5850358/is-there-a-preprocessor-define-that-is-defined-if-the-compiler-is-msvc
#pragma message(REPLIT_WARNING)
#else
#warning REPLIT_WARNING
#endif

#undef REPLIT_WARNING

#endif

#endif