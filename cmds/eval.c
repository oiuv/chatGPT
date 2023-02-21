int help(object me);

int main(object me, string arg)
{
    object eval_ob;
    string filename, file;

    if (!wizardp(me))
    {
        return 0;
    }

    if (!arg)
        return help(me);

    filename = "/tmp/debug_eval_file.c";

    /* clean up first */
    if (file_size(filename) != -1)
        rm (filename);
    if (eval_ob = find_object(filename))
        destruct(eval_ob);

    file = "#include <ansi.h>\nmixed eval(object me) { " + arg + "; }\n";
    write_file(filename, file, 1);
    printf("Result = %O\n", filename->eval(me));

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : eval <lpc code>
指令说明:
    测试专用，可直接运行代码片断，如：`eval return me`
HELP);

    return 1;
}
