message(STATUS "## Reading dls_doc.cmake")

function(dls_markdown_to_man MARKDOWN_FILE MAN_FILE)
	execute_process(
		COMMAND
			pandoc ${MARKDOWN_FILE} -t man -o ${MARKDOWN_FILE}.man
	)
	set(${MAN_FILE} ${MARKDOWN_FILE}.man PARENT_SCOPE)
endfunction()
