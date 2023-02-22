#include <ansi.h>

// debug_message 随机彩色版，方便调试
void debug(mixed arg)
{
    string *color = ({HIB, HIC, HIG, HIM, HIR, HIW, HIY});

    debug_message(sprintf("%s%O%s", element_of(color), arg, NOR));
}

// 读取文件返回每一行内容构成的数组
string *read_lines(string file)
{
    if (file_size(file) > 0)
    {
        return filter_array(explode(read_file(file), "\n"), (: trim($1) != "" && $1[0] != '#' && $1[0] != ';' :));
    }
    else
        return ({});
}
