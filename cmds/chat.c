#include <ansi.h>

int main(object me, string arg, string type)
{
    if (!arg)
        arg = "...";

    arg = HIM "【" + (type || "聊天") + "】" NOR HIY + geteuid(me) + "@" MUD_NAME "：" NOR HIC + arg + NOR "\n";

    write(arg);
    say(arg);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : chat <聊天内容>

在线聊天命令，不会向chatGPT提问。

HELP );
    return 1;
}
