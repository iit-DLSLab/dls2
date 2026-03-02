# test_logic.py

import numpy as np

class MyLogic:
    def __init__(self, gain: float, robot_name: str):
        print(f"[Python] MyLogic.__init__(gain={gain}, robot_name={robot_name})")

    def tick(self):
        print("[Python] MyLogic.tick() called")
        
    def tick_with_args(
        self, 
        grid: np.ndarray,    
        out: np.ndarray): # preallocated output array, e.g. shape (3,)

        out[0] = out[0] + 1.0
        out[1] = out[1] + 1.0
        out[2] = out[2] + 1.0
        
        print(f"[Python] MyLogic.tick_with_args called, grid shape: {grid}, out: {out}")