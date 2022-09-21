#!/usr/bin/env bash

robot_specified=0
layers_specified=0
mode_specified=0
version_specified=0
help_specified=0
core_specified=0

robot_names=("hyq hyqreal aliengo")

# TODO this is a stub function that can be made more complete
_dls_completion()
{
	completions=()
	# if [[ ${COMP_WORDS[COMP_CWORD]} =~ "--" ]]; then
		if ((!robot_specified)); then
			completions+=("--robot")
		fi
		if ((!layers_specified)); then
			completions+=("--layers")
		fi
		if ((!mode_specified)); then
			completions+=("--hardware")
			completions+=("--simulation")
		fi
		if ((!core_specified)); then
			completions+=("--core")
		fi
		if ((!help_specified)); then
			completions+=("--help")
		fi
		if ((!version_specified)); then
			completions+=("--version")
		fi
		completions+=("--docs")
	# fi
	COMPREPLY=($(compgen -W "${completions[*]}" -- "${COMP_WORDS[COMP_CWORD]}"))
}

complete -F _dls_completion dls
