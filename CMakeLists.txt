cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(SPLProject CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























set(PROJECT_NAME SPLProject)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "bytecode.md"
    "clc.bat"
    "clc.py"
    "README.md"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "ConsoleSubOp.h"
    "ErrorStream.h"
    "Float.h"
    "Int.h"
    "Keyword.h"
    "resource.h"
    "RNG.h"
    "rom.h"
    "SPLErrors.h"
    "Stack.h"
    "String.h"
    "Vector2.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__SPL__Compiler__Assembling
    "SPL/Compiler/Assembling/Assembler.h"
)
source_group("Header Files\\SPL\\Compiler\\Assembling" FILES ${Header_Files__SPL__Compiler__Assembling})

set(Header_Files__SPL__Compiler__Parser
    "SPL/Compiler/Parsing/Parser.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser" FILES ${Header_Files__SPL__Compiler__Parser})

set(Header_Files__SPL__Compiler__Parser__Nodes
    "SPL/Compiler/Parsing/Nodes/Block.h"
    "SPL/Compiler/Parsing/Nodes/Console.h"
    "SPL/Compiler/Parsing/Nodes/Constant.h"
    "SPL/Compiler/Parsing/Nodes/Input.h"
    "SPL/Compiler/Parsing/Nodes/IsFloat.h"
    "SPL/Compiler/Parsing/Nodes/IsInt.h"
    "SPL/Compiler/Parsing/Nodes/Let.h"
    "SPL/Compiler/Parsing/Nodes/Node.h"
    "SPL/Compiler/Parsing/Nodes/Raise.h"
    "SPL/Compiler/Parsing/Nodes/RandomNode.h"
    "SPL/Compiler/Parsing/Nodes/Single.h"
    "SPL/Compiler/Parsing/Nodes/Sleep.h"
    "SPL/Compiler/Parsing/Nodes/Value.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser\\Nodes" FILES ${Header_Files__SPL__Compiler__Parser__Nodes})

set(Header_Files__SPL__Compiler__Parser__Nodes__Casting
    "SPL/Compiler/Parsing/Nodes/Casting/ToFloat.h"
    "SPL/Compiler/Parsing/Nodes/Casting/ToInt.h"
    "SPL/Compiler/Parsing/Nodes/Casting/ToString.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser\\Nodes\\Casting" FILES ${Header_Files__SPL__Compiler__Parser__Nodes__Casting})

set(Header_Files__SPL__Compiler__Parser__Nodes__Comparisons
    "SPL/Compiler/Parsing/Nodes/Comparisons/Comparison.h"
    "SPL/Compiler/Parsing/Nodes/Comparisons/Equ.h"
    "SPL/Compiler/Parsing/Nodes/Comparisons/Grt.h"
    "SPL/Compiler/Parsing/Nodes/Comparisons/GrtEqu.h"
    "SPL/Compiler/Parsing/Nodes/Comparisons/Lwr.h"
    "SPL/Compiler/Parsing/Nodes/Comparisons/LwrEqu.h"
    "SPL/Compiler/Parsing/Nodes/Comparisons/Neq.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser\\Nodes\\Comparisons" FILES ${Header_Files__SPL__Compiler__Parser__Nodes__Comparisons})

set(Header_Files__SPL__Compiler__Parser__Nodes__ControlFlow
    "SPL/Compiler/Parsing/Nodes/ControlFlow/Call.h"
    "SPL/Compiler/Parsing/Nodes/ControlFlow/Exit.h"
    "SPL/Compiler/Parsing/Nodes/ControlFlow/Goto.h"
    "SPL/Compiler/Parsing/Nodes/ControlFlow/Ret.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser\\Nodes\\ControlFlow" FILES ${Header_Files__SPL__Compiler__Parser__Nodes__ControlFlow})

