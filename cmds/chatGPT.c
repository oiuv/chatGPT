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

HELP );
    return 1;
}
