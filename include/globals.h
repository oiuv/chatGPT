// 全局头文件

#define LOGIN_OB "/system/object/login"
#define USER_OB "/system/object/user"
#define VOID_OB "/system/object/void"

#define MOTD "/system/etc/MOTD" + random(3)
// 限制访问IP列表
#define DENIED_IPS "denied_ips.txt"
// 敏感词过滤列表
#define SENSITIVEWORDS "SensitiveWords.txt"
// OPENAI API密钥列表
#define OPENAI_API_KEY "OPENAI_API_KEY"

#define LOG_DIR "/log/"

/* inherit 模块 */
#define _EXTERNAL_CMD "/inherit/external_cmd"