set(Header_Files__SPL__Compiler__Parser__Nodes__Maths
    "SPL/Compiler/Parsing/Nodes/Maths/Add.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Concat.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Dec.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Div.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Inc.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Mod.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Mul.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Pow.h"
    "SPL/Compiler/Parsing/Nodes/Maths/Sub.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser\\Nodes\\Maths" FILES ${Header_Files__SPL__Compiler__Parser__Nodes__Maths})

set(Header_Files__SPL__Compiler__Parser__Nodes__Output
    "SPL/Compiler/Parsing/Nodes/Free.h"
    "SPL/Compiler/Parsing/Nodes/Output/Print.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser\\Nodes\\Output" FILES ${Header_Files__SPL__Compiler__Parser__Nodes__Output})

set(Header_Files__SPL__Compiler__Parser__Nodes__Stack
    "SPL/Compiler/Parsing/Nodes/Stack/Pop.h"
    "SPL/Compiler/Parsing/Nodes/Stack/Push.h"
    "SPL/Compiler/Parsing/Nodes/Stack/SetPop.h"
)
source_group("Header Files\\SPL\\Compiler\\Parser\\Nodes\\Stack" FILES ${Header_Files__SPL__Compiler__Parser__Nodes__Stack})

set(Header_Files__SPL__Compiler__PreProcessing
    "SPL/Compiler/PreProcessing/PreProcessor.h"
)
source_group("Header Files\\SPL\\Compiler\\PreProcessing" FILES ${Header_Files__SPL__Compiler__PreProcessing})

set(Header_Files__SPL__Compiler__Tokenisation
    "SPL/Compiler/Tokenisation/Lexer.h"
    "SPL/Compiler/Tokenisation/Token.h"
    "SPL/Compiler/Tokenisation/TokenType.h"
)
source_group("Header Files\\SPL\\Compiler\\Tokenisation" FILES ${Header_Files__SPL__Compiler__Tokenisation})

set(Header_Files__SPL__Disassembling
    "SPL/Disassembling/Disassembler.h"
)
source_group("Header Files\\SPL\\Disassembling" FILES ${Header_Files__SPL__Disassembling})

set(Header_Files__SPL__VirtualMachine
    "SPL/VirtualMachine/Accumulator.h"
    "SPL/VirtualMachine/Processor.h"
    "SPL/VirtualMachine/ProcessorStack.h"
    "SPL/VirtualMachine/VariableData.h"
)
source_group("Header Files\\SPL\\VirtualMachine" FILES ${Header_Files__SPL__VirtualMachine})

set(Resource_Files
    "test.spl"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
    "main.cpp"
    "Vector2.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__SPL__Compiler__Assembler
    "SPL/Compiler/Assembling/Assembler.cpp"
)
source_group("Source Files\\SPL\\Compiler\\Assembler" FILES ${Source_Files__SPL__Compiler__Assembler})

set(Source_Files__SPL__Compiler__Parser
    "SPL/Compiler/Parsing/Parser.cpp"
)
source_group("Source Files\\SPL\\Compiler\\Parser" FILES ${Source_Files__SPL__Compiler__Parser})

set(Source_Files__SPL__Compiler__Parser__Nodes
    "SPL/Compiler/Parsing/Nodes/Block.cpp"
    "SPL/Compiler/Parsing/Nodes/Free.cpp"
    "SPL/Compiler/Parsing/Nodes/Let.cpp"
    "SPL/Compiler/Parsing/Nodes/Node.cpp"
    "SPL/Compiler/Parsing/Nodes/Single.cpp"
    "SPL/Compiler/Parsing/Nodes/Value.cpp"
)
source_group("Source Files\\SPL\\Compiler\\Parser\\Nodes" FILES ${Source_Files__SPL__Compiler__Parser__Nodes})

set(Source_Files__SPL__Compiler__Parser__Nodes__ControlFlow
    "SPL/Compiler/Parsing/Nodes/ControlFlow/Call.cpp"
    "SPL/Compiler/Parsing/Nodes/ControlFlow/Exit.cpp"
    "SPL/Compiler/Parsing/Nodes/ControlFlow/Goto.cpp"
)
source_group("Source Files\\SPL\\Compiler\\Parser\\Nodes\\ControlFlow" FILES ${Source_Files__SPL__Compiler__Parser__Nodes__ControlFlow})

