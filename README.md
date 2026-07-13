# ros学习

## 1.项目结构

```bash
# 📦📨🧩✨🧱📝📄🚀
├── base_interfaces_demo	# 📦自定义消息文件
│   ├── action
│   │   └── Progress.action # action通信自定义消息
│   ├── CMakeLists.txt
│   ├── msg
│   │   └── Student.msg		# 话题通信自定义接口消息
│   ├── package.xml
│   └── srv
│       └── AddInts.srv		# 服务通信接口消息
```

```bash
pkg04_cpp01_topic			# 📦话题通信
├── CMakeLists.txt
├── include
│   └── pkg04_cpp01_topic
├── package.xml
└── src
    ├── demo01_talker_str.cpp		# 话题通信发布放，C++
    ├── demo02_listener_str.cpp		# 话题通信订阅放， C++
    ├── demo03_talker_stu.cpp		# 自定义，话题通信发布放，C++
    └── demo04_listener_stu.cpp		# 自定义，话题通信发布放，C++

├── pkg04_py01_topic        # 📦话题通信 py
│   ├── package.xml
│   ├── pkg04_py01_topic
│   │   ├── demo01_talker_str_py.py     # 话题通信发布放，py
│   │   ├── demo02_listener_str_py.py   # 话题通信订阅放，py
│   │   ├── demo03_talker_stu_py.py     # 自定义，话题通信发布放，py
│   │   ├── demo04_listener_stu_py.py   # 自定义，话题通信发布放，py
│   │   └── __init__.py
│   ├── resource
│   │   └── pkg04_py01_topic
│   ├── setup.cfg
│   ├── setup.py
│   └── test
│       ├── test_copyright.py
│       ├── test_flake8.py
│       └── test_pep257.py



├── pkg05_cpp02_service     # 📦服务通信 C++
│   ├── CMakeLists.txt
│   ├── include
│   │   └── pkg05_cpp02_service
│   ├── package.xml
│   └── src
│       ├── demo01_server.cpp       # 服务通信服务端
│       └── demo02_client.cpp       # 服务通信客户端


├── pkg06_py02_service      # 📦服务通信 py
│   ├── package.xml
│   ├── pkg06_py02_service
│   │   ├── demo01_server_py.py     # 服务通信服务端 py
│   │   ├── demo02_client_py.py     # 服务通信客户端 py
│   │   └── __init__.py
│   ├── resource
│   │   └── pkg06_py02_service
│   ├── setup.cfg
│   ├── setup.py
│   └── test
│       ├── test_copyright.py
│       ├── test_flake8.py
│       └── test_pep257.py


├── pkg07_cpp03_action      # 📦动作通信
│   ├── CMakeLists.txt
│   ├── include
│   │   └── pkg07_cpp03_action
│   ├── package.xml
│   └── src
│       ├── demo01_action_server.cpp    # 动作通信服务端实现
│       └── demo02_action_client.cpp    # 动作通信客户端实现

├── pkg08_py03_action       # 📦动作通信 py
│   ├── package.xml
│   ├── pkg08_py03_action
│   │   ├── demo01_action_server_py.py  # 动作通信服务端实现 py
│   │   ├── demo02_action_client_py.py  # 动作通信客户端实现 py
│   │   └── __init__.py
│   ├── resource
│   │   └── pkg08_py03_action
│   ├── setup.cfg
│   ├── setup.py
│   └── test
│       ├── test_copyright.py
│       ├── test_flake8.py
│       └── test_pep257.py

├── pkg09_cpp04_param      # 📦参数服务  
│   ├── CMakeLists.txt
│   ├── include
│   │   └── pkg09_cpp04_param
│   ├── package.xml
│   └── src
│       ├── demo00_param.cpp            # 参数服务案例
│       ├── demo01_param_server.cpp     # 参数服务，服务端
│       └── demo02_param_client.cpp     # 参数服务，客户端

├── pkg10_py04_param        # 📦参数服务 py
│   ├── package.xml
│   ├── pkg10_py04_param
│   │   ├── demo00_param_py.py          # 参数服务案例
│   │   ├── demo01_param_server_py.py   # 参数服务，服务端
│   │   └── __init__.py
│   ├── resource
│   │   └── pkg10_py04_param
│   ├── setup.cfg
│   ├── setup.py
│   └── test
│       ├── test_copyright.py
│       ├── test_flake8.py
│       └── test_pep257.py

├── pkg11_cpp05_names       # 📦节点重命名, 话题重命名
│   ├── CMakeLists.txt
│   ├── include
│   │   └── pkg11_cpp05_names
│   ├── launch
│   │   ├── demo01_names_launch.py      # 节点重命名 launch 文件
│   │   ├── demo02_names_launch.xml     # 节点重命名 launch 文件
│   │   └── demo03_names_launch.yaml    # 节点重命名 launch 文件
│   ├── package.xml
│   └── src
│       └── demo01_names.cpp

├── pkg12_py05_names        # 📦节点重命名 py
│   ├── package.xml
│   ├── pkg12_py05_names
│   │   ├── demo01_names_py.py
│   │   └── __init__.py
│   ├── resource
│   │   └── pkg12_py05_names
│   ├── setup.cfg
│   ├── setup.py
│   └── test
│       ├── test_copyright.py
│       ├── test_flake8.py
│       └── test_pep257.py

├── pkg13_cpp06_names       # 📦时间相关API  C++
│   ├── CMakeLists.txt
│   ├── include
│   │   └── pkg13_cpp06_names
│   ├── package.xml
│   └── src
│       └── demo01_time.cpp     # 时间相关API C++

├── pkg14_py06_names        # 📦时间相关API  py
│   ├── package.xml
│   ├── pkg14_py06_names
│   │   ├── demo01_time_py.py    # 时间相关API  py
│   │   └── __init__.py
│   ├── resource
│   │   └── pkg14_py06_names
│   ├── setup.cfg
│   ├── setup.py
│   └── test
│       ├── test_copyright.py
│       ├── test_flake8.py
│       └── test_pep257.py



```
