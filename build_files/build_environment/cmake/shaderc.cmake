# SPDX-FileCopyrightText: 2022 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

set(SHADERC_EXTRA_ARGS
  -DSHADERC_SKIP_TESTS=On
  -DSHADERC_SPIRV_TOOLS_DIR=${BUILD_DIR}/shaderc_spirv_tools/src/external_shaderc_spirv_tools
  -DSHADERC_SPIRV_HEADERS_DIR=${BUILD_DIR}/shaderc_spirv_headers/src/external_shaderc_spirv_headers
  -DSHADERC_GLSLANG_DIR=${BUILD_DIR}/shaderc_glslang/src/external_shaderc_glslang
  -DPython_EXECUTABLE=${PYTHON_BINARY}
  -DPython3_EXECUTABLE=${PYTHON_BINARY}
  -DSHADERC_ENABLE_SHARED_CRT=ON
)

ExternalProject_Add(external_shaderc
  URL file://${PACKAGE_DIR}/${SHADERC_FILE}
  URL_HASH ${SHADERC_HASH_TYPE}=${SHADERC_HASH}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  PREFIX ${BUILD_DIR}/shaderc
  CMAKE_GENERATOR ${PLATFORM_ALT_GENERATOR}

  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${LIBDIR}/shaderc
    ${DEFAULT_CMAKE_FLAGS}
    ${SHADERC_EXTRA_ARGS}

  INSTALL_DIR ${LIBDIR}/shaderc
)

add_dependencies(
  external_shaderc
  external_shaderc_spirv_tools
  external_shaderc_spirv_headers
  external_shaderc_glslang
  external_python
)


if(WIN32)
  if(BUILD_MODE STREQUAL Release)
    ExternalProject_Add_Step(external_shaderc after_install
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LIBDIR}/shaderc/include
        ${HARVEST_TARGET}/shaderc/include
      COMMAND ${CMAKE_COMMAND} -E copy
        ${LIBDIR}/shaderc/bin/shaderc_shared.dll
        ${HARVEST_TARGET}/shaderc/bin/shaderc_shared.dll
      COMMAND ${CMAKE_COMMAND} -E copy
        ${LIBDIR}/shaderc/lib/shaderc_shared.lib
        ${HARVEST_TARGET}/shaderc/lib/shaderc_shared.lib

      DEPENDEES install
    )
  endif()
else()
  harvest(external_shaderc shaderc shaderc "*")
endif()
