# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2017-2025, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

include(CheckCXXCompilerFlag)

if(NOT CMAKE_CXX_STANDARD)
    if(${PROJECT_NAME}_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD ${PROJECT_NAME}_CXX_STANDARD)
    else()
        set(CMAKE_CXX_STANDARD 20)
    endif()
endif()

if(CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "{$PROJECT_NAME} requires C++17 or greater")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13.0)
    if(CMAKE_CXX_STANDARD LESS 20)
        message(
            FATAL_ERROR
                " using gcc>=13.0 {$PROJECT_NAME} requires setting C++ standard to C++20 or greater"
        )
    endif()
endif()

if(MSVC)
    if(CMAKE_CXX_STANDARD EQUAL 17)
        check_cxx_compiler_flag(/std:c++17 has_std_17_flag)
        if(has_std_17_flag)
            set(CXX_STANDARD_FLAG /std:c++17)
            set(has_std_1z_flag ON)
        else()
            set(CXX_STANDARD_FLAG /std:c++latest)
        endif()
    elseif(CMAKE_CXX_STANDARD EQUAL 20)
        check_cxx_compiler_flag(/std:c++20 has_std_20_flag)
        if(has_std_20_flag)
            set(CXX_STANDARD_FLAG /std:c++20)
            set(has_std_2a_flag ON)
        else()
            set(CXX_STANDARD_FLAG /std:c++latest)
        endif()
    else()
        set(CXX_STANDARD_FLAG /std:c++latest)
    endif()
else()
    if(CMAKE_CXX_STANDARD GREATER_EQUAL 23)
        check_cxx_compiler_flag(-std=c++23 has_std_23_flag)
        if(has_std_23_flag)
            set(CXX_STANDARD_FLAG -std=c++23)
            set(has_std_2b_flag ON)
        else()
            check_cxx_compiler_flag(-std=c++2b has_std_2b_flag)
            if(has_std_2b_flag)
                set(CXX_STANDARD_FLAG -std=c++2b)
            endif()
        endif()
    elseif(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        check_cxx_compiler_flag(-std=c++20 has_std_20_flag)
        if(has_std_20_flag)
            set(CXX_STANDARD_FLAG -std=c++20)
            set(has_std_2a_flag ON)
        else()
            check_cxx_compiler_flag(-std=c++2a has_std_2a_flag)
            if(has_std_2a_flag)
                set(CXX_STANDARD_FLAG -std=c++2a)
            endif()
        endif()
    elseif(CMAKE_CXX_STANDARD EQUAL 17)
        if(CYGWIN AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CXX_STANDARD_FLAG -std=gnu++17)
        else()
            set(CXX_STANDARD_FLAG -std=c++17)
        endif()

    else()
        message(FATAL_ERROR "{$PROJECT_NAME} requires C++17 or Higher")
    endif()

endif()
# set(CMAKE_REQUIRED_FLAGS ${CXX_STANDARD_FLAG})
