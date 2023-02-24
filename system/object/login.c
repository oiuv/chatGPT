#include <ansi.h>

void setup(string id)
{
    object from, to;

    id = trim(id);
    if (!pcre_match(id, "^[0-9a-zA-Z\\p{Han}]{3,9}$"))
    {
        write(HIY "\n⚠️ 昵称不能包含特殊符号且长度要求为3-9位\n请重新输入昵称：" NOR);
        input_to("setup");
    }
    else if (member_array(lower_case(id), read_lines("SensitiveWords.txt")) != -1)
    {
        write(HIR "\n🈲 此昵称为本站禁止使用的敏感词汇\n请重新输入昵称：" NOR);
        input_to("setup");
    }
    else if (find_player(id))
    {
        write(HIM "\n💔 此昵称已被其它用户占用了\n请重新输入昵称：" NOR);
        input_to("setup");
    }
    else
    {
        write(HIG "\n😘 欢迎来到" MUD_NAME "，请直接发言向chatGPT提问吧~\n" NOR);
        from = this_object();
        to = new(USER_OB, id);
        exec(to, from);
        destruct(from);
        // log
        debug_message(ctime() + " " + query_ip_number(to) + " " + id);
    }
}

void login()
{
    write(read_file(MOTD));
    write(HIC "请输入你的昵称：" NOR);
    input_to("setup");
}

void logon()
{
    // 延迟登陆，解决mudlet初次连接v2019编码自动识别问题
#ifdef FLUFFOS
    call_out_walltime("login", 0.25);
#else
    call_out("login", 1);
#endif
}

void net_dead()
{
    debug_message(ctime() + " net_dead " + query_ip_number());
    destruct();
}
