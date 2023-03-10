int main(object me, string arg)
{
    // 直接向chatGPT提问
    return me->chat(arg);
}

int help(object me)
{
    write(@HELP
指令格式 : chatGPT <prompt>

向chatGPT提问，注意chatGPT回答需要时间，请不要重复提问。

直接输入`chatGPT`为显示基本信息，而输入`chatGPT -d`为清除上下文关联。

HELP );
    return 1;
}
