int main(object me, string arg)
{
    write("Bye，欢迎下次光临^_^\n");
    say(geteuid(me) + "离开了" MUD_NAME "。\n");
    destruct(me);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : quit

这个指令可以让你礼貌的退出聊天室。

HELP );
    return 1;
}
