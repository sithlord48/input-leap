# InputLeap -- mouse and keyboard sharing utility
# Copyright (C) 2012-2016 Symless Ltd.
# Copyright (C) 2012 Nick Bolton
#
# This package is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# found in the file LICENSE that should have accompanied this file.
#
# This package is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


set(sources input-leapd.cpp)
if (WIN32)
    list(APPEND sources input-leapd.rc)
    set_property(SOURCE input-leapd.rc PROPERTY COMPILE_FLAGS /nologo)
endif()

add_executable (input-leapd WIN32 ${sources})

target_link_libraries (input-leapd
    arch base common io ipc mt net platform synlib ${libs} ${OPENSSL_LIBS})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    install(TARGETS input-leapd DESTINATION ${INPUTLEAP_BUNDLE_BINARY_DIR})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install (TARGETS input-leapd DESTINATION bin)
else()
    install (TARGETS input-leapd DESTINATION .)
endif()
