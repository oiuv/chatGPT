inherit _EXTERNAL_CMD;

#include <ansi.h>

#define CMD_PATH "/cmds/"

// chatGPT的角色
nosave string Role;
// 用户发送提示内容
nosave string Prompt;
// chatGPT回复内容
nosave string Reply;
// 当前会话的Usage
nosave mapping Usage;
// 会话消息内容
nosave mixed *Messages = ({});

nosave int AtTime;
nosave int UserCommand;

int chat(string prompt);
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
        say(HIC "提示：你可以使用 `chat` 指令和其他用户聊天，如：chat 大家好呀~\n" NOR);
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
        // 没有匹配到指令的转为会话
        write(YEL "🤖 为了让chatGPT更懂你，建议尽可能详细地描述问题和需求~\n" NOR);
        return chat(prompt);
    }
}

int reject_command()
{
    int t;

    if (wizardp(this_object()))
        return 0;
    // 限制每10秒指令数
    t = time() / 10;
    if (AtTime != t)
    {
        AtTime = t;
        UserCommand = 1;
    }
    else
        UserCommand++;
    // 限制最多2条指令
    if (UserCommand > 2)
        return 1;

    return 0;
}

mixed process_input(string verb)
{
    string *word = explode(verb, " ");
    mapping alias = ([
        "say"  : "chat",
        "exit" : "quit",
        "set"  : "setGPT",
        "gpt"  : "chatGPT"
    ]);

    // verb = lower_case(verb);
    if (reject_command())
    {
        write(RED "⚠️ 服务器负载过高，请稍等几秒再发送……\n" NOR);
        return 1;
    }

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
        if (sizeof(word[0]) > 15 || (strsrch(word[0], "#") != -1))
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

    return trim(verb);
}

void net_dead()
{
    say(HIR "😇 ~Bye~ 用户(" + geteuid() + ")离开了……\n" NOR);
    destruct();
}

void heart_beat()
{
    if (!wizardp(this_user()) && (query_idle(this_user()) > 3600))
    {
        write(HIR "💔 ~Bye~ 因超过1小时不活跃，你自动离线了……\n" NOR);
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
        write(HBYEL "chatGPT Usage" NOR "\n");
        printf("角色描述：%s\n", Role || "未设定chatGPT的身份");
        printf("会话记录：%O\n", Messages);
        printf("最新消息：%O\n", Reply);
        printf("令牌信息：%O\n", Usage);
        return 1;
    }
    else if (prompt == "-d")
    {
        Role = 0;
        Reply = 0;
        Messages = ({});
        return notify_fail(HIY "已清除chatGPT角色设定和上下文会话记录😘\n" NOR);
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
    // 关联最近1条会话
    Messages = Messages[< 2..] + ({(["role":"user", "content":prompt])});
    // 设置chatGPT的角色
    if (sizeof(Role))
    {
        Messages = ({(["role":"system", "content":Role])}) + Messages;
    }

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
    // 让聊天室更有气氛
    say(sprintf("【%s】chatGPT回复了 %s 的消息，会话消耗 %d tokens 😘\n", ctime(data["created"]), geteuid(), Usage["total_tokens"]));
}

int setGPT(string role)
{
    Role = role;
    if (role == "-d")
    {
        Role = 0;
    }

    write(HIC "🤖 已设置chatGPT的角色描述为：" HIY + (Role || "空") + NOR "\n");
    write(CYN "🤖 请发送消息给chatGPT开始神奇的会话之旅吧\n" NOR);
    return 1;
}
