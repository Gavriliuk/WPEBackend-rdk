# - Try to find libxkbcommon.
# Once done, this will define
#
#  LIBXKBCOMMON_FOUND - system has libxkbcommon.
#  LIBXKBCOMMON_INCLUDE_DIRS - the libxkbcommon include directories
#  LIBXKBCOMMON_LIBRARIES - link these to use libxkbcommon.
#
# Copyright (C) 2014 Igalia S.L.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND ITS CONTRIBUTORS ``AS
# IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

find_package(PkgConfig)
pkg_check_modules(PC_LIBXKBCOMMON xkbcommon)

find_path(LIBXKBCOMMON_INCLUDE_DIRS
    NAMES xkbcommon/xkbcommon.h
    HINTS ${PC_LIBXKBCOMMON_INCLUDEDIR} ${PC_LIBXKBCOMMON_INCLUDE_DIRS}
    )

find_library(LIBXKBCOMMON_LIBRARIES
    NAMES libxkbcommon
    HINTS ${PC_LIBXKBCOMMON_LIBDIR} ${PC_LIBXKBCOMMON_LIBRARY_DIRS}
    )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBXKBCOMMON REQUIRED_VARS LIBXKBCOMMON_INCLUDE_DIRS LIBXKBCOMMON_LIBRARIES)

mark_as_advanced(LIBXKBCOMMON_INCLUDE_DIRS LIBXKBCOMMON_LIBRARIES)
