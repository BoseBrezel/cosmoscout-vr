# ------------------------------------------------------------------------------------------------ #
#                                This file is part of CosmoScout VR                                #
# ------------------------------------------------------------------------------------------------ #

# SPDX-FileCopyrightText: German Aerospace Center (DLR) <cosmoscout@dlr.de>
# SPDX-License-Identifier: MIT

option(CSP_DEMO_NODE_EDITOR "Enable compilation of this plugin" ON)

if (NOT CSP_DEMO_NODE_EDITOR)
  return()
endif()

# build plugin -------------------------------------------------------------------------------------

file(GLOB SOURCE_FILES src/*.cpp src/nodes/*.cpp)
file(GLOB NODE_FILES src/nodes/*.js)

# Resoucre files and header files are only added in order to make them available in your IDE.
file(GLOB HEADER_FILES src/*.hpp)
file(GLOB_RECURSE CONTROL_FILES src/controls/*.js)

add_library(csp-demo-node-editor SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${NODE_FILES}
  ${CONTROL_FILES}
)

target_link_libraries(csp-demo-node-editor
  PUBLIC
    csl-node-editor
)

# Add this Plugin to a "plugins" folder in your IDE.
set_property(TARGET csp-demo-node-editor PROPERTY FOLDER "plugins")

# We mark all node source files as "header" in order to make sure that no one tries to compile them.
set_source_files_properties(${NODE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

# We mark all control source files as "header" in order to make sure that no one tries to compile them.
set_source_files_properties(${CONTROL_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

# Make directory structure available in your IDE.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES 
  ${SOURCE_FILES} ${HEADER_FILES} ${NODE_FILES} ${CONTROL_FILES}
)

# install plugin -----------------------------------------------------------------------------------

install(TARGETS csp-demo-node-editor  DESTINATION "share/plugins")
install(FILES   ${NODE_FILES}         DESTINATION "share/resources/nodes/csp-demo-node-editor")
install(FILES   ${CONTROL_FILES}      DESTINATION "share/resources/nodes/csp-demo-node-editor")

