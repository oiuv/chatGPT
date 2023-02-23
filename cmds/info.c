#include <ansi.h>

string duration()
{
    int t, d, h, m, s;
    string time;

    t = uptime();
    s = t % 60;
    t /= 60;
    m = t % 60;
    t /= 60;
    h = t % 24;
    t /= 24;
    d = t;

    if (d)
        time = d + "天";
    else
        time = "";

    if (h)
        time += h + "小时";
    if (m)
        time += m + "分";
    time += s + "秒";

    return time;
}

int main(object me, string arg)
{
    write("驱动环境：" + __ARCH__ + "\n");
    write("驱动版本：" + __VERSION__ + "\n");
    write("执行效率：" + query_load_average() + "\n");
    write("内存占用：" + memory_info() / 1024 / 1024 + "M\n");
    write("运行时间：" + duration() + "\n");
    write("在线用户：" + sizeof(users()) + "人\n");
    write("载入对象：" + sizeof(objects()) + "个\n");

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : info

这个指令可以让你查看服务器基本运行信息。

HELP );
    return 1;
}
