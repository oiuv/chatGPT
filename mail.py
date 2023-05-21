import argparse
import smtplib
import sys
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication
# pip install python-dotenv
from dotenv import dotenv_values

def send_mail(recipient_email, attachment_filename):
    # 加载环境变量
    env = dotenv_values('.env')

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
    print('邮件发送成功❤️❤️❤️')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='发送和chatGPT的聊天记录到指定邮箱')
    parser.add_argument('recipient_email', help='收信邮箱地址')
    parser.add_argument('attachment_filename', help='聊天记录文件（手机号）')
    args = parser.parse_args()

    try:
        send_mail(args.recipient_email, args.attachment_filename)
    except:
        parser.print_help()
