#include <ansi.h>

int main(object me, string arg)
{
    mapping data = fetch_variable("data", me);
    string phone = data["mobile"];
    int code = data["verify_code"];

    if (!arg)
    {
        if (phone = fetch_variable("mobile", me))
        {
            write(HIY "你当前绑定的手机号码是：" + phone + "，如需修改请输入：verify 新号码\n" NOR);
            return 1;
        }
        write(BRED "请输入“" YEL " verify 手机号码 " NOR BRED "”根据提示完成认证，注意每次登录都需重新认证。" NOR "\n");
        return 1;
    }
    // 验证
    if (code && !pcre_match(arg, "^1[3-9][0-9]{9}$"))
    {
        if (to_int(arg) != code)
        {
            return notify_fail(HIR "验证码错误，请重试或重新发送！\n" NOR);
        }
        store_variable("mobile", phone, me);
        write(HIG "身份认证完成，现在开始你拥有自己的会话存档了，你可以随时使用 mailto 指令获取会话存档 💟\n" NOR);
        map_delete(data, "verify_code");
        map_delete(data, "mobile");

        return 1;
    }
    // 发送验证码
    else if (!pcre_match(arg, "^1[3-9][0-9]{9}$"))
    {
        return notify_fail(HIR "手机号码格式不正确！\n" NOR);
    }
    else
    {
        code = 1000 + random(9000);
        // 保存验证码
        data["verify_code"] = code;
        data["mobile"] = arg;
        me->sms();
        write(HIC "收到验证码后请输入“verify 验证码”完成验证。\n" NOR);
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式: verify [phone|code]

这个指令可以让你绑定手机号码或修改绑定的手机号码，手机号码为会话记录的识别凭据。

HELP);
    return 1;
}
