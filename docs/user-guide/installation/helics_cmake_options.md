# HELICS CMake options

## Main Options

- `CMake_INSTALL_PREFIX`: CMake variable listing where to install the files
- `HELICS_BUILD_APP_LIBRARY` : \[Default=ON\] Tell HELICS to build the helics apps shared library
- `HELICS_BUILD_APP_EXECUTABLES` : \[Default=ON\] Build some executables associated with the apps
- `HELICS_BUILD_BENCHMARKS` : \[Default=OFF\] Build some timing benchmarks associated with HELICS
- `HELICS_BUILD_CXX_SHARED_LIB` : \[Default=OFF\] Build C++ shared libraries of the Application API C++ interface to HELICS and if `HELICS_BUILD_APP_LIBRARY` is also enabled another C++ shared library with the APP library
- `HELICS_BUILD_EXAMPLES` : \[Default=OFF\] Build a few select examples using HELICS, this is mostly for testing purposes. The main examples repo is [here](https://github.com/GMLC-TDC/HELICS-Examples)
- `HELICS_BUILD_TESTS` : \[Default=OFF\] Build the HELICS unit and system test executables.
- `HELICS_ENABLE_LOGGING` : \[Default=ON\] Enable debug and higher levels of logging, if this is turned off that capability is completely removed from HELICS
- `HELICS_ENABLE_PACKAGE_BUILD` : \[Default=OFF\] Enable the generation of some installer packages for HELICS
- `HELICS_GENERATE_DOXYGEN_DOC` : \[Default=OFF\] Generate doxygen documentation for HELICS
- `HELICS_WITH_CMAKE_PACKAGE` : \[Default=ON\] Generate a `HELICSConfig.cmake` file on install for loading into other libraries
- `HELICS_BUILD_JAVA_INTERACE` : \[Default=OFF\] Build the HELICS Java Interface
- `HELICS_BUILD_CSHARP_INTERACE` : \[Default=OFF\] Build the HELICS C# Interface
- `CMAKE_CXX_STANDARD` : Specify the C++ standard to use in building, HELICS 3.0 requires 17 or higher which will be used if nothing is specified.
- `HELICS_INSTALL` :\[Default=ON\] If set to off HELICS will not generate any install instructions

NOTE: All HELICS options are prefixed with HELICS\_ to separate them from other libraries so HELICS can be used cleanly as a subproject.

## Advanced Options

There are several different additional options available to configure HELICS for particular situations, most of which are not needed for general use and the default options should suffice.

### HELICS Configuration options

These options effect the configuration of HELICS itself and how/what gets built into the HELICS core libraries

- `HELICS_ENABLE_ZMQ_CORE` : \[Default=ON\] Enable the HELICS ZeroMQ related core types
- `HELICS_ENABLE_TCP_CORE` : \[Default=ON\] Enable the HELICS TCP related core types
- `HELICS_ENABLE_UDP_CORE` : \[Default=ON\] Enable the HELICS UDP core type
- `HELICS_ENABLE_IPC_CORE` : \[Default=ON\] Enable the HELICS interprocess shared memory related core types
- `HELICS_ENABLE_TEST_CORE` : \[Default=OFF\] Enable the HELICS in process core type with some additional features for tests, required and enabled if the `HELICS_BUILD_TESTS` option is enabled
- `HELICS_ENABLE_INPROC_CORE` : \[Default=ON\] Enable the HELICS in process core type, required if `HELICS_BUILD_BENCHMARKS` is on
- `HELICS_ENABLE_MPI_CORE` : \[Default=OFF\] Enable the HELICS Message Passing Interface (MPI) related core types, most commonly used for High Performance Computing applications (HPC)

#### HELICS logging Options

- `HELICS_ENABLE_TRACE_LOGGING` : \[Default=ON\] Enable trace level of logging inside HELICS, if this is turned off that capability is completely removed from HELICS
- `HELICS_ENABLE_DEBUG_LOGGING` : \[Default=ON\] Enable debug levels of logging inside HELICS, if this is turned off that capability is completely removed from HELICS

### Build configuration Options

Options effect the connection of libraries used in HELICS and how they are linked.

- `HELICS_DISABLE_BOOST` : \[Default=OFF\] Completely turn off searching and inclusion of boost libraries. This will disable the IPC core, disable the webserver and few other features, possibly more in the future.
- `HELICS_DISABLE_WEBSERVER` : \[Default=OFF\] Disable building the webserver part of the `helics_broker_server` and `helics_broker`. The webserver requires boost 1.70 or higher and `HELICS_DISABLE_BOOST` will take precedence.
- `HELICS_DISABLE_ASIO` : \[Default=OFF\] Completely turn off inclusion of ASIO libraries. This will disable all TCP and UDP cores, disable real time mode for HELICS, and disable all timeout features for the Library so **use with caution**.
- `HELICS_ENABLE_SUBMODULE_UPDATE` : \[Default=ON\] Enable CMake to automatically download the submodules and update them if necessary
- `HELICS_ENABLE_ERROR_ON_WARNING` :\[Default=OFF\] Turns on Werror or equivalent, probably not useful for normal activity, There isn't many warnings but left in to allow the possibility
- `HELICS_ENABLE_EXTRA_COMPILER_WARNINGS` : \[Default=ON\] Turn on higher levels of warnings in the compilers, can be turned off if you didn't need or want the warning checks.
- `STATIC_STANDARD_LIB`: \[Default=""\] link the standard library as a static library for no additional C++ system dependencies (recognized values are `default`, `static`, and `dynamic`, anything else is treated the same as `default`)
- `HELICS_ENABLE_SWIG`: \[Default=OFF\] Conditional option if `HELICS_BUILD_MATLAB_INTERACE` or `HELICS_BUILD_JAVA_INTERACE` is selected and no other option that requires swig is used. This enables swig usage in cases where it would not otherwise be necessary.
- `HELICS_ENABLE_GIT_HOOKS`: install a git hook to check clang format before a push
- `Boost_NO_BOOST_CMAKE`: \[Default=OFF\] This is an option related to the Boost find module, but is occasionally needed if a specific version of boost is desired and there is a system copy of BoostConfig.cmake. So if an incorrect version of boost is being found even when `BOOST_ROOT` is being specified this option might need to be set to `ON`.
- `HELICS_BUILD_CONFIGURATION`: A string containing a specialized build configuration, if any. This is currently not in use, but was previously needed for building on a Raspberry PI system, by setting the option to "PI".
- `HELICS_DISABLE_C_SHARED_LIB`: \[Default=OFF\] Turns off building of the HELICS C shared library. May be used for building apps that only use the (modern) C++ shared library. Using the C++98 wrapper requires the C shared library.

#### ZeroMQ related Options

- `HELICS_USE_SYSTEM_ZEROMQ_ONLY`: \[Default=OFF\] Only find Zeromq through the system libraries, never attempt a local build.
- `HELICS_USE_ZMQ_STATIC_LIBRARY`: \[Default=OFF (unless only libzmq-static found)\] Build and link Zeromq using a static library. (NOTE: This has licensing implications if the resulting binary is distributed)
- `HELICS_ZMQ_SUBPROJECT`: \[Default=ON (MSVC) OFF(otherwise)\] Allow ZeroMQ to be built as a subproject if a system library is not found
- `HELICS_ZMQ_FORCE_SUBPROJECT`: \[Default=OFF\] Force ZMQ to be built and linked as a subproject.
- `ZeroMQ_INSTALL_PATH`: Can be used to specify a path to ZeroMQ for inclusion.

#### Options related to helics tests and CI configurations

- `HELICS_TEST_CODE_COVERAGE` :\[Default=OFF\] Turn on code coverage testing, enables additional linkage and options inside HELICS for coverage testing, mainly useful inside the CI or for testing.
- `HELICS_ENABLE_SUBPROJECT_TESTS`: \[Default=OFF\] Turn on some additional tests for using HELICS as a subproject, mainly used in some of the CI testing to make sure HELICS works as a subproject.
- `HELICS_ENABLE_CLANG_TOOLS`: \[Default=OFF\] Enables some helper targets for using clang-tidy and clang-format.

#### Options related to using external/vendored libraries

These options are for making HELICS look for an external installation or system copy of libraries instead of using the vendored copy included with HELICS. The search process for locating the external copy of libraries follows the typical [CMake Config Mode Search Procedure](https://cmake.org/cmake/help/latest/command/find_package.html#config-mode-search-procedure). Most users should leave these options `OFF`.

When enabling these options, if the static library variant is found it _must_ have been compiled using the option `CMAKE_POSITION_INDEPENDENT_CODE=ON` or _you will encounter linking errors_.

Furthermore, there are no guarantees that HELICS will compile using arbitrary versions of these libraries other than the exact version that is included as a vendored library in the HELICS repository. To get the current commit hashes for the libraries included in the HELICS repository, the command `git submodule status` can be run using a git clone of the HELICS repository with your desired branch checked out (_beware that if there is a + before the hash_ it means you _must_ run `git submodule update` and _re-run the status command_ to get the correct commit hash for the submodule the currently checked out branch). The GitHub website will also show the current commit hash for submodules when you use a browser to view the contents of the `ThirdParty` folder; for example, <https://github.com/GMLC-TDC/HELICS/tree/main/ThirdParty> will show the current commit hashes used for the various library submodules on the `main` HELICS branch.

- `HELICS_USE_EXTERNAL_FMT` : Use an external copy of the fmt library.
- `HELICS_USE_EXTERNAL_JSONCPP` : Use an external copy of the jsoncpp library.
- `HELICS_USE_EXTERNAL_SPDLOG` : Use an external copy of the spdlog library. Note that an external spdlog library may itself depend on an external fmt library, resulting in weird, confusing errors if it is incompabile with the internal fmt library used by HELICS.
- `HELICS_USE_EXTERNAL_UNITS` : Use an external copy of the LLNL units library.

## Hidden Options

There are a few options in the CMake system that are not visible in the GUI they mainly deal with particular situations related to release, testing, benchmarks, and code generation and should not be normally used. They are all default off unless otherwise noted.

- `HELICS_SWIG_GENERATE_INTERFACE_FILES_ONLY` : Use swig to generate the interface files for the different languages but don't compile them.
- `HELICS_OVERWRITE_INTERFACE_FILES` : Instruct CMake to take the generated files, and overwrite the existing interface files for the given language, only applies to Matlab and Java. This is used in the generation of the interface files for releases and the git repo. It is only active is `HELICS_SWIG_GENERATE_INTERFACE_FILES_ONLY` is enabled.
- `HELICS_DISABLE_SYSTEM_CALL_TESTS` : There are a few test that execute system calls, which could be problematic to compile or execute on certain platforms. This option removes those tests from compilation.
- `INSTALL_SYSTEM_LIBRARIES` : Install system libraries with the installation, mainly useful for making a complete installer package with all needed libraries included.
- `HELICS_INSTALL_PACKAGE_TESTS` : Set the find_package tests to only look for HELICS in the system install paths, and enable the package-config-tests
- `HELICS_DISABLE_GIT_OPERATIONS` : will turn off any of the helper tools that require git, this is useful in a couple cases for building packages and other situations where updates shouldn't be checked and no modifications should be made.
- `HELICS_SKIP_ZMQ_INSTALL`: This is only relevant if ZMQ is built as part of the compilation process, but it skips the installation of zmq as part of HELICS install in that case.
- `HELICS_BENCHMARK_SHIFT_FACTOR`: For running the benchmarks this shift factor can be used to scale the number of federates used for the benchmark tests. If used it is required to be a number and is power of 2 shift from nominal values. For example for a small system a shift factor of -1 or -2 might be appropriate for the benchmarks not to take too long. The default for systems with 4 or fewer cores is `-1` and 0 for larger compute systems. For small 2 core systems a value of -2 might be appropriate. For some very large systems a bigger value might be able to be used.
- `HELICS_HIDE_CMAKE_VARIABLES`: When using HELICS as a subproject in a CMake build this option can be enabled to hide all HELICS related variables in CMake so they won't show up in the CMake GUI.
- `WIN32_WINNT`: On some systems like msys or cygwin some libraries need to know the version of windows and it is not automatically detected so this variable can override the default which is set up for windows 10.