set(Source_Files__SPL__Compiler__Parser__Nodes__Output
    "SPL/Compiler/Parsing/Nodes/Output/Print.cpp"
)
source_group("Source Files\\SPL\\Compiler\\Parser\\Nodes\\Output" FILES ${Source_Files__SPL__Compiler__Parser__Nodes__Output})

set(Source_Files__SPL__Compiler__Parser__Nodes__Stack
    "SPL/Compiler/Parsing/Nodes/Stack/Push.cpp"
    "SPL/Compiler/Parsing/Nodes/Stack/SetPop.cpp"
)
source_group("Source Files\\SPL\\Compiler\\Parser\\Nodes\\Stack" FILES ${Source_Files__SPL__Compiler__Parser__Nodes__Stack})

set(Source_Files__SPL__Compiler__PreProcessing
    "SPL/Compiler/PreProcessing/PreProcessor.cpp"
)
source_group("Source Files\\SPL\\Compiler\\PreProcessing" FILES ${Source_Files__SPL__Compiler__PreProcessing})

set(Source_Files__SPL__Compiler__Tokenisation
    "SPL/Compiler/Tokenisation/Lexer.cpp"
    "SPL/Compiler/Tokenisation/Token.cpp"
)
source_group("Source Files\\SPL\\Compiler\\Tokenisation" FILES ${Source_Files__SPL__Compiler__Tokenisation})

set(Source_Files__SPL__Disassembling
    "SPL/Disassembling/Disassembler.cpp"
)
source_group("Source Files\\SPL\\Disassembling" FILES ${Source_Files__SPL__Disassembling})

set(Source_Files__SPL__VirtualMachine
    "SPL/VirtualMachine/Accumulator.cpp"
    "SPL/VirtualMachine/Processor.cpp"
    "SPL/VirtualMachine/VariableData.cpp"
)
source_group("Source Files\\SPL\\VirtualMachine" FILES ${Source_Files__SPL__VirtualMachine})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Header_Files__SPL__Compiler__Assembling}
    ${Header_Files__SPL__Compiler__Parser}
    ${Header_Files__SPL__Compiler__Parser__Nodes}
    ${Header_Files__SPL__Compiler__Parser__Nodes__Casting}
    ${Header_Files__SPL__Compiler__Parser__Nodes__Comparisons}
    ${Header_Files__SPL__Compiler__Parser__Nodes__ControlFlow}
    ${Header_Files__SPL__Compiler__Parser__Nodes__Maths}
    ${Header_Files__SPL__Compiler__Parser__Nodes__Output}
    ${Header_Files__SPL__Compiler__Parser__Nodes__Stack}
    ${Header_Files__SPL__Compiler__PreProcessing}
    ${Header_Files__SPL__Compiler__Tokenisation}
    ${Header_Files__SPL__Disassembling}
    ${Header_Files__SPL__VirtualMachine}
    ${Resource_Files}
    ${Source_Files}
    ${Source_Files__SPL__Compiler__Assembler}
    ${Source_Files__SPL__Compiler__Parser}
    ${Source_Files__SPL__Compiler__Parser__Nodes}
    ${Source_Files__SPL__Compiler__Parser__Nodes__ControlFlow}
    ${Source_Files__SPL__Compiler__Parser__Nodes__Output}
    ${Source_Files__SPL__Compiler__Parser__Nodes__Stack}
    ${Source_Files__SPL__Compiler__PreProcessing}
    ${Source_Files__SPL__Compiler__Tokenisation}
    ${Source_Files__SPL__Disassembling}
    ${Source_Files__SPL__VirtualMachine}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE SPLProject)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "SPL"
        TARGET_NAME_RELEASE "SPL"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "SPL"
        TARGET_NAME_RELEASE "SPL"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "SPL"
        TARGET_NAME_RELEASE "SPL"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Any CPU")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

