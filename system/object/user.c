#include <ansi.h>

#define CMD_PATH "/cmds/"
#define CHAT_CMD "/cmds/chat"
#define CHATGPT_CMD "/cmds/chatGPT"

int command_hook(string arg);

varargs void create(string id)
{
    if (clonep())
    {
        enable_commands();
        add_action("command_hook", "", 1);
        seteuid(id);
        set_living_name(id);
        set_heart_beat(60);
        move_object(VOID_OB);
        say(HIG "🥰 ~Hi~ 用户(" + id + ")连线了^_^\n" NOR);
    }
}

int command_hook(string arg)
{
    string cmd;
    object cmd_ob;

    cmd = CMD_PATH + query_verb();

    if (cmd_ob = load_object(cmd))
    {
        return cmd_ob->main(this_object(), arg);
    }
    else
    {
        // 没有匹配到指令的转为聊天或提问
        string prompt = query_verb() + (arg ? " " + arg : "");
        if (strlen(prompt) < 5)
        {
            CHAT_CMD->main(this_object(), prompt);
            return notify_fail(HIW "【提示】因API资源有限，少于5个字符的内容默认为聊天而不是提问\n" NOR, );
        }
        else
        {
            return CHATGPT_CMD->main(this_object(), prompt);
        }
    }
}

mixed process_input(string verb)
{
    string *word = explode(verb, " ");
    mapping alias = ([
        "say":"chat",
        "exit":"quit",
    ]);

    // verb = lower_case(verb);

    switch (verb[0])
    {
    case '@':
        return "who " + verb[1..];
    case '.':
        return "chat " + verb[1..];
    }

    if (sizeof(word))
    {
        // 长内容直接转为提问
        if (sizeof(word[0]) > 15)
        {
            return "chatGPT " + verb;
        }
        // 处理指令别名
        if (!undefinedp(alias[word[0]]))
        {
            word[0] = alias[word[0]];
            return implode(word, " ");
        }
    }

    return verb;
}

void net_dead()
{
    say(HIR "😇 ~Bye~ 用户(" + geteuid() + ")离开了……\n" NOR);
    destruct();
}

void heart_beat()
{
    if (query_idle(this_user()) > 1800)
    {
        write(HIR "💔 ~Bye~ 因超过半小时不活跃，你自动离线了……\n" NOR);
        say(HIR "💔 ~Bye~ 用户(" + geteuid() + ")因发呆时间过长自动离线了……\n" NOR);
        destruct();
    }
}
