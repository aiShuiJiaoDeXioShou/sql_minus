项目基于标准C库开发

编译：

```shell
gcc -o sql_minus main.c lexer.c parser.c database.c query.c io.c error.c utils.c -I.

```

单元测试在test目录下，直接使用gcc编译，windows用户运行bat文件即可。  

