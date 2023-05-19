#include <ansi.h>
#include "simul_efun/json.c"

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

// 系统配置信息，缓存到全局变量
mapping Config;
mixed config(string key)
{
    Config = Config || json_decode(read_file("config.json"));
    if (key)
    {
        return Config[key];
    }
    else
    {
        return Config;
    }
}

//检查state的第pos位是否为1
int bitCheck(int state, int pos)
{
    return state & (1 << (pos - 1));
}
