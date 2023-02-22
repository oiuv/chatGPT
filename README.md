                     __________  ______
      __ _  __ _____/ / ___/ _ \/_  __/
     /  ' \/ // / _  / (_ / ___/ / /
    /_/_/_/\_,_/\_,_/\___/_/    /_/
    ------------------------------

> chatGPT MUD聊天室版

直接发言提问，要求描述长度大于9个字符，问题和回答所有人可见

如果你只是和其它网友聊天而不想向chatGPT提问，请输入`chat 聊天内容`

----

![mudGPT](https://oss.mud.ren/images/mudGPT.png)

## 安装说明

1. 下载项目源码

    git clone https://github.com/oiuv/chatGPT.git

2. 安装OpenAI包

    pip install openai

## 配置密钥

1. 配置OPENAI密钥：新建文件 `OPENAI_API_KEY`，在此文件中写入你的密钥，每行一个，每次回答时API会随机选择一个使用。
2. 配置OPENAI命令：修改文件 `config.cfg` 中 `external_cmd_3 : /usr/local/bin/openai` openai指令路径为你系统中的。

## 启动项目

使用[FluffOS](https://github.com/fluffos/fluffos)驱动运行项目

    driver config.cfg

## 说明

本项目功能未做任何限制，注意不要被人聊坏了，建议有兴趣的小伙伴自己完善。
