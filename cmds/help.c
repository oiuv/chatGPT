#include <ansi.h>

int main(object me, string arg)
{
    string file;

    if (!arg)
    {
        write(read_file(MOTD));
        write(read_file("tips.md"));
        return notify_fail(HIC "\n> 如需chatGPT使用的更多帮助请输入：" HIY "help tutorial\n" NOR);
    }
    else if(arg == "tutorial")
    {
        write(read_file("tutorial.md"));
    }
    else if (file_size(file = "/cmds/" + arg + ".c") > 0)
    {
        notify_fail("有这个指令存在，但是并没有详细的说明文件。\n");
        return file->help();
    }
    else
    {
        return notify_fail("没有这个指令，找不到帮助信息 😭\n");
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : help [?]

帮助命令，可查看指定命令的作用。

HELP );
    return 1;
}
