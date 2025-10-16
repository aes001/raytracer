include(./private/DebugSanitizers.cmake)

function(RACC_DEFINE_EXECUTABLE_TARGET projectName)
	add_executable(${projectName} ${ARGN})
	set(RACC_${projectName}_ENABLE_ASAN false CACHE BOOL "Enable address sanitizer")
	set(RACC_${projectName}_ENABLE_UBSAN false CACHE BOOL "Enable undefined behaviour sanitizer")

	_RACC_TARGET_ENABLE_ASAN(${projectName} ${RACC_${projectName}_ENABLE_ASAN})
	_RACC_TARGET_ENABLE_UBSAN(${projectName} ${RACC_${projectName}_ENABLE_UBSAN})
endfunction()

function(RACC_DEFINE_LIBRARY_TARGET projectName)
	add_library(${projectName} ${ARGN})
	set(RACC_LIB_${projectName}_ENABLE_ASAN false CACHE BOOL "Enable address sanitizer")
	set(RACC_LIB_${projectName}_ENABLE_UBSAN false CACHE BOOL "Enable undefined behaviour sanitizer")

	_RACC_TARGET_ENABLE_ASAN(${projectName} ${RACC_LIB_${projectName}_ENABLE_ASAN})
	_RACC_TARGET_ENABLE_UBSAN(${projectName} ${RACC_LIB_${projectName}_ENABLE_UBSAN})
endfunction()
