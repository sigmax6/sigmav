
1、目前只是实现了用dbClient端发送请求，由dbServer端对比目录并返回结果
2、./dbCheckServer 进入守候模式 默认端口6666
   ./dbCheckClient 127.0.0.1 （服务器ip) 会发送检查命令 (#define DEBUG
       决定了是否使用调试模式)
3、#define CHECK_FOLDER 指定检查路径的全名（完整路径)
    /home/sigmax6/Test/
                      Test1/  Test1 -> 保存列表
                                  1
                                  2
                                  3
                                  4
                                  5
                                  ... -> 普通文件
4、假定列表中一行两列 分别是文件名和文件大小
5、检查正常会将列表文件传送 错误会返回字符串"Error"
