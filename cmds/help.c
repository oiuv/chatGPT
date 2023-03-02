int main(object me, string arg)
{
    string file;

    if (!arg)
    {
        write(read_file(MOTD));
        write(read_file("tips.md"));
    }
    else if (file_size(file = "/cmds/" + arg + ".c") > 0)
    {
        notify_fail("有这个指令存在，但是并没有详细的说明文件。\n");
        return file->help();
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
