# LINKUS_Wh
LINKUS，IM项目，研究生二年级完成，修改于2024/10/13
项目分析
IM聊天系统：游戏有，购物软件，小红书，博客留言，很广泛。
※通信协议：TCP协议，为什么选择这个协议？※
※模型：C/S模型，为什么选择这个模型？※


※怎么判断传输过来的数据，是跟你的注册登录功能相对应的呢？※
我自己设置了一个协议头的一种匹配的机制，在客户端和服务端都约定好这种。
在每一个数组的前面加一个 *int*(data)
功能有
注册:
登陆:
获得好友列表:
聊天:
添加好友:
下线:

面对对象编程：类

客户端：Qt
net类（收发数据，初始化网络，关闭网络）
中介者类（为了以后扩展使用）
kernel类（处理接收到的数据，组织要发送的数据）
ui类（注册&登录界面&友好列表界面&窗口聊天）

服务端：VScode
net类（收发数据，初始化网络，关闭网络）
中介者类（为了以后扩展使用）
kernel类（处理接收到的数据，组织要发送的数据）
mysql类

net类(继承和多态)
父类(收发数据，初始化网络，关闭网络)
TCP子类(TCP子类，TCP客户端)（收发数据，初始化网络，关闭网络）
UDP子类（收发数据，初始化网络，关闭网络）
