# ros学习

## 1.项目结构

```bash
# 📦📨🧩✨🧱📝📄🚀
├── base_interfaces_demo	# 📦自定义消息文件
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
```
