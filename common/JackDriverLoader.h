/*
Copyright (C) 2001-2005 Paul Davis
Copyright (C) 2004-2008 Grame

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __JackDriverLoader__
#define __JackDriverLoader__

#include "driver_interface.h"
#include "control_types.h"
#include "JackPlatformSynchro.h"


#ifdef WIN32

#include <windows.h>
#define DRIVER_HANDLE HINSTANCE
#define LoadDriverModule(name) LoadLibrary((name))
#define UnloadDriverModule(handle) (FreeLibrary(((HMODULE)handle)))
#define GetProc(handle, name) GetProcAddress(((HMODULE)handle),(name))

#else

#include <dlfcn.h>
#define DRIVER_HANDLE void*
#define LoadDriverModule(name) dlopen((name), RTLD_NOW | RTLD_GLOBAL)
#define UnloadDriverModule(handle) dlclose((handle))
#define GetProc(handle, name) dlsym((handle), (name))

#endif

namespace Jack
{
    class JackDriverClientInterface;
    class JackLockedEngine;
};

typedef jack_driver_desc_t * (*JackDriverDescFunction) ();
typedef Jack::JackDriverClientInterface* (*initialize) (Jack::JackLockedEngine*, Jack::JackSynchro*, const JSList*);

typedef struct _jack_driver_info
{
    Jack::JackDriverClientInterface* (*initialize)(Jack::JackLockedEngine*, Jack::JackSynchro*, const JSList*);
    DRIVER_HANDLE handle;
}
jack_driver_info_t;

jack_driver_desc_t * jack_find_driver_descriptor (JSList * drivers, const char * name);
jack_driver_desc_t * jack_drivers_get_descriptor (JSList * drivers, const char * sofile);

JSList * jack_drivers_load (JSList * drivers);
JSList * jack_internals_load (JSList * internals);

jack_driver_info_t * jack_load_driver (jack_driver_desc_t * driver_desc);
EXPORT int jackctl_parse_driver_params (jackctl_driver * driver_ptr, int argc, char* argv[]);
EXPORT void jack_free_driver_params(JSList * param_ptr);

#endif

