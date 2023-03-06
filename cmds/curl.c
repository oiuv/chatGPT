inherit _EXTERNAL_CMD;

protected void response(string result)
{
    shout(result);
}

int main(object me, string arg)
{
    int CURL_CMD = 1;

    if (!wizardp(me))
    {
        return 0;
    }

    if (__ARCH__ == "Microsoft Windows")
        CURL_CMD = 2;

    exec(CURL_CMD, arg || "-h");

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : curl <...>

调试专用，调用系统curl指令发起http(s)请求。

HELP );
    return 1;
}
