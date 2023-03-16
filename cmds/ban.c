int main(object me, string arg)
{
    if (!wizardp(me))
    {
        return 0;
    }

    if (!arg)
    {
        printf("目前被限制访问的IP名单：\n%s\n", read_file(DENIED_IPS));
    }
    else
    {
        object player = find_player(arg);
        if (objectp(player))
        {
            write_file(DENIED_IPS, query_ip_number(player));
            destruct(player);
        }
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : ban <用户>
指令说明:
    禁止指定用户访问本站，注意限制的是登录IP。
HELP);

    return 1;
}
