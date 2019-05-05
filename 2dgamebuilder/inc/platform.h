/**
* \date 23/02/2015
* \author Nicholas Messaritis
*
* \file platform.h
*
*/
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef WINDOWS
#define PLATFORM_WINDOWS 1
#elif  LINUX
#define PLATFORM_LINUX   1
#elif  OSX
#define PLATFORM_OSX	 1
#elif  IOS
#define PLATFORM_IOS     1
#elif  ANDROID
#define PLATFORM_ANDROID 1
#elif  PS3
#define PLATFORM_PS3	 1
#elif  PSP
#define PLATFORM_PSP	 1
#elif  PSVITA
#define PLATFORM_PSVITA  1
#elif  PS4
#define PLATFORM_PS4     1
#endif
#elif  XBOXONE
#define PLATFORM_XBOXONE 1
#elif  XBOX360
#define PLATFORM_XBOX360 1

#endif
