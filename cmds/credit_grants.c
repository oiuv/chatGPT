inherit _EXTERNAL_CMD;
#include <ansi.h>

int main(object me, string arg)
{
    int CURL_CMD = 1;
    string key = json_decode(read_file("config.json"))["session_key"];
    if (__ARCH__ == "Microsoft Windows")
        CURL_CMD = 2;
    exec(CURL_CMD, ({"-s", "https://api.openai.com/dashboard/billing/credit_grants", "--header", "Authorization: Bearer " + key}));

    return 1;
}

protected void response(string result)
{
    mapping data = json_decode(result);
    string arg;
    if (data["error"])
        arg = sprintf("%serror：%s\n", HIG "【chatGPT】" NOR, data["error"]["message"]);
    else
        arg = sprintf("%sAPI额度：total_used : $%.2f, total_available : $%.2f\n", HIG "【chatGPT】" NOR, data["total_used"], data["total_available"]);

    shout(arg);
}

int help(object me)
{
    write(@HELP
指令格式 : credit_grants
指令说明:
    查询密钥额度信息
HELP);

    return 1;
}
