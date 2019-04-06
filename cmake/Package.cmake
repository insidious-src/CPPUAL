cmake_minimum_required(VERSION 3.3)
cmake_policy(SET CMP0026 OLD) # allow use of the LOCATION target property

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS TRUE)

include(CMakeParseArguments)
include(GetPrerequisites)
include(InstallRequiredSystemLibraries)

find_package(PackageHandleStandardArgs)

set(SYSTEM_LIBS_SKIP
    "KERNEL32.dll"
    "VERSION.dll"
    "ADVAPI32.dll"
    "msvcrt.dll"
    "MPR.DLL"
    "MPR.dll"
    "NETAPI32.dll"
    "ole32.dll"
    "SHELL32.dll"
    "USER32.dll"
    "USERENV.dll"
    "WINMM.DLL"
    "WINMM.dll"
    "WS2_32.dll"
    "GDI32.dll"
    "OPENGL32.DLL"
    "OPENGL32.dll"
    "CRYPT32.dll"
    "DNSAPI.dll"
    "IPHLPAPI.DLL"
    "IPHLPAPI.dll"
    "dwmapi.dll"
    "UxTheme.dll"
    )

#separate_arguments(SYSTEM_LIBS_SKIP)

function(install_file DLL DIRECTORY MODULE)
    message(STATUS "${DLL}")

    install(FILES ${DLL}
        DESTINATION ${DIRECTORY}
        COMPONENT ${MODULE}
        PERMISSIONS
            OWNER_READ
            GROUP_READ
            WORLD_READ
            OWNER_EXECUTE
            GROUP_EXECUTE
            WORLD_EXECUTE
        )
endfunction()

function(install_shared DLL DIRECTORY MODULE)
    # find the release *.dll file
    get_target_property(SRC_LIB_LOCATION ${DLL} LOCATION)
    message(STATUS "${SRC_LIB_LOCATION}")

    install(FILES ${SRC_LIB_LOCATION}
        DESTINATION ${DIRECTORY}
        COMPONENT ${MODULE}
        PERMISSIONS
            OWNER_READ
            GROUP_READ
            WORLD_READ
            OWNER_EXECUTE
            GROUP_EXECUTE
            WORLD_EXECUTE
        )
endfunction()

function(install_target TARGET DIRECTORY MODULE)
    message(STATUS "${TARGET}")

    install(TARGETS ${TARGET}
        RUNTIME
        DESTINATION ${DIRECTORY}
        COMPONENT ${MODULE}
        PERMISSIONS
            OWNER_WRITE
            OWNER_READ
            GROUP_WRITE
            GROUP_READ
            WORLD_WRITE
            WORLD_READ
            OWNER_EXECUTE
            GROUP_EXECUTE
            WORLD_EXECUTE
        )
endfunction()

