# ------------------------------------------------------------------------------------------------ #
#                                This file is part of CosmoScout VR                                #
# ------------------------------------------------------------------------------------------------ #

# SPDX-FileCopyrightText: German Aerospace Center (DLR) <cosmoscout@dlr.de>
# SPDX-License-Identifier: MIT

option(CSP_ANCHOR_LABELS "Enable compilation of this plugin" ON)

if (NOT CSP_ANCHOR_LABELS)
  return()
endif()

# build plugin -------------------------------------------------------------------------------------

file(GLOB SOURCE_FILES src/*.cpp)

# Resoucre files and header files are only added in order to make them available in your IDE.
file(GLOB HEADER_FILES src/*.hpp)
file(GLOB_RECURSE RESOUCRE_FILES gui/*)

add_library(csp-anchor-labels SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${RESOUCRE_FILES}
)

target_link_libraries(csp-anchor-labels
  PUBLIC
    cs-core
)

# Add this Plugin to a "plugins" folder in your IDE.
set_property(TARGET csp-anchor-labels PROPERTY FOLDER "plugins")

# We mark all resource files as "header" in order to make sure that no one tries to compile them.
set_source_files_properties(${RESOUCRE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

# Make directory structure available in your IDE.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES 
  ${SOURCE_FILES} ${HEADER_FILES} ${RESOUCRE_FILES}
)

# install plugin -----------------------------------------------------------------------------------

install(TARGETS   csp-anchor-labels DESTINATION "share/plugins")
install(DIRECTORY "gui"             DESTINATION "share/resources")
