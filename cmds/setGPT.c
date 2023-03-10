int main(object me, string arg)
{
    // 设置chatGPT的角色行为
    return me->setGPT(arg);
}

int help(object me)
{
    write(@HELP
指令格式 : setGPT <角色描述>

为chatGPT设置角色定位，如：“你是一位翻译家，你的任务是把任何语言翻译成中文”。

直接输入`chatGPT`为取消角色设置。

HELP );
    return 1;
}
