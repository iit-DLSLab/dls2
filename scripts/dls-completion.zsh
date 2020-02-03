#compdef _dls dls

function _dls {
	local line

	_arguments \
		'--robot=-[Specify robot name]:robot name:->robot_name' \
		'-r-[Specify robot name]:robot name:->robot_name' \
		'--layers=-[Specify application layers]:layer name:->layers' \
		'-l-[Specify application layers]:layer name:->layers' \
		'--core[launch in core mode]' \
		'-c[launch in core mode]' \
		'--help[prints help and exits]' \
		'-h[prints help and exits]' \
		'--version[prints version and exits]' \
		'-v[prints version and exits]' \
		'-s[specify running in simulation]' \
		'--simulation[specify running in simulation]' \
		'-H[specify running on real hardware]' \
		'--hardware[specify running on real hardware]' \

	case "$state" in
		(robot_name)
			_robot_name
		;;

		(layers)
			_layers
		;;
	esac
}

function _robot_name {
	_alternative 'options:robot name:(hyq hyqreal)'
}

function _layers {
	_values -s , 'layers' "hardware" "console" "control" "log" "estimation"
}
