void setup(string id)
{
    object from, to;

    if (!id || id == "")
    {
        write("ID不能为空，请重新输入：");
        input_to("setup");
    }
    else
    {
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
    write("请输入你的ID：");
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
