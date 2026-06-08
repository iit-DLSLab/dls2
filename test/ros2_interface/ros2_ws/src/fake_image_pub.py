#!/usr/bin/env python3

"""_summary_

xhost +SI:localuser:root
sudo apt-get update && sudo apt-get install pip && pip install pillow numpy --break-system-packages
"""
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy, HistoryPolicy
import time

class SimpleImagePub(Node):
    def __init__(self):
        super().__init__('fake_image_pub')

        qos = QoSProfile(
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.VOLATILE,
        )
        self.pub = self.create_publisher(Image, '/image', qos)


        self.width = 320
        self.height = 240
        self.timer = self.create_timer(0.1, self.tick)  # 10 Hz
        self.t0 = time.time()

    def tick(self):
        msg = Image()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "camera"

        msg.height = self.height
        msg.width = self.width
        msg.encoding = "mono8"          # 8-bit grayscale
        msg.is_bigendian = 0
        msg.step = self.width           # bytes per row for mono8

        # Simple moving gradient pattern
        shift = int((time.time() - self.t0) * 30) % 256
        data = bytearray(self.width * self.height)
        for y in range(self.height):
            for x in range(self.width):
                data[y*self.width + x] = (x + shift) & 0xFF

        msg.data = bytes(data)
        self.pub.publish(msg)
        print(f"Published image with shift {shift}")

def main():
    rclpy.init()
    node = SimpleImagePub()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()