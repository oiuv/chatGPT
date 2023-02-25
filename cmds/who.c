#include <ansi.h>

int getcid(object ob)
{
    int id;

    sscanf(file_name(ob), "%*s#%d", id);
    return id;
}

int main(object me, string arg)
{
    printf(HIC "%-6s%-20s%-9s\n" NOR, "编号", "昵称", "状态");
    printf("%s\n", repeat_string("-*-", 11));
    foreach (object user in livings())
    {
        int idle = query_idle(user);
        printf("%-6d%-20s%-9s\n", getcid(user), geteuid(user), idle < 180 ? "<活跃中>" : "发呆 " + idle + " 秒");
    }
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : who
指令说明:
    查看在线用户列表。
HELP);

    return 1;
}
