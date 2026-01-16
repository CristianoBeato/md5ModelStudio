/*
===============================================================
===============================================================
*/

#ifndef __MAIN_PRECOMPILED_HPP__
#define __MAIN_PRECOMPILED_HPP__

#include <cstdio>
#include <cstdint>
#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <SDL3/SDL_timer.h> // SDL_GetTics
#include <SDL3/SDL_mutex.h> // SDL_Mutex

// util
struct SDL_ScopedLock
{
    SDL_Mutex* mutex;

    SDL_ScopedLock( SDL_Mutex* &in_lock ) : mutex( in_lock )
    {
        SDL_LockMutex( mutex );
    }

    inline ~SDL_ScopedLock( void )
    {
        SDL_UnlockMutex( mutex );
        mutex = nullptr;
    }
};

// gl matematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "crglCore.hpp"
#include "Gwen/Gwen.h"
//#include "Image.hpp"
#include "md5Mesh/md5Model.hpp"
#include "dialogs/ViewStructures.hpp"

#endif //__MAIN_PRECOMPILED_HPP__