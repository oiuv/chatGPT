inherit _EXTERNAL_CMD;

#include <ansi.h>

#define CMD_PATH "/cmds/"
#define CHAT_CMD "/cmds/chat"
#define CHATGPT_CMD "/cmds/chatGPT"

nosave string Prompt;
nosave string Reply;
nosave string *Messages = ({});

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
        if (strlen(prompt) < 2)
        {
            CHAT_CMD->main(this_object(), prompt);
            return notify_fail(HIW "【提示】为节省资源，少于2个字符的内容不发送给chatGPT\n" NOR, );
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

// 根据config.cfg中external_cmd_x指定
#define OPENAI_CMD 3
//todo 上下文角色处理
int prompt(string prompt)
{
    // 读取LIB根目录下的OPENAI_API_KEY文件中配置的随机密钥
    string key = element_of(read_lines("OPENAI_API_KEY"));
    string *args = ({"-k", key, "api", "chat_completions.create", "-m", "gpt-3.5-turbo"});

    if (!prompt)
    {
        Reply = 0;
        Messages = ({});
        return notify_fail(HIY "已重置chatGPT会话历史记录😘\n" NOR);
    }
    if (Prompt)
        return notify_fail(HIR "请等待chatGPT回复后再继续提问吧😅\n" NOR);
    // 缓存问题
    Prompt = prompt;
    // 显示问题
    write(HIG "『你』💬 " NOR HIC + prompt + NOR "\n");
    // 为了安全，记录提问信息
    write_file(LOG_DIR + "chatGPT.log", sprintf("[%s]%-16s%-14s%s\n", ctime(), query_ip_number(), geteuid(), prompt));
    // store prior responses
    if (sizeof(Messages) < 30 && sizeof(Reply) && sizeof(Reply) < 1000)
        Messages += ({"-g", "assistant", "\"" + Reply + "\""});
    else
        Messages = ({});

    Messages += ({"-g", "user", "\"" + prompt + "\""});
    // usage: openai [-h] [-v] [-b API_BASE] [-k API_KEY] [-o ORGANIZATION] {api,tools,wandb} ...
    // exec(OPENAI_CMD, ({"-k", key, "api", "completions.create", "-m", "text-davinci-003", "-M", "3072", "-p", prompt }));
    exec(OPENAI_CMD, args + Messages);

    return 1;
}

protected void response(string result)
{
    string arg = HIG "『chatGPT』" NOR + result + "\n";
    // 读取LIB根目录下tips.md文件中的随机提示
    string tips = CYN "\n-提示" + element_of(read_lines("tips.md")) + NOR"\n";
    tell_object(this_object(), arg + tips);
    // 备份问答
    write_file(LOG_DIR + "chatGPT.md", "## " + Prompt + "\n" + result + "\n\n");
    // 清除提问
    Prompt = 0;
    // 记录回答
    Reply = result;
}
