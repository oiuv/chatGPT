int main(object me, string arg)
{
    function f;

    if (member_array(query_ip_number(me), ({"127.0.0.1", "118.190.104.241", "47.88.55.34"})) < 0)
    {
        return 0;
    }

    if (wizardp(me))
    {
        f = bind((: disable_wizard :), me);
        write("关闭wizard权限!\n");
    }
    else
    {
        f = bind((: enable_wizard :), me);
        write("开启wizard权限!\n");
    }
    evaluate(f);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : wizard

切换wizard权限。

HELP );
    return 1;
}
