from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

import os
from ament_index_python import get_package_share_directory


"""
    需求： 在当前launch文件, 包含其他launch文件
"""

def generate_launch_description():

    include_launch = IncludeLaunchDescription(
        launch_description_source= PythonLaunchDescriptionSource(
            launch_file_path=os.path.join(
                get_package_share_directory("pkg16_cpp01_launch"),
                "launch/py",
                "py03_args.launch.py"
            )
        ),
        launch_arguments={
            "backg_r": "200",
            "backg_g": "100",
            "backg_b": "70",
        }.items()
    )

    return LaunchDescription([include_launch])