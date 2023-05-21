int main(object me, string arg)
{
    function f;

    if (member_array(fetch_variable("Mobile", me), config("wizards")) < 0)
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
