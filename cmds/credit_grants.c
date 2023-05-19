inherit _EXTERNAL_CMD;
#include <ansi.h>

int main(object me, string arg)
{
    int CURL_CMD = 1;
    string *keys = config("session_keys");
    string proxy = config("proxy");
    string key;
    string *cURL = ({"-s", "https://api.openai.com/dashboard/billing/credit_grants", "--header"});

    if (to_int(arg) < 1 || to_int(arg) > sizeof(keys))
        key = element_of(keys);
    else
        key = keys[to_int(arg) - 1];

    cURL += ({"Authorization: Bearer " + key});

    if (__ARCH__ == "Microsoft Windows")
        CURL_CMD = 2;
    if (proxy)
        cURL += ({"-x", proxy});

    // debug(cURL);
    external_cmd(CURL_CMD, cURL);

    return 1;
}

protected void response(string result)
{
    mapping data = json_decode(result);
    string arg;
    // debug(result);
    if (data["error"])
        arg = sprintf("%serror：%s\n", HIG "【chatGPT】" NOR, data["error"]["message"]);
    else if (data["grants"])
    {
        mapping grants = data["grants"]["data"][0];
        arg = sprintf("%O", grants);
        arg = sprintf("%s截止 %s 额度信息：\nid : %s\n账户总额 : $%.2f\n消费额度 : $%.2f\n账户余额 : $%.2f\n生效时间 : %s\n过期时间 : %s\n", HIG "【chatGPT】" NOR, ctime(), grants["id"], data["total_granted"], data["total_used"], data["total_available"], ctime(to_int(grants["effective_at"])), ctime(to_int(grants["expires_at"])));
    }
    else
        arg = result;

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
