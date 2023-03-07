
    ███╗   ███╗██╗   ██╗██████╗  ██████╗ ██████╗ ████████╗
    ████╗ ████║██║   ██║██╔══██╗██╔════╝ ██╔══██╗╚══██╔══╝
    ██╔████╔██║██║   ██║██║  ██║██║  ███╗██████╔╝   ██║
    ██║╚██╔╝██║██║   ██║██║  ██║██║   ██║██╔═══╝    ██║
    ██║ ╚═╝ ██║╚██████╔╝██████╔╝╚██████╔╝██║        ██║
    ╚═╝     ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝        ╚═╝


> chatGPT MUD聊天室版(bbs.mud.ren)

直接发言提问，和chatGPT的会话内容仅自己可见

如果你要和在线网友聊天而不是和chatGPT会话，请输入`chat 聊天内容`

----

本项目使用LPC语言开发，前端功能使用[xterm.js](https://github.com/xtermjs/xterm.js)实现，样式使用[bulma](https://github.com/jgthms/bulma)框架。

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

> 因为 `v0.27.0` 在 openai cli 模式存在问题，当前版本改用 curl 调用API，无需安装openai

## 配置密钥

1. 配置OPENAI密钥：新建文件 `OPENAI_API_KEY`，在此文件中写入你的密钥，每行一个，每次回答时API会随机选择一个使用。
2. 配置CURL命令：修改文件 `config.cfg` 中 `external_cmd_1 : /usr/bin/curl` 为 curl 指令在你系统中的路径。
3. ~~配置OPENAI命令：修改文件 `config.cfg` 中 `external_cmd_3 : /usr/local/bin/openai` 为 openai 指令你系统中的路径。~~

## 启动项目

使用[FluffOS](https://github.com/fluffos/fluffos)驱动运行项目，注意使用最新版（需用到 `external_start` 函数）

```bash
driver config.cfg
```

> 经测试截止2月底的FLuffOS驱动在Windows系统下存在BUG，所以请使用Ubuntu系统

## 使用说明

### 提问

输入昵称登录后发文字可以直接向chatGPT提问，为避免浪费额度限制超过2个字的内容才发送给chatGPT。

> chatGPT回复的内容会自动备份到 `log/chatGPT.md` 中

### 聊天

如果只是和在线网友聊天请输入 `chat 聊天内容`。

### 提示

输入`who`可以查看目前在线的用户列表，更多提示见[tips.md](tips.md)

> 本项目功能未做任何限制，注意不要被人聊坏了，建议有兴趣的小伙伴自己完善