function(install_package
        PACKAGE_NAME
        COMPANY_NAME
        MAJOR_VERSION
        MINOR_VERSION
        PATCH_VERSION
        )

    # parse the macro arguments
    cmake_parse_arguments(ARG "" "" "TARGETS;DEPENDS" ${ARGN})

    if(ARG_TARGETS)
    else()
        message(FATAL "Package TARGETS not specified!")
    endif()

    string(REPLACE " " "_" PACKAGE_NAME_TRIMMED "${PACKAGE_NAME}")
    string(TOLOWER "${PACKAGE_NAME_TRIMMED}" PACKAGE_NAME_LOWER)
    string(TOLOWER "${COMPANY_NAME}" COMPANY_NAME_LOWER)

    # set the list of dependant libraries
    if(ARG_DEPENDS)
        set(DEPENDENCIES "")

        foreach(LIB ${ARG_DEPENDS})
            if(TARGET ${LIB} AND NOT ANDROID)
                # item is a CMake target, extract the library path
                if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                    get_property(LIB_PATH TARGET ${LIB} PROPERTY DEBUG_LOCATION)
                else()
                    get_property(LIB_PATH TARGET ${LIB} PROPERTY LOCATION)
                endif()

                # extract dependencies ignoring the system ones
                get_prerequisites(${LIB_PATH} PREREQUISITES 0 1 "" "")

                foreach(PREREQ_LIB ${PREREQUISITES})
                    get_filename_component(REALPATH_LIB "${PREREQ_LIB}" REALPATH)
                    list(APPEND DEPENDENCIES ${REALPATH_LIB})
                endforeach()

                list(APPEND DEPENDENCIES ${PREREQUISITES})

            endif()

            list(APPEND DEPENDENCIES "${LIB}")
        endforeach()

        # remove duplicates
        list(REMOVE_DUPLICATES DEPENDENCIES)

        # remove shared targets
        foreach(LIB ${ARG_DEPENDS})
            # unix format
            list(REMOVE_ITEM DEPENDENCIES "${CMAKE_SHARED_LIBRARY_PREFIX}${LIB}${CMAKE_SHARED_LIBRARY_SUFFIX}")
            # windows format on linux
            list(REMOVE_ITEM DEPENDENCIES "${LIB}${CMAKE_SHARED_LIBRARY_SUFFIX}")
            # windows format on linux & unix format
            list(REMOVE_ITEM DEPENDENCIES
                "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        endforeach()

        # remove system libraries
        foreach(LIB_SKIP ${SYSTEM_LIBS_SKIP})
            list(REMOVE_ITEM DEPENDENCIES "${LIB_SKIP}")
        endforeach()
    endif()

    if(ANDROID)

        include(AddQtAndroidApk REQUIRED)

        if(ARG_DEPENDS)
            set(ANDROID_DEPENDS DEPENDS ${ARG_DEPENDS})
        endif()

        add_qt_android_apk("${PACKAGE_NAME_LOWER}" "${ARG_TARGETS}"
            NAME "${COMPANY_NAME} ${PACKAGE_NAME}"
            VERSION_CODE "${MAJOR_VERSION}${MINOR_VERSION}${PATCH_VERSION}"
            PACKAGE_NAME "org.${COMPANY_NAME_LOWER}.${PACKAGE_NAME_LOWER}"
            BUILDTOOLS_REVISION "${ANDROID_BUILDTOOLS_REVISION}"
            #KEYSTORE ${CMAKE_CURRENT_LIST_DIR}/mykey.keystore myalias
            #KEYSTORE_PASSWORD xxxx
            ${ANDROID_DEPENDS}
            INSTALL
            )

    elseif(IOS_PLATFORM)

        set_target_properties(${TARGET} PROPERTIES
            MACOSX_BUNDLE TRUE
            #XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "6GTJ292YX4"
            MACOSX_BUNDLE_GUI_IDENTIFIER "org.${COMPANY_NAME_LOWER}.${PACKAGE_NAME_LOWER}"
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "org.${COMPANY_NAME_LOWER}.${PACKAGE_NAME_LOWER}"
            #MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ios-info.plist.in"
            )

        message(STATUS "Preparing package ${PACKAGE_NAME_LOWER}.app...")

#        set(CPACK_GENERATOR "Bundle")

#        #The names of the Developer certs you got from Apple iTunesConnect
#        set(CPACK_BUNDLE_NAME "${PACKAGE_NAME_LOWER}")
#        set(CPACK_APPLE_BUNDLE_ID "org.${COMPANY_NAME_LOWER}.${PACKAGE_NAME_LOWER}")
#        set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ios-info.plist.in")
#        set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_LIST_DIR}/resources/cfg.png")
#        set(CPACK_APPLE_CERT_APP "Developer ID Application: [YOUR NAME]")
#        set(CPACK_APPLE_CERT_INSTALLER "Developer ID Installer: [YOUR NAME]")

#        include(CPack)

#        add_custom_target(
#            ${PACKAGE_NAME_LOWER}
#            ALL
#            DEPENDS ${ARG_TARGETS} ${INSTALL_PACKAGES}
#            COMMAND ${CMAKE_CPACK_COMMAND} -DCPACK_COMPONENTS_ALL="${PACKAGE_NAME_LOWER}" -DCPACK_PACKAGE_FILE_NAME="${PACKAGE_NAME_LOWER}-${CPACK_PACKAGE_VERSION}-$<CONFIGURATION>"
#            COMMENT "Building package ${PACKAGE_NAME_LOWER}..."
#            )
    else()
        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
            set(WIN_DEFINED 1)
        else()
            set(WIN_DEFINED 0)
        endif()

        message(STATUS "Files to install:")

        if(DEPENDENCIES)
            foreach(LIB ${DEPENDENCIES})
                if(TARGET ${LIB})

                    install_shared(${LIB} . ${PACKAGE_NAME_LOWER})
                else()
                    if(${LIB} MATCHES "Qt5Gui${CMAKE_SHARED_LIBRARY_SUFFIX}")
                        if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
                            install_shared(Qt5::QWindowsIntegrationPlugin platforms ${PACKAGE_NAME_LOWER})
                        elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
                            install_shared(Qt5::QXcbIntegrationPlugin platforms ${PACKAGE_NAME_LOWER})
                            #install_shared(Qt5::QWaylandIntegrationPlugin platforms ${PACKAGE_NAME_LOWER})
                        else()
                            install_shared(Qt5::QXcbIntegrationPlugin platforms ${PACKAGE_NAME_LOWER})
                        endif()
                    endif()

                    string(FIND ${LIB} "/" SUB_STR_POS REVERSE)

                    if(NOT ${SUB_STR_POS} MATCHES -1)
                        string(SUBSTRING ${LIB} ${SUB_STR_POS} -1 LIB)
                    endif()

                    find_library(PATH_${LIB}
                        NAMES ${LIB}
                        $<WIN_DEFINED:HINTS "/usr/${TOOLCHAIN_PREFIX}/bin" >
                        $<WIN_DEFINED:NO_DEFAULT_PATH >
                        )

                    install_file(${PATH_${LIB}} . ${PACKAGE_NAME_LOWER})
                endif()
            endforeach()
        endif()

        foreach(TARGET ${ARG_TARGETS})
            install_target(${TARGET} . ${PACKAGE_NAME_LOWER})
        endforeach()

        if(WIN32 OR WIN64)
            set(CPACK_GENERATOR NSIS)
        else()
            set(CPACK_GENERATOR TGZ)
        endif()

        set(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
        set(CPACK_PACKAGE_VENDOR "${COMPANY_NAME}")
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PACKAGE_NAME} Installation.")
        set(CPACK_PACKAGE_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
        set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
        set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
        set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
        set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_LIST_DIR}/resources/solventer_logo.bmp")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/LICENSE.md")
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "${COMPANY_NAME} ${PACKAGE_NAME}")
        set(CPACK_NSIS_MODIFY_PATH ON)

        include(CPack)

        add_custom_target(
            ${PACKAGE_NAME_LOWER}
            ALL
            DEPENDS ${ARG_TARGETS} ${INSTALL_PACKAGES}
            COMMAND ${CMAKE_CPACK_COMMAND} -DCPACK_COMPONENTS_ALL="${PACKAGE_NAME_LOWER}" -DCPACK_PACKAGE_FILE_NAME="${PACKAGE_NAME_LOWER}-${CPACK_PACKAGE_VERSION}-$<CONFIGURATION>"
            COMMENT "Building package ${PACKAGE_NAME_LOWER}..."
            )

        set(INSTALL_PACKAGES ${PACKAGE_NAME_LOWER} ${INSTALL_PACKAGES} PARENT_SCOPE)
    endif()
endfunction()
