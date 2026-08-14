/*
 * nevonex/dllNevonex.hpp
 * Copyright (c) Robert Bosch GmbH. All rights reserved.
 */

#ifndef _DLL_NEVONEX_HPP
#define _DLL_NEVONEX_HPP
/*
 * Defines 3 preprocessor symbols:
 * - MAKE_NEVONEX_DLL  set to 1 when building the dll, else unset
 * - USE_NEVONEX_DLL    set to 1 when using the dll, else unset
 * - EXPORT_NEVONEX_DLL  always set, import when using the dll,
 *          export when building dll
 *
 * Pattern taken from qglobal.h
 */

#if defined(__WIN32__) || defined(_WIN32)

#   if defined(MAKE_NEVONEX_DLL)
#       if defined(USE_NEVONEX_DLL)
#           undef USE_NEVONEX_DLL
#       endif

#       define EXPORT_NEVONEX_DLL __declspec(dllexport)
#       define EXTERN_NEVONEX_DLL __declspec(dllimport)

#   else
#       if !defined(USE_NEVONEX_DLL)
#           define USE_NEVONEX_DLL 1
#       endif

#       define EXPORT_NEVONEX_DLL __declspec(dllimport)
#       define EXTERN_NEVONEX_DLL __declspec(dllexport)

#   endif


#else

#   define EXPORT_NEVONEX_DLL
#   define EXTERN_NEVONEX_DLL

#endif

#endif // _NEVONEX_HPP
