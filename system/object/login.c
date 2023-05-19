#include <ansi.h>

void setup(string id)
{
    object from, to;

    id = trim(id);
    if (!pcre_match(id, "^[0-9a-zA-Z\\p{Han}]{2,8}$"))
    {
        write(HIY "\n⚠️  昵称不能包含特殊符号且长度要求为2-8位\n请重新输入昵称：" NOR);
        input_to("setup");
    }
    else if (member_array(lower_case(id), read_lines(SENSITIVEWORDS)) != -1)
    {
        write(HIR "\n🈲 此昵称为本站禁止使用的敏感词汇\n请重新输入昵称：" NOR);
        input_to("setup");
    }
    else if (find_player(id))
    {
        write(HIM "\n💔 您所选择的昵称已经被其他用户抢先使用\n请重新输入昵称：" NOR);
        input_to("setup");
    }
    else
    {
        write(HIG "\n😘 欢迎来到" MUD_NAME "，请直接发言向chatGPT提问吧~\n" NOR);
        write(HIY "\n🧡 「提问示例」" NOR "请提供10个向ChatGPT提问的示例，要求问题涵盖不同领域且具有示范性。\n");
        write(HIY "\n🧡 「功能提示」" NOR "你可以使用指令" HIC "set" NOR "指定chatGPT的能力，如 " HIC "set 3" NOR " 为翻译外语成中文。\n");
        write(RED "\n⚠️  因API余额不足，对未认证的游客用户限制每分钟 3 次请求，请使用`" HIY "verify 手机号码" NOR RED "`认证身份\n" NOR);
        write(HIG "\n⚠️  使用手机完成验证的好处：\n1. 解除 3 RPM 的会话次数限制\n2. 保留你和chatGPT的全部会话历史记录\n3. 可使用 mailto 指令下载会话记录到个人邮箱\n" NOR);
        from = this_object();
        to = new(USER_OB, id);
        exec(to, from);
        destruct(from);
        // log
        debug_message(ctime() + " " + query_ip_number(to) + " " + id);
    }
}

int checkDenied()
{
    return member_array(query_ip_number() ,read_lines(DENIED_IPS)) != -1;
}

void login()
{
    write(read_file(MOTD));
    write(read_file("tips.md"));
    write("\n");
    if (checkDenied())
    {
        write(HBRED "🈲 对不起，您的IP已被禁止访问 💔" NOR);
        destruct();
    }
    write(HBRED "提问之前请先给自己起个有趣的昵称：" NOR);
    input_to("setup");
}

void logon()
{
    // 延迟登陆，解决mudlet初次连接v2019编码自动识别问题
    call_out_walltime("login", 0.25);
}

void net_dead()
{
    debug_message(ctime() + " net_dead " + query_ip_number());
    destruct();
}
