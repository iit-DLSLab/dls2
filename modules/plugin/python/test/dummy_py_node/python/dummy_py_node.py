#!/usr/bin/env python3

from __future__ import annotations

import math
import time
import numpy as np

from dls2_py.env_bootstrap import setup_environment

APP_ID = "dummy_py_node"
SIGNALS_DOMAIN = 3

PeriodicAppPlugin = setup_environment(
    package_name=APP_ID,
    module_file=__file__,
    required_message_modules=("BaseState","TrajectoryGenerator"),
)

import BaseState  # noqa: E402
import TrajectoryGenerator # noqa: E402

class DummyPyNodeApp(PeriodicAppPlugin):
    def __init__(self) -> None:
        super().__init__(APP_ID, SIGNALS_DOMAIN)

        self.position = np.zeros(3)
        self.orientation = np.zeros(4)
        self.linear_velocity = np.zeros(3)
        self.angular_velocity = np.zeros(3)
        self.first_message_base_arrived = False
        self.mode = 1
        self.enabled = False
        
        self.reader_base_state = self.build_input(
            "rt/base_state",
            BaseState.BaseStatePubSubType(),
            BaseState.BaseState(),
            auxiliary_callback=self._update_base_state,
            required_on_activation=False,
        )
        
        self.writer_trajectory_generator = self.build_output(
            "rt/dummy_trajectory_generator",
            TrajectoryGenerator.TrajectoryGeneratorPubSubType(),
            TrajectoryGenerator.TrajectoryGenerator(),
        )
        self.sequence_id = 0
        
        self.add_command(
            "set_mode",
            "set the operating mode to a number",
            self._set_mode,
            enabled=True,
        )
        self.add_command(
            "toggle_enabled",
            "toggle the internal enabled flag",
            self._toggle_enabled,
            enabled=True,
        )

    def check_activation(self) -> bool:
        return True

    def deactivation(self, timestamp_ns: int):
        del timestamp_ns
        return True
                
    def run(self, timestamp_ns: int) -> None:
        self.read()
        
        control_output = self.control_step(
            position=self.position,
            orientation=self.orientation,
            linear_velocity=self.linear_velocity,
            angular_velocity=self.angular_velocity)
        
        if control_output is None:
            return

        if control_output["shutdown_requested"]:
            raise SystemExit(0)
        
        msg = self.writer_trajectory_generator.data
        msg.timestamp(timestamp_ns) # or msg.timestamp(float(time.time_ns()))
        msg.sequence_id(int(self.sequence_id % 1000))
        self.sequence_id += 1
        
        msg.com_pose().position([float(x) for x in self.position])
        msg.com_pose().orientation([float(x) for x in self.orientation])
        msg.com_vel().linear([float(x) for x in self.linear_velocity])
        msg.com_vel().angular([float(x) for x in self.angular_velocity])
        
        msg.joints_position(_as_double_vector(control_output["desired_joint_positions"]))
        msg.joints_velocity(_as_double_vector(control_output["desired_joint_velocities"]))
        msg.kp(_as_double_vector(control_output["kp"]))
        msg.kd(_as_double_vector(control_output["kd"]))

        self.writer_trajectory_generator.write()

    def control_step(
        self,
        position,
        orientation,
        linear_velocity,
        angular_velocity):

        del position, orientation, linear_velocity, angular_velocity

        return {
            "shutdown_requested": False,
            "desired_joint_positions": np.zeros(12),
            "desired_joint_velocities": np.zeros(12),
            "kp": np.zeros(12),
            "kd": np.zeros(12),
        }
    
    def _update_base_state(self) -> None:
        msg = self.reader_base_state.getData()
        self.position = np.array(msg.pose().position(), copy=True)
        self.orientation = np.roll(np.array(msg.pose().orientation(), copy=True), 1)
        self.linear_velocity = np.array(msg.velocity().linear(), copy=True)
        self.angular_velocity = np.array(msg.velocity().angular(), copy=True)
        self.first_message_base_arrived = True
    
    def _set_mode(self, value: str) -> bool:
        try:
            mode = int(value.strip())
        except ValueError:
            print(f"discarding malformed mode: {value!r}")
            return False

        if not math.isfinite(mode):
            print(f"discarding malformed mode: {value!r}")
            return False

        self.mode = mode
        print("new mode:", self.mode)
        return True

    def _toggle_enabled(self) -> bool:
        self.enabled = not self.enabled
        print("enabled:", self.enabled)
        return True

def _as_double_vector(values: np.ndarray) -> object:
    vector = TrajectoryGenerator.double_vector()
    for value in values:
        vector.push_back(float(value))
    return vector
    
def main() -> None:
    app = DummyPyNodeApp()
    app.execute()

if __name__ == "__main__":
    main()
