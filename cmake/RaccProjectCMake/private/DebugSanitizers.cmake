function(_RACC_TARGET_ENABLE_ASAN target enable)
	target_compile_options(${target} PRIVATE "$<$<BOOL:${enable}>:-fsanitize=address>")
	target_link_options(${target} PRIVATE "$<$<BOOL:${enable}>:-fsanitize=address>")
endfunction()

function(_RACC_TARGET_ENABLE_UBSAN target enable)
	target_compile_options(${target} PRIVATE "$<$<BOOL:${enable}>:-fsanitize=undefined>")
	target_link_options(${target} PRIVATE "$<$<BOOL:${enable}>:-fsanitize=undefined>")
endfunction()
