#!/usr/bin/python3

"""
这是多行注释，使用双引号。
这是多行注释，使用双引号。
这是多行注释，使用双引号。
"""

#输出打印
print("Hello, World!")

#中文字符
print( "你好，世界" )

#获取用户输入字符
# str = input("按下 enter 键退出，其他任意键显示...\n")
# print("User input:", str)

#变量
counter = 100 # 赋值整型变量
miles = 1000.0 # 浮点型
name = "John" # 字符串
print(counter)
print(miles)
print(name)

#多变量初始化
a, b, c = 1, 2, "john"

#复数
vcomplex = 1 + 2j
print("complex :", vcomplex)

#字符串
str="test string for cut"
str2=str[1:5] + "combine"  #字符串拼接
print("cut ", str, " => ", str2)    #print多个变量

str3=str[1::2]  #采用步长2来获取字符串
print("cut ", str, " => ", str3)

#列表
list = [ 'runoob', 786 , 2.23, 'john', 70.2 ]
tinylist = [123, 'john']
 
print(list)               # 输出完整列表
print(list[0])            # 输出列表的第一个元素
print(list[1:3])          # 输出第二个至第三个元素 
print(list[2:])           # 输出从第三个开始至列表末尾的所有元素
print(tinylist * 2)       # 输出列表两次
print(list + tinylist)    # 打印组合的列表

#if
a = 3
if a == 1 :
    print("a == 1")
elif a == 2 :
    print("a == 2")
else :
    print("a == ", a)

#字典
dict = {}
dict['one'] = "This is one"
dict[2] = "This is two"
 
tinydict = {'name': 'john','code':6734, 'dept': 'sales'}
 
print(dict['one'])          # 输出键为'one' 的值
print(dict[2])              # 输出键为 2 的值
print(tinydict)             # 输出完整的字典
print(tinydict.keys())      # 输出所有键
print(tinydict.values())    # 输出所有值
for (k,v) in tinydict.items():
    print(k, "=>", v)

for k in tinydict.keys():
    print(k, "=>", tinydict[k])

"""
内嵌函数
int(x [,base])

将x转换为一个整数

long(x [,base] )

将x转换为一个长整数

float(x)

将x转换到一个浮点数

complex(real [,imag])

创建一个复数

str(x)

将对象 x 转换为字符串

repr(x)

将对象 x 转换为表达式字符串

eval(str)

用来计算在字符串中的有效Python表达式,并返回一个对象

tuple(s)

将序列 s 转换为一个元组

list(s)

将序列 s 转换为一个列表

set(s)

转换为可变集合

dict(d)

创建一个字典。d 必须是一个序列 (key,value)元组。

frozenset(s)

转换为不可变集合

chr(x)

将一个整数转换为一个字符

unichr(x)

将一个整数转换为Unicode字符

ord(x)

将一个字符转换为它的整数值

hex(x)

将一个整数转换为一个十六进制字符串

oct(x)

将一个整数转换为一个八进制字符串
"""


#幂
print("10 * 10 = ", 10**2)

#整除
print("10 / 3 = ", 10/3)
print("10 / 3 = ", 10//3)


#逻辑运算符
"""
and	x and y	布尔"与" - 如果 x 为 False，x and y 返回 False，否则它返回 y 的计算值。	(a and b) 返回 20。
or	x or y	布尔"或"	- 如果 x 是非 0，它返回 x 的值，否则它返回 y 的计算值。	(a or b) 返回 10。
not	not x	布尔"非" - 如果 x 为 True，返回 False 。如果 x 为 False，它返回 True。	not(a and b) 返回 False
"""
a, b = 10, 20
print( "a and b = " , a and b)
print( "a or b = " , a or b)
print( "not a = " , not a)

#in 运算符
a = 10
b = 20
list = [10, 2, 3, 4, 5 ];
 
if ( a in list ):
   print("1 - 变量 a 在给定的列表中 list 中")
else:
   print("1 - 变量 a 不在给定的列表中 list 中")
 
if ( b not in list ):
   print("2 - 变量 b 不在给定的列表中 list 中")
else:
   print("2 - 变量 b 在给定的列表中 list 中")

if a is list[0]:
   print("3 - 变量 a is list[0]")
else:
   print("3 - 变量 a is not list[0]")
     
count = 0
while (count < 9):
   print('The count is:', count)
   count = count + 1
   if count == 5 :
       break
   else :
        continue
   pass


#class
class Employee:
   '所有员工的基类'
   empCount = 0     #所有类实例的共有变量，相当于static成员变量
 
   def __init__(self, name, salary, age):
      self.name = name
      self.salary = salary
      self.age = age
      Employee.empCount += 1
   
   def displayCount(self):
     print("Total Employee: ", Employee.empCount)
 
   def displayEmployee(self):
      print("Name : ", self.name,  ", Salary: ", self.salary, ", Age: ", self.age)

employee = Employee("user", 1000, 20)
employee.displayCount()
employee.displayEmployee()
employee.name = "xxxx"
employee.displayEmployee()

print("has Age:", hasattr(employee, 'age'))
delattr(employee, 'age')    #删除成员变量
print("has Age:", hasattr(employee, 'age'))

print("name:", employee.__class__, employee.__module__)


class EmployeeSub(Employee):
    def __init__(self):
        self.sub = 1
        Employee.__init__(self, "test", 100, 10)

    def displayEmployee(self):
        print("EmployeeSub")
        Employee.displayEmployee(self)

employeeSub = EmployeeSub()
employeeSub.displayEmployee()

# __private 双下划线开头的变量，表示是类的私有变量，只能在类内部使用
"""
__foo__: 定义的是特殊方法，一般是系统定义名字 ，类似 __init__() 之类的。
_foo: 以单下划线开头的表示的是 protected 类型的变量，即保护类型只能允许其本身与子类进行访问，不能用于 from module import *
__foo: 双下划线的表示的是私有类型(private)的变量, 只能是允许这个类本身进行访问了。
"""


