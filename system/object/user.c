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
nosave int mobile;
nosave mapping data = ([]);

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
    if (mobile)
        return 0;

    // 限制每60秒指令数
    t = time() / 60;
    if (AtTime != t)
    {
        AtTime = t;
        UserCommand = 1;
    }
    else
        UserCommand++;
    // 限制最多3条指令
    if (UserCommand > 3)
        return 1;

    return 0;
}

mixed process_input(string verb)
{
    string *word;
    mapping alias = ([
        "say"  : "chat",
        "exit" : "quit",
        "set"  : "setGPT",
        "gpt"  : "chatGPT"
    ]);

    switch (verb[0])
    {
    case '@':
        return "who " + verb[1..];
    case '.':
        return "chat " + verb[1..];
    }

    verb = replace_string(verb, "|CRLF|", "\n");
    word = explode(verb, " ");

    if (sizeof(word))
    {
        // 指令限制
        if (word[0] != "verify" && reject_command())
        {
            write(RED "⚠️  未验证手机用户限制每分钟 3 次请求，请使用`" HIY "verify 手机号码" NOR RED "`做安全认证\n" NOR);
            write(YEL "⚠️  完成手机验证的优势：\n1. 解除 3 RPM 的会话次数限制\n2. 保留和chatGPT的全部会话历史记录\n3. 可使用 mailto 指令下载会话记录到个人邮箱\n" NOR);
            return 1;
        }
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
    // 随机密钥
    string key = element_of(config("openai_api_keys"));
    // 会话历史关联条数（包括 role 和 content，所以需*2）
    int history = config("history") * 2 || 6;
    //采样时使用的温度值，取值范围为0到2之间，值越大生成输出的随机性越强
    float temperature = config("temperature") || 1;
    string proxy = config("proxy");
    string *args = ({"-s", "https://api.openai.com/v1/chat/completions", "-H", "Content-Type: application/json", "-H", "Authorization: Bearer " + key});
    int CURL_CMD = 1;
    mapping data;

    if (__ARCH__ == "Microsoft Windows")
        CURL_CMD = 2;
    if (proxy)
        args += ({"-x", proxy});
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
    if (bitCheck(config("log"), LOG_Q))
    {
        // 为了安全，记录提问信息
        write_file(LOG_DIR + "chatGPT.log", sprintf("[%s]%-16s%-14s%s\n", ctime(), query_ip_number(), geteuid(), prompt));
    }
    // store prior responses
    if (sizeof(Reply))
        Messages += ({(["role":"assistant", "content":Reply])});
    // 关联最近N/2条会话
    // todo 这里可以增加total_tokens判断避免超过上限，但3条会话大概率不会超，暂不判断
    Messages = Messages[< history..] + ({(["role":"user", "content":prompt])});
    // 设置chatGPT的角色
    if (sizeof(Role))
    {
        if (Messages[0]["role"] == "system")
            Messages[0]["content"] = Role;
        else
            Messages = ({(["role":"system", "content":Role])}) + Messages;
    }

    data = ([
        "model"       : "gpt-3.5-turbo",
        "temperature" : temperature,
        "messages"    : Messages
    ]);

    external_cmd(CURL_CMD, args + ({"-d", json_encode(data)}));

    return 1;
}

protected void response(string result)
{
    mixed data = ([]);
    string content = result, msg;
    // 读取LIB根目录下tips.md文件中的随机提示
    string tips = CYN "\n-提示" + element_of(read_lines("tips.md")) + NOR "\n";

    if (bitCheck(config("log"), LOG_R))
    {
        // 备份问答
        write_file(LOG_DIR + "chatGPT.txt", "> " + Prompt + "\n" + result + "\n");
    }
    if (pcre_match(result, "^{.+}$"))
    {
        if (bitCheck(config("log"), LOG_J))
        {
            // 备份JSONL文件
            write_file(LOG_DIR + "chatGPT.jsonl", result);
        }
        data = json_decode(result);
    }
    if (data["error"])
    {
        content = data["error"]["message"];
    }
    else if (data["choices"])
    {
        content = data["choices"][0]["message"]["content"];
        if (bitCheck(config("log"), LOG_A))
        {
            // 备份问答
            write_file(LOG_DIR + "chatGPT.md", "## " + Prompt + "\n\n" + content + "\n\n");
        }
        if (mobile)
        {
            // 用户历史消息
            write_file(LOG_DIR + "history/" + mobile + ".txt", "> " + Prompt + "\n\n" + content + "\n\n");
        }
        // 记录usage
        Usage = data["usage"];
        // 让聊天室更有气氛
        say(sprintf("【%s】chatGPT回复了 %s 的消息，会话消耗 %d tokens 😘\n", ctime(data["created"]), geteuid(), Usage["total_tokens"]));
    }
    else if (data["SuccessCounts"])
    {
        content = "验证码信息已发送至您的手机，请注意查收 📱 并输入指令 `verify <验证码>`完成验证 💟";
    }

    if (!sizeof(content))
    {
        content = "💤💥💢 <服务器未能正确响应请求> 💢💥💤";
    }

    msg = HIG "『chatGPT』" NOR + content + "\n";

    tell_object(this_object(), msg + tips);

    // 清除提问
    Prompt = 0;
    // 记录message
    Reply = content;
}

int setGPT(string role)
{
    Role = role;

    write(HIC "🤖 已设置chatGPT的角色描述为：" HIY + (Role || "空") + NOR "\n");
    write(CYN "🤖 请发送消息给chatGPT开始神奇的会话之旅吧\n" NOR);
    return 1;
}

// 个人认证，发送短信
void sms(string tpl)
{
    int CURL_CMD = 1;
    string url;
    string AppCode = config("AppCode");

    if (!AppCode)
    {
        error("请先在config.json中配置AppCode！");
    }
    if (data["verify_code"])
    {
        tpl = "【雪风】你的验证码是：" + data["verify_code"] + "，请勿泄漏于他人！";
    }
    url = "http://gwgp-wtxhytukujk.n.bdcloudapi.com/chuangxin/dxjk?content=" + tpl + "&mobile=" + data["mobile"];
    external_cmd(CURL_CMD, ({"-s", "--location", url, "--header", "Content-Type: application/json;charset=UTF-8", "--header", "X-Bce-Signature: AppCode/" + AppCode}));

    // 记录日志
    write_file(LOG_DIR "mobile", "[" + ctime() + "]" + data["mobile"] + "\t" + query_ip_number(this_object()) + "\n");
}
