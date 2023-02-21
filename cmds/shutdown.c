int main(object me, string arg)
{
    if (!wizardp(me))
    {
        return 0;
    }

    shutdown(0);
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式: shutdown

关闭mudGPT服务器。

HELP);
    return 1;
}
