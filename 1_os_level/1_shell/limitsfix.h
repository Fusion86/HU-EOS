#pragma once

// Taken from https://github.com/edaniels/cop4610-project-1/blob/master/src/limitsfix.h

// Lets us use HOST_NAME_MAX on Mac OS X which
// still hasn't defined HOST_NAME_MAX
#ifndef HOST_NAME_MAX
	#ifdef _POSIX_HOST_NAME_MAX
		#define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
	#else
		#define HOST_NAME_MAX 255
	#endif
#endif

// Lets us use LOGIN_NAME_MAX on Mac OS X which
// still hasn't defined LOGIN_NAME_MAX
#ifndef LOGIN_NAME_MAX
	#ifdef _POSIX_LOGIN_NAME_MAX
		#define LOGIN_NAME_MAX _POSIX_LOGIN_NAME_MAX
	#else
		#define LOGIN_NAME_MAX 32
	#endif
#endif
