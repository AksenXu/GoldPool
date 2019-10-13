import time

#https://www.runoob.com/python/python-date-time.html
t = time.time()
print(t, "=>", time.ctime())

localtime = time.localtime(time.time())
print("本地时间为 :", localtime)

localtime = time.asctime( time.localtime(time.time()) )
print("本地时间为 :", localtime)

# 格式化成2016-03-20 11:45:39形式
print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())) 
 
# 格式化成Sat Mar 28 22:24:24 2016形式
print(time.strftime("%a %b %d %H:%M:%S %Y", time.localtime()))
  
# 将格式字符串转换为时间戳
a = "Sat Mar 28 22:24:24 2016"
print(time.mktime(time.strptime(a,"%a %b %d %H:%M:%S %Y")))

import calendar
 
cal = calendar.month(2016, 1)
print("以下输出2016年1月份的日历:")
print(cal)

total = 0 # 这是一个全局变量
# 可写函数说明
def sum( arg1, arg2 ):
   #返回2个参数的和."
   total = arg1 + arg2 # total在这里是局部变量.
   print("函数内是局部变量 : ", total)
   return total
 
#调用sum函数
sum( 10, 20 )
print("函数外是全局变量 : ", total)
total = sum(10, 20)
print("函数外是全局变量 : ", total)


# 导入内置math模块
import math
content = dir(math)
#得到module里面的接口
print(content)


# 文件操作
# https://www.runoob.com/python/python-files-io.html

try:
    fh = open("./testfile", "w")
    print(fh.name)
    fh.write("这是一个测试文件，用于测试异常!!")
except IOError:
    print("Error: 没有找到文件或读取文件失败")
else:
    print("内容写入文件成功")
    fh.close()


# 文件目录
# https://www.runoob.com/python/os-file-methods.html

# 内置函数
# https://www.runoob.com/python/python-built-in-functions.html

# 正则表达式
# https://www.runoob.com/python/python-reg-expressions.html