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
// 记录不同格式的日志1+2+3
#define LOG_Q 1 // 记录提问到chatGPT.log
#define LOG_A 2 // 记录回答到chatGPT.md
#define LOG_R 3 // 记录数据到chatGPT.txt

/* inherit 模块 */
#define _EXTERNAL_CMD "/inherit/external_cmd"
