#define ROOT_UID "Root"
#define BACKBONE_UID "Backbone"
#define MUID "MudGPT"

string get_root_uid()
{
    return ROOT_UID;
}

string author_file(string file)
{
    debug_message("author_file : " + file);
    if (file == "/system/kernel/master")
    {
        return ROOT_UID;
    }

    return MUID;
}

string get_bb_uid()
{
    return BACKBONE_UID;
}

string domain_file(string file)
{
    debug_message("domain_file : " + file);
    if (file == "/")
    {
        return ROOT_UID;
    }

    return MUID;
}

string creator_file(string file)
{
    debug_message("creator_file : " + file);
    return MUID;
}

object connect(int port)
{
    return new(LOGIN_OB);
}

/**
 * 日志记录
 */
void log_error(string file, string message)
{
    write_file(LOG_DIR + "log_error", message);
}

void error_handler(mapping map, int caught)
{
    string str = "[" + ctime(time()) + "]";

    str += sprintf("\n%O\n\n", map);

    if (caught)
        write("出错啦！详情记录在 error_handler 日志。");
    else
        printf("%s", map["error"]);

    write_file(LOG_DIR + "error_handler", str);
}

/**
 * valid applies
 */
int valid_seteuid(object ob, string euid)
{
    return 1;
}

int valid_socket(object caller, string func, mixed *info)
{
    return 1;
}

int valid_bind(object binder, object old_owner, object new_owner)
{
    return 1;
}
