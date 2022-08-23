cmake_minimum_required (VERSION 2.9)
project(mshconverter)

include_directories(
    ${HPL2_INCLUDE_DIR}
    ${DEP_INCLUDE_DIR}
)

link_directories(
    ${DEP_LIB_DIR}
)

AddTestTarget(mshconverter
    MshConverter.cpp
    MshConverter.h
)

# vim: et ts=4
