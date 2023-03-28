nosave mixed Prompts;

void create()
{
    Prompts = json_decode(read_file("prompts.json"));
}

int main(object me, string arg)
{
    if (!arg)
    {
        int i = 0;
        write("设置chatGPT的角色，示例如下：\n");
        foreach (mapping prompt in Prompts)
            printf("%3d. %s\n", i++, prompt["act"]);

        return notify_fail("请使用`set 编号`为chatGPT设置特定角色。\n");
    }

    // 如果是自己描述角色，内容不应过短
    if (sizeof(arg) < 10)
    {
        int index = to_int(arg);
        if (index < 0 || index > 120)
        {
            return notify_fail("请使用`set 编号`为chatGPT设置特定角色，编号范围：0-120\n");
        }

        arg = Prompts[index]["prompt"];
    }

    // 设置chatGPT的角色行为
    return me->setGPT(arg);
}

int help(object me)
{
    write(@HELP
指令格式 : setGPT [<角色描述>|<角色编号>]

为chatGPT设置角色定位，如：“你是一位翻译家，你的任务是把任何语言翻译成中文”。

输入`setGPT -d`为取消角色设置。

HELP );
    return 1;
}
