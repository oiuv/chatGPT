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
        move_object(VOID_OB);
        say(HIG "🥰 ~Hi~用户(" + id + ")连线了^_^\n" NOR);
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
        string prompt = query_verb() + (arg || "");
        CHAT_CMD->main(this_object(), prompt);

        return CHATGPT_CMD->main(this_object(), prompt);
    }
}

mixed process_input(string verb)
{
    string *word = explode(verb, " ");
    mapping alias = ([]);

    // verb = lower_case(verb);

    switch (verb[0])
    {
    case '@':
        return "chat " + verb[1..];
    }

    if (sizeof(word) && !undefinedp(alias[word[0]]))
    {
        word[0] = alias[word[0]];
        return implode(word, " ");
    }

    return verb;
}

void net_dead()
{
    say(HIR "😇 ~Bye~ 用户(" + geteuid() + ")离开了……\n" NOR);
    destruct();
}
