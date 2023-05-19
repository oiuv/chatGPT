#include <ansi.h>
inherit _EXTERNAL_CMD;

protected void response(string result)
{
    shout(result);
}

int main(object me, string arg)
{
    int PYTHON_CMD = 3;
    string mobile = fetch_variable("Mobile", me);

    if (!arg)
    {
        return notify_fail(HIC "指令格式：mailto 邮箱地址，如：mailto mail@gmail.com\n" NOR);
    }

    if (!pcre_match(arg, "^[a-zA-Z0-9._+-]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,}$"))
    {
        return notify_fail(YEL "请输入正确的邮箱地址！ 😅\n" NOR);
    }

    if (!mobile)
    {
        return notify_fail(RED "你还没有认证身份，无法发送会话记录到你的邮箱，请输入" HIY " verify " NOR RED "认证。\n" NOR);
    }

    if (file_size("/log/history/" + mobile + ".txt") < 1)
    {
        return notify_fail(YEL "当前认证号码下还没有会话存档，请先和chatGPT聊聊呗 😅\n" NOR);
    }

    if (__ARCH__ == "Microsoft Windows")
        PYTHON_CMD = 4;

    external_cmd(PYTHON_CMD, "mail.py " + arg + " " + mobile);
    write(MAG "邮件已发送，请注意查收 💌\n" NOR);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : mailto <邮箱>

发送你和chatGPT的聊天记录到指定邮箱。

HELP );
    return 1;
}
