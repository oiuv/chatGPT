inherit _EXTERNAL_CMD;

protected void response(string result)
{
    shout(result);
}

int main(object me, string arg)
{
    int PYTHON_CMD = 3;

    if (!wizardp(me))
    {
        return 0;
    }

    if (__ARCH__ == "Microsoft Windows")
        PYTHON_CMD = 4;

    external_cmd(PYTHON_CMD, arg || "-h");

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : python <...>

调试专用，调用系统python指令运行程序。

HELP );
    return 1;
}
