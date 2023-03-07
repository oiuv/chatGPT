#include <ansi.h>

int help(object me);

int main(object me, string file)
{
    object obj;

    if (!file)
    {
        return help(me);
    }

    write("重新编译[" + file + "]:");

    if (obj = find_object(file))
    {
        destruct(obj);
    }
    if (objectp(load_object(file)))
    {
        write(HIG "编译成功!\n" NOR);
    }
    else
    {
        write(HIR "失败，文件不存在!\n" NOR);
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : update <对象文件名>
指令说明:
    这个指令用来重新载入一个对象。
HELP );
    return 1;
}
