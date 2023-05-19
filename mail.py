import smtplib
import sys
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication
# pip install python-dotenv
from dotenv import dotenv_values
'''
MAIL_HOST=smtpdm.aliyun.com
MAIL_PORT=80
MAIL_USERNAME=mudGPT@mud.ren
MAIL_PASSWORD=12345
MAIL_FROM_ADDRESS=mudGPT@mud.ren
'''
# 加载环境变量
env = dotenv_values('.env')
# print(env)

# 获取脚本参数
recipient_email = sys.argv[1]
attachment_filename = sys.argv[2]

# 邮件内容
msg = MIMEMultipart()
msg['Subject'] = '您在mudGPT上的chatGPT会话记录'
msg['From'] = env['MAIL_FROM_ADDRESS']
msg['To'] = recipient_email
# 添加邮件正文
body = MIMEText("""
亲爱的用户：

邮件附件文件 chatGPT.txt 是您在 MudGPT 上和 ChatGPT 的所有聊天记录，您可以通过查看这些记录来回顾过去的交流并从中获取所需的信息。

如果您有任何问题或需要进一步的帮助，请随时在bbs.mud.ren与雪风联系，祝您使用愉快！😋

""")
msg.attach(body)
# 添加附件
with open('log/history/' + attachment_filename + '.txt', 'rb') as f:
    attachment = MIMEApplication(f.read())
    attachment.add_header('Content-Disposition',
                          'attachment',
                          filename='chatGPT.txt')
    msg.attach(attachment)
# 发送邮件
smtp_server = env['MAIL_HOST']
smtp_port = env['MAIL_PORT']
smtp_username = env['MAIL_USERNAME']
smtp_password = env['MAIL_PASSWORD']
smtp = smtplib.SMTP(smtp_server, smtp_port)
smtp.login(smtp_username, smtp_password)
smtp.sendmail(smtp_username, [msg['To']], msg.as_string())
smtp.quit()
