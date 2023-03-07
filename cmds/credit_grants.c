inherit _EXTERNAL_CMD;
#include <ansi.h>

int main(object me, string arg)
{
    int CURL_CMD = 1;
    string key = element_of(read_lines("OPENAI_API_KEY"));
    if (__ARCH__ == "Microsoft Windows")
        CURL_CMD = 2;
    exec(CURL_CMD, ({"-s", "https://api.openai.com/dashboard/billing/credit_grants", "--header", "Authorization: Bearer " + key}));

    return 1;
}

protected void response(string result)
{
    mapping data = json_decode(result);
    string arg = sprintf("%sAPI额度：total_used : %f, total_available : %f\n", HIG "【chatGPT】" NOR, data["total_used"], data["total_available"]);

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
