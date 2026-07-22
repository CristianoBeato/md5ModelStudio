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
#include <list>
#include <memory>
#include <stdexcept>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h> // SDL_CreateWindow /
#include <SDL3/SDL_timer.h> // SDL_GetTics
#include <SDL3/SDL_thread.h> // SDL_CreateThread
#include <SDL3/SDL_mutex.h> // SDL_Mutex

// gl matematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "crglCore.hpp"

#include "framework/pointers.hpp"
#include "framework/threads.hpp"
#include "framework/tasks.hpp"

//#include "Image.hpp"
#include "md5Mesh/md5Model.hpp"
#include "dialogs/MainDialog.hpp"
#include "dialogs/ViewStructures.hpp"

#endif //__MAIN_PRECOMPILED_HPP__