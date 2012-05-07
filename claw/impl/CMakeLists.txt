cmake_minimum_required(VERSION 2.6)

project(claw)

file(GLOB CLAW_CODE "*.[ti]pp")
install( FILES ${CLAW_CODE} DESTINATION "${CLAW_INSTALLDIR_SOURCE}/impl" )
