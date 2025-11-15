set( CRGL_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/crglBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglBuffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglContext.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglCore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglCore.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglFence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglFence.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglFormat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglFormat.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglFrameBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglFrameBuffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglImage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglImage.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglPrecompiled.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglSampler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglSampler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglShaders.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglShaders.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crglVertexArray.cpp
    )

add_library( crglLib STATIC ${CRGL_SOURCE} )
target_precompile_headers( crglLib PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/crglCore.hpp>" )
