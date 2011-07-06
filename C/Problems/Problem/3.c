/* 请用read和write系统调用从标准输入获得用户的输入并将内容输出到标准输出 */
/* 当用户输入结束（Ctrl+Z表示输入结束）时，程序退出 */

# include <unistd.h>
# include <stdio.h>

# define MAX_BUF_LEN 256

int main()
{
    /* 存放内容 */
    char buff[MAX_BUF_LEN];

    /* code in below area */
    int ret;
    while((ret=read(STDIN_FILENO,buff,sizeof(buff)))>0)
    {
        ret=read(STDIN_FILENO,buff,sizeof(buff));
       // printf("%d\n",STDIN_FILENO);
       // printf("%d\n",STDOUT_FILENO);
       // printf("%d\n",ret);
        write(STDOUT_FILENO,buff,ret);
    }
    /* code in above area */

    return 0;
}
