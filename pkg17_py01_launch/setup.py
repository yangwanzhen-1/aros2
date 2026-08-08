from setuptools import find_packages, setup
from glob import glob

package_name = 'pkg17_py01_launch'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, glob("launch/py/*.launch.py")),
        ('share/' + package_name, glob("launch/xml/*.launch.xml")),
        ('share/' + package_name, glob("launch/yaml/*.launch.yaml"))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='root',
    maintainer_email='2729845660@qq.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
        ],
    },
)
