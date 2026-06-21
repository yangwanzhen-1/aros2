from setuptools import find_packages, setup

package_name = 'pkg10_py04_param'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='ubuntu',
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
            'demo00_param_py = pkg10_py04_param.demo00_param_py:main',
            'demo01_param_server_py = demo01_param_server_py.demo00_param_py:main',
        ],
    },
)
