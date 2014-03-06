NodeNet
=======
worker: 业务员，用于业务逻辑的扩展，允许多个挂接节点

scheduler: 调度员，用于任务分发

log: 实现不同等级日记输出

task：负责worker跟dispatcher内部的沟通

connection: 对socket API封装

env: 维护环境变量

任务：
1.网络模块
2.线程模块
3.消息分发机制
4.节点管理
