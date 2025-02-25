macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}
                 ${XCODE_VALUE})
endmacro (set_xcode_property)

message(STATUS "App: ${APP_NAME}...")

option(USE_METAL "USE METAL" ON)

# toggle Metal/MoltenVK
# set(USE_METAL OFF)

# ---------------------------------------------------------------------

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SOURCE_CPP_FILES *.cpp *.cc)
file(GLOB_RECURSE SOURCE_C_FILES *.c)
file(GLOB_RECURSE SOURCE_MM_FILES *.m *.mm)

list(APPEND APP_UI_RES
    ${CMAKE_CURRENT_LIST_DIR}/Base.lproj/Main.storyboard
    ${CMAKE_CURRENT_LIST_DIR}/Assets.xcassets/Contents.json
)

set(ALL_FILES
    ${HEADER_FILES}
    ${SOURCE_CPP_FILES}
    ${SOURCE_C_FILES}
    ${SOURCE_MM_FILES}
    ${GFX_TESTCASE_HEADER}
    ${GFX_TESTCASE_SOURCE}
    ${APP_UI_RES}
    ${cc_common_res}
    )

if (USE_METAL)
    add_definitions("-DCC_USE_METAL")
    set(GFX_LIB GFXMetal)
else ()
    add_definitions("-DCC_USE_VULKAN")
    set(GFX_LIB GFXVulkan)
    set(LIB_MOLTEN_VK "${GFX_EXTERNAL_PATH}/MoltenVK/mac/release/libMoltenVK.dylib")
    list(APPEND ALL_FILES ${LIB_MOLTEN_VK})
endif()

add_executable(${APP_NAME} MACOSX_BUNDLE ${ALL_FILES})

set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/Info.plist"
)
set_xcode_property(${APP_NAME} PRODUCT_BUNDLE_IDENTIFIER ${APP_NAME})

# Locate system libraries on macOS
find_library(APPKIT_LIBRARY AppKit)
find_library(METAL_LIBRARY Metal)
find_library(METALKIT_LIBRARY MetalKit)
find_library(QUARTZCORE_LIBRARY QuartzCore)
find_library(MPS_LIBRARY MetalPerformanceShaders)
list(APPEND PLATFORM_SPECIFIC_LIBS
    ${APPKIT_LIBRARY}
    ${METAL_LIBRARY}
    ${METALKIT_LIBRARY}
    ${QUARTZCORE_LIBRARY}
    ${MPS_LIBRARY}
)

if (NOT USE_METAL)
    set_source_files_properties(${LIB_MOLTEN_VK} PROPERTIES MACOSX_PACKAGE_LOCATION Frameworks)
    set_source_files_properties(${LIB_MOLTEN_VK} PROPERTIES XCODE_FILE_ATTRIBUTES "CodeSignOnCopy")
    set_target_properties(${APP_NAME} PROPERTIES FRAMEWORK true)
    set_xcode_property(${APP_NAME} LD_RUNPATH_SEARCH_PATHS @executable_path/../Frameworks)
endif()

target_link_libraries(${APP_NAME}
    ${PLATFORM_SPECIFIC_LIBS}
    ${ENGINE_BASE_LIBS}
    ${GFX_LIB}
    ${CC_EXTERNAL_LIBS}
    z
)

target_include_directories(${APP_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${COCOS_SRC_PATH}/platform
    ${COCOS_EXTERNAL_PATH}/sources
    ${CC_EXTERNAL_INCLUDES}
    ${CC_EXTERNAL_INCLUDES}/v8
)

if(MACOSX)
    target_include_directories(${APP_NAME} PRIVATE
        ##FIXME: bad dependency
        ${COCOS_SRC_PATH}/platform/ios
    )
endif()

source_group(TREE ${COCOS_SRC_PATH}/base PREFIX "Header Files/base" FILES ${CC_BASE_HEADERS})
source_group(TREE ${COCOS_SRC_PATH}/base PREFIX "Source Files/base" FILES ${CC_BASE_SOURCES})
source_group(TREE ${COCOS_SRC_PATH}/math PREFIX "Header Files/math" FILES ${CC_MATH_HEADERS})
source_group(TREE ${COCOS_SRC_PATH}/math PREFIX "Source Files/math" FILES ${CC_MATH_SOURCES})

# setup default flags
target_compile_definitions(${APP_NAME} PUBLIC
    $<$<NOT:$<BOOL:${USE_VIDEO}>>:USE_VIDEO=0>
    $<$<NOT:$<BOOL:${USE_WEB_VIEW}>>:USE_WEB_VIEW=0>
    $<$<NOT:$<BOOL:${USE_AUDIO}>>:USE_AUDIO=0>
    $<$<NOT:$<BOOL:${USE_SOCKET}>>:USE_SOCKET=0>
    $<$<NOT:$<BOOL:${USE_MIDDLEWARE}>>:USE_MIDDLEWARE=0>
    $<$<NOT:$<BOOL:${USE_SPINE}>>:USE_SPINE=0>
    $<$<NOT:$<BOOL:${USE_DRAGONBONES}>>:USE_DRAGONBONES=0>
    $<$<NOT:$<BOOL:${USE_TIFF}>>:CC_USE_TIFF=0>
    $<$<BOOL:${USE_GFX_RENDERER}>:USE_GFX_RENDERER=1>
    $<$<NOT:$<BOOL:${USE_V8_DEBUGGER}>>:USE_V8_DEBUGGER=0>
)

file(GLOB PVRDLLS ${PVRSDK_LIB_DIR}/*.dylib)
foreach(PVRDLL ${PVRDLLS})
  add_custom_command(TARGET ${APP_NAME} PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${APP_NAME}>/../Frameworks
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${PVRDLL} $<TARGET_FILE_DIR:${APP_NAME}>/../Frameworks/)
endforeach()

message(STATUS "${APP_NAME} configuration completed.")
