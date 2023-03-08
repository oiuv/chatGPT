inherit _EXTERNAL_CMD;

#include <ansi.h>

#define CMD_PATH "/cmds/"
#define CHAT_CMD "/cmds/chat"
#define CHATGPT_CMD "/cmds/chatGPT"

nosave string Prompt;
nosave string Reply;
nosave mapping Usage;
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
    if (!wizardp(this_user()) && (query_idle(this_user()) > 1800))
    {
        write(HIR "💔 ~Bye~ 因超过半小时不活跃，你自动离线了……\n" NOR);
        say(HIR "💔 ~Bye~ 用户(" + geteuid() + ")因发呆时间过长自动离线了……\n" NOR);
        destruct();
    }
}

// curl版api
int chat(string prompt)
{
    // 读取LIB根目录下的OPENAI_API_KEY文件中配置的随机密钥
    string key = element_of(read_lines("OPENAI_API_KEY"));
    string *args = ({"-s", "https://api.openai.com/v1/chat/completions", "-H", "Content-Type: application/json", "-H", "Authorization: Bearer " + key});
    int CURL_CMD = 1;
    mapping data;

    if (__ARCH__ == "Microsoft Windows")
        CURL_CMD = 2;

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
    write(HIG "『 你 』💬 " NOR HIC + prompt + NOR "\n");
    // 为了安全，记录提问信息
    write_file(LOG_DIR + "chatGPT.log", sprintf("[%s]%-16s%-14s%s\n", ctime(), query_ip_number(), geteuid(), prompt));
    // store prior responses
    if (sizeof(Reply))
        Messages += ({(["role":"assistant", "content":Reply])});

    Messages = Messages[< 2..] + ({(["role":"user", "content":prompt])});
    data = ([
        "model": "gpt-3.5-turbo",
        "messages": Messages
    ]);

    exec(CURL_CMD, args + ({"-d", json_encode(data)}));

    return 1;
}

protected void response(string result)
{
    mixed data = ([]);
    string content = result, msg;
    // 读取LIB根目录下tips.md文件中的随机提示
    string tips = CYN "\n-提示" + element_of(read_lines("tips.md")) + NOR"\n";

    if (pcre_match(result, "^{.+}$"))
    {
        data = json_decode(result);
    }
    if (data["error"])
    {
        content = data["error"]["message"];
    }
    else if (data["choices"])
    {
        content = data["choices"][0]["message"]["content"];
    }

    msg = HIG "『chatGPT』" NOR + content + "\n";

    tell_object(this_object(), msg + tips);
    // 备份问答
    write_file(LOG_DIR + "chatGPT.md", "## " + Prompt + "\n" + content + "\n\n");
    // 清除提问
    Prompt = 0;
    // 记录message
    Reply = content;
    // 记录usage
    Usage = data["usage"];
}
