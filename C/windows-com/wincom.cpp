#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static HANDLE hComHandle = 0; //全局变量，串口句柄
static int writeFile2Com(const char *file)
{
#if 0
    printf("writeFile2Com: 准备写入文件到串口: %s\n", file);
    //打开已经存在的文件，读取内容
    HANDLE hFileRead = CreateFileA(file,                  //name
                                   GENERIC_READ,          //以读方式打开
                                   FILE_SHARE_READ,       //可共享读
                                   NULL,                  //默认安全设置
                                   OPEN_EXISTING,         //只打开已经存在的文件
                                   FILE_ATTRIBUTE_NORMAL, //常规文件属性
                                   NULL);                 //无模板
                                                          //打开文件是否成功
    if (hFileRead == INVALID_HANDLE_VALUE)
    {
        printf("writeFile2Com 打开文件 %s 失败: %d\n", file, GetLastError());
        return -1;
    }

    unsigned int liFileSize;
    if (!GetFileSize(hFileRead, &liFileSize))
    {
        printf("writeFile2Com 获取文件大小失败： %d\n", GetLastError());
        CloseHandle(hFileRead);
        return -1;
    }
    else
    {
        printf("writeFile2Com 文件 %s 大小 %d\n", file, liFileSize);
    }

    char *fileContent = (char *)malloc(liFileSize + 1);
    fileContent[liFileSize] = 0;
    unsigned int dwReadedSize;
    if (!ReadFile(hFileRead,     //读取文件句柄
                  fileContent,   //存储读取文件内容
                  liFileSize,    //读取的大小(字节)
                  &dwReadedSize, //实际读取的大小
                  NULL))         //不使用Overlapped
    {
        printf("writeFile2Com 读取文件 %s 错误: %d\n", file, GetLastError());
        goto end;
    }

    printf("writeFile2Com 读取文件 %s %d字节", file, dwReadedSize);

    printf("writeFile2Com 开始写入串口", file, dwReadedSize);

    {
        //将数据写入文件
        unsigned int dwWritedDateSize = 0;
        if (!WriteFile(hComHandle, fileContent, dwReadedSize, &dwWritedDateSize, NULL))
        {
            printf("写文件失败： %d\n", GetLastError());
        }
        else
        {
            printf("写文件成功，写入%d字节。\n", dwWritedDateSize);
        }
    }
end:
    free(fileContent);
    CloseHandle(hFileRead);

#else

    FILE *fp = fopen(file, "r");
    if(NULL == fp) {
        printf("failed to open file %s\n", file);
        return -1;
    }

    //等待设备启动完成
    Sleep(5000);
    while(!feof(fp))
    {
        char szTest[256] = { 0 };
        fgets(szTest, sizeof(szTest), fp);
        if(szTest[0] == '#') {
            continue;
        }

        unsigned int dwWritedDateSize = 0;
        //先写10个回车, 防止设备休眠
        const char* dummyStr = "\n\n\n\n\n\n\n\n\n\n";
        WriteFile(hComHandle, dummyStr, strlen(dummyStr), &dwWritedDateSize, NULL);
        if (!WriteFile(hComHandle, szTest, strlen(szTest), &dwWritedDateSize, NULL))
        {
            printf("写串口失败： %d\n", GetLastError());
        }
        else
        {
            WriteFile(hComHandle, dummyStr, strlen(dummyStr), &dwWritedDateSize, NULL);
            printf("写串口成功，写入内容: %s\n", szTest);
        }
        FlushFileBuffers(hComHandle);
        Sleep(100);
    }
 
    fclose(fp);
#endif

    printf("writeFile2Com: 写入完毕\n");
    return 0;
}

static int waitForTargetStr(const char *str)
{
    printf("waitForTargetStr: 读取串口输入, 等待字符串: %s\n", str);
    int buffersize = 1024 * 1024;
    char *buffer = (char *)malloc(buffersize);
    int offset = 0;
    while (1)
    {
        unsigned int len = 0;
        BOOL bReadStatus = ReadFile(hComHandle, buffer + offset,
                                    buffersize - offset, &len, NULL);
        if (!bReadStatus)
        {
            printf("waitForTargetStr: 读取串口失败 %d \n", GetLastError());
        }
        else
        {
            offset += len;
            // printf("waitForTargetStr: len %d offset %d buffer %s\n", len, offset, buffer);
        }

        if (strstr(buffer, str))
        {
            printf("waitForTargetStr: 读取字符串 %s 成功", str);
            break;
        }

        if (offset == buffersize)
        {
            offset = 0;
        }
    }

    free(buffer);
    printf("waitForTargetStr: 读取字符串 %s 完成\n", str);
    return 0;
}

static void configComm(int baudrate)
{
    DCB dcb;
    printf("configComm: 开始配置串口\n");
    dcb.DCBlength = sizeof(DCB);
    GetCommState(hComHandle, &dcb); // 获得端口默认设置
    dcb.BaudRate = baudrate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = (BYTE)ONESTOPBIT;
    SetCommState(hComHandle, &dcb);

    printf("configComm OK: baudrate %d\n", baudrate);
    return;
}

static void usage(const char *name)
{
    printf("usage: \n");
    printf("%s comX baudrate targetStr writeFile: 从comX读数据, 读到targetStr以后, 将writeFile通过串口写下去\n", name);
    return;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        usage(argv[0]);
        return 0;
    }

    const char *comPort = argv[1];
    int baudrate = atoi(argv[2]);
    const char *targetStr = argv[3];
    const char *writeFile = argv[4];
    printf("%s com: %s targetStr: %s writeFile: %s\n", argv[0], comPort, targetStr, writeFile);
    hComHandle = CreateFile(comPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hComHandle == INVALID_HANDLE_VALUE)
    {
        printf("打开串口: %s 失败, 请检查串口是否存在\n", comPort);
        return -1;
    }
    else
    {
        printf("打开串口: %s 成功\n", comPort);
    }

    configComm(baudrate);

    int writeTimes = 0;
    while (1)
    {
        printf("\n\n\n\n\n");
        printf("=================================================\n");
        printf("准备第%d次读写串口: %s\n", writeTimes++, comPort);
        printf("=================================================\n");

        PurgeComm(hComHandle, 0xF);
        waitForTargetStr(targetStr);
        writeFile2Com(writeFile);
    }

    return 0;
}
