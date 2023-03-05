inherit _EXTERNAL_CMD;

protected void response(string result)
{
    shout(result);
}

int main(object me, string arg)
{
    int CURL_CMD = 1;
    if (__ARCH__ == "Microsoft Windows")
        CURL_CMD = 2;

    exec(CURL_CMD, arg || "-h");

    return 1;
}
