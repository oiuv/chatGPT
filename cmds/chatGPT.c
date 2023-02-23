inherit _EXTERNAL_CMD;

#include <ansi.h>

// 根据config.cfg中external_cmd_x指定
#define OPENAI_CMD 3

void prompt(string prompt)
{
    // 读取LIB根目录下的OPENAI_API_KEY文件中配置的随机密钥
    string key = element_of(read_lines("OPENAI_API_KEY"));
    // usage: openai [-h] [-v] [-b API_BASE] [-k API_KEY] [-o ORGANIZATION] {api,tools,wandb} ...
    exec(OPENAI_CMD, ({"-k", key, "api", "completions.create", "-m", "text-davinci-003", "-M", "3721", "-p", prompt }));
}

protected void response(string result)
{
    string arg = HIG "【chatGPT】" NOR HIC + result + NOR "\n";
    // 读取LIB根目录下tips.md文件中的随机提示
    string tips = "\n-提示" + element_of(read_lines("tips.md")) + "\n";
    shout(arg + tips);
    // 备份问答
    write_file(LOG_DIR + "chatGPT.md", "## " + result + "\n\n");
}

int main(object me, string arg)
{
    prompt(arg); // 直接向chatGPT提问
    return notify_fail(HIM "【提示】" NOR HIG "chatGPT思考中……" NOR "\n"); // 因为还没获取结果，给未成功响应
}

int help(object me)
{
    write(@HELP
指令格式 : chatGPT <prompt>

向chatGPT提问，注意chatGPT回答需要时间，请不要重复提问。

HELP );
    return 1;
}
