
    ███╗   ███╗██╗   ██╗██████╗  ██████╗ ██████╗ ████████╗
    ████╗ ████║██║   ██║██╔══██╗██╔════╝ ██╔══██╗╚══██╔══╝
    ██╔████╔██║██║   ██║██║  ██║██║  ███╗██████╔╝   ██║
    ██║╚██╔╝██║██║   ██║██║  ██║██║   ██║██╔═══╝    ██║
    ██║ ╚═╝ ██║╚██████╔╝██████╔╝╚██████╔╝██║        ██║
    ╚═╝     ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝        ╚═╝


> chatGPT MUD聊天室版(bbs.mud.ren)

直接发言提问，**和chatGPT的会话内容仅自己可见**

如果你要和在线网友聊天而不是和chatGPT会话，请输入`chat 聊天内容`

----

本项目使用LPC语言开发，前端功能使用[xterm.js](https://github.com/xtermjs/xterm.js)实现，样式使用[bulma](https://github.com/jgthms/bulma)框架。

> 因为2023年3月1日开放gpt-3.5的API后接口国内无法访问，请使用魔法突破封印

![mudGPT](https://oss.mud.ren/images/mudGPT.png)

## 安装说明

1. 下载项目源码

```bash
git clone https://github.com/oiuv/chatGPT.git
# 国内镜像
git clone https://gitee.com/oiuv/chatGPT.git
```

2. 安装OpenAI包（可选|备用）

```bash
pip install openai
```

> 项目使用 curl 调用API，无需安装openai

## 配置密钥

1. 配置OPENAI密钥：复制 `config_example.json` 为 `config.json`，在此文件中配置你的密钥和其它参数。
2. 配置CURL命令：修改文件 `config.cfg` 中 `external_cmd_1 : /usr/bin/curl` 为 curl 指令在你系统中的路径，如果你是windows系统，请修改`external_cmd_2`。
3. 配置Python命令：修改文件 `config.cfg` 中 `external_cmd_3`为你系统中python指令的路径，如果是windows系统请修改`external_cmd_4`。

## 启动项目

使用[FluffOS](https://github.com/fluffos/fluffos)驱动运行项目

1. 第一次请运行`build.sh`脚本编译驱动

```bash
./build.sh
```

2. 运行`run.sh`脚本启动服务

```bash
./run.sh
```

3. 客户端连接

- 浏览器访问：http://127.0.0.1:8080
- 客户端访问：telnet 127.0.0.1 6666

> windows系统请自己编译或访问bbs.mud.ren下载已编译好的驱动

## 使用说明

### 提问

输入昵称登录后发文字可以直接向chatGPT提问。

如果你需要为chatGPT指定角色定位，请使用`setGPT`指令（区分大小定），如：

    setGPT 你来充当翻译家，你的目标是把任何语言翻译成中文

注意：如果直接输入`setGPT`或`setGPT -d`则取消对chatGPT的角色限制。

默认会话有上下文关联，如果你要发起新的会话，可以输入`chatGPT -d`重置会话。

> chatGPT回复的内容会自动备份到 `log/chatGPT.md` 中

### 聊天

如果只是和在线网友聊天请输入 `chat 聊天内容`。

### 存档

当玩家使用手机号码完成认证后，可以记录个人会话并发送至指定邮箱，注意这里需要配置短信和邮件服务。

### 提示

输入`who`可以查看目前在线的用户列表，更多提示见[tips.md](tips.md)

> 本项目功能未做任何限制，注意不要被人聊坏了，建议有兴趣的小伙伴自己完善
