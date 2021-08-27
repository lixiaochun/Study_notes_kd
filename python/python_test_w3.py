print("hello world!!!")
x = 5
if x > 2:
    print("five is bigger than two.")
print(type(x))
y = range(6)
y = bool(-1)
y = bytes(3)
print(y)

###### random
import random
print(random.randrange(1,10))
print(random.random())

###### 类型转换
z = float("3")
print(z)

###### 长注释
a = """Python is a widely used general-purpose, high level programming language.
It was initially designed by Guido van Rossum in 1991
and developed by Python Software Foundation.
It was mainly developed for emphasis on code readability,
and its syntax allows programmers to express concepts in fewer lines of code."""
print(a)

######字符串
strTitle = " 字符串 "
print(strTitle.center(30,"#"))
a = " Hello, World!!!"
print(a[1])
print(a[2:5])  #not included five
print(a[-5:-2])
print(len(a))
print(a + " I am csquan")

print(a.replace("World","Kitty"))
print(a.split(","))
print(a.strip())
print(a.casefold()) #lower

b = "llo" in a
print(b)

###### 字符串
age = 29
txt = "My name is csquan, and I am {}"
print(txt.format(age))   # {} 为占位符

quantity = 5
itemno = 789
price = 24.36
myorder = "I want to pay {2} dollars for {0} pieces of item {1}."
print(myorder.format(quantity, itemno, price))  #占位符可添加索引值指定参数


###### 布尔
strTitle = " 布尔 "
print(strTitle.center(30,"#"))
print(bool("")) #空值 布尔数值为false


###### 运算符
strTitle = " 运算符 "
print(strTitle.center(30,"#"))
print(15 // 2)  #地板除（取整除）
print(2 ** 10)  #幂

x = 5
print(x > 3 and x < 10) # and/or/not 与/或/非, 大写为位运算

x = ["apple", "banana"]
y = ["apple", "banana"]
z = x
print(x is z)
print(x is y)
print(x is not z)
print(x is not y)


 ### 位运算
'''

& AND 与
| OR 或

'''

###### 列表
strTitle = " 列表[] "
print(strTitle.center(30,"#"))
thislist = ["apple", "banana", "cherry"]
#thislist = list(("apple", "banana", "cherry")) # 请注意双括号
print(thislist)
print("索引访问1：" + thislist[1])
print("负索引访问-1：" + thislist[-1])
print("索引范围[0:2],排除2：")
print(thislist[0:2])
for x in thislist:
  print(x)
if "apple" in thislist:
    print("Yes, 'apple' is in the fruits list")
print(len(thislist))
thislist.append("orange")
print(thislist)
thislist.insert(1,"orange")
print(thislist)
thislist.remove("orange") #delete first item
print(thislist)
thislist.pop()
print(thislist)
del thislist[0]
print(thislist)
thislist.clear() # del thislist,删除列表；clear 仅清空
print(thislist)

thislist = ["apple", "banana", "cherry"]
mylist = thislist.copy()  #copy 复制
#myiist = list(thislist)  #内建复制
print(mylist)

list1 = ["a", "b" , "c"]
list2 = [1, 2, 3]

#list3 = list1 + list2  #end add
#print(list3)
for x in list2:
    list1.append(x)
#list1.extend(list2)  #end add
print(list1)

#reverse()  颠倒列表的顺序
#sort()  对列表进行排序


###### 元组
strTitle = " 元组() "
print(strTitle.center(30,"#"))
thistuple = ("apple", "banana", "cherry")  #单项元组，须加逗号("apple",)
#thistuple = tuple(("apple", "banana", "cherry")) # 构造函数，请注意双括号
print(thistuple)
print(thistuple[1])  #且支持索引范围访问
#元组不可更改，但可完全删除元组  del thistuple
#更改元组值方法：将元组转换为列表，更改列表，然后将列表转换回元组
x = ("apple", "banana", "cherry")
y = list(x)
y[1] = "kiwi"
x = tuple(y)
print(x)
tuple1 = ("a", "b" , "c")
tuple2 = (1, 2, 3)
tuple3 = tuple1 + tuple2
print(tuple3)  #合并两个元组
x = tuple3.count(3)  #指定值出现次数
print(x)
x = tuple3.index(3)  #指定值索引
print(x)


###### 集合
strTitle = " 集合{} "
print(strTitle.center(30,"#"))
thisset = {"apple", "banana", "cherry"}  #无序 无索引 不重复
print(thisset)
thisset.add("orange") #逐个添加
thisset.update(["mango", "grapes"]) #多个添加
print(thisset)
thisset.remove("banana")  #删除
thisset.discard("orange") #删除 若项目不存在 不报错
print(thisset)
x = thisset.pop() #删除最后一项,不确定
print(x)
print(thisset)
#thisset.clear()
#del thisset
set1 = {"a", "b" , "c"}
set2 = {1, 2, 3}
set3 = set1.union(set2)  # or, set1.update(set2)
#联合不保留重复项
print(set3)

####### 字典
strTitle = " 字典{:} "
print(strTitle.center(30,"#"))
thisdict =	{
  "brand": "Porsche",
  "model": "911",
  "year": 1963
}
#thisdict = dict(brand="Porsche", model="911", year=1963)
#关键字非字符串字面量
#等号非冒号来赋值
print(thisdict)
print(thisdict["model"])  #获取
print(thisdict.get("model"))  #获取
thisdict["year"] = 2021
for x in thisdict.values():
    print(x)  #所有字典的值打印
for x, y in thisdict.items():
    print(x, y)  #遍历键和值
thisdict.pop("model")  #指定删除  == del thisdict["model"]
thisdict.popitem()  #删除最后插入的项目，3.7之前的版本随机删除
print(thisdict)

myfamily = {
  "child1" : {
    "name" : "Phoebe Adele",
    "year" : 2002
  },
  "child2" : {
    "name" : "Jennifer Katharine",
    "year" : 1996
  },
  "child3" : {
    "name" : "Rory John",
    "year" : 1999
  }
}  #内建嵌套
child1 = {
  "name" : "Phoebe Adele",
  "year" : 2002
}
child2 = {
  "name" : "Jennifer Katharine",
  "year" : 1996
}
child3 = {
  "name" : "Rory John",
  "year" : 1999
}

myfamily = {
  "child1" : child1,
  "child2" : child2,
  "child3" : child3
}  #字典嵌套
thisdict = dict(brand="Porsche", model="911", year=1963)#构造函数
#请注意，关键字不是字符串字面量
#请注意，使用了等号而不是冒号来赋值
## items(),keys(),values() 获取到的列表会根据字典变化而刷新

####### 条件
strTitle = " 条件 if else "
print(strTitle.center(30,"#"))
a = 200
b = 66
if b > a:
  print("b is greater than a")
elif a == b:
  print("a and b are equal")
else:
  print("a is greater than b")

print('A') if a > b else print('B')  #简写if else形式
print('a') if a > b else print('=') if a == b else print('b') #简写多个else语句
# and or  逻辑运算组合使用

if b > a: pass #无内容的分支，用pass 代替避免错误

####### while
strTitle = " while "
print(strTitle.center(30,"#"))
i = 0
while i < 6:
  i += 1
  if i == 5 or i == 3 or i == 1:
    continue  #break
  print(i)
else:  # while 不成立可再执行一次代码块
 print("i is no longer less than six")

####### for
strTitle = " for "
print(strTitle.center(30,"#"))
#for . in . 类似于迭代关系，无需初始化迭代变量
for x in range(5):  #0~4，默认起始值为0可指定；不包括5；默认增量为1可指定
  print(x)
else:
  print("Finally finished!")

####### 函数
strTitle = " 函数 "
print(strTitle.center(30,"#"))
def my_function(country = "China"):  # def 关键字定义函数，参数可设默认值
  print("I am from " + country)
my_function()
my_function("Sweden")

def my_function(child3, child2, child1):  #key=value 指定发送参数
  print("The youngest child is " + child3)
  #return 需要返回值可自行加上
my_function(child1 = "Phoebe", child2 = "Jennifer", child3 = "Rory")

def my_function(*kids):  #不清楚参数数目，可在参数名称前添加*
    if len(kids) > 0:  print("The youngest child is " + kids[len(kids)-1])
my_function("Phoebe", "Jennifer", "Rory")

def tri_recursion(k):  #递归
  if(k>0):
    result = k+tri_recursion(k-1)
    print(result)
  else:
    result = 0
  return result

print("\nRecursion Example Results")
tri_recursion(3)

####### Lambda
strTitle = " Lambda "
print(strTitle.center(30,"#"))
## lambda 函数可接受任意数量的参数，但只能有一个表达式
x = lambda a, b, c : a + b + c
print(x(5, 6, 2))

## 把 lambda 用作另一个函数内的匿名函数时，会更好地展现 lambda 的强大能力
def myfunc(n):
  return lambda a : a * n

mydoubler = myfunc(2)
mytripler = myfunc(3)

print(mydoubler(11)) 
print(mytripler(11))

####### 类
strTitle = " 类 "
print(strTitle.center(30,"#"))
class Person:
  def __init__(mysillyobject, name, age):
    mysillyobject.name = name
    mysillyobject.age = age

  def myfunc(abc):
    print("Hello my name is " + abc.name)

p1 = Person("Bill", 63)
p1.myfunc()

####### 继承
strTitle = " 继承 "
print(strTitle.center(30,"#"))

class Person:
  def __init__(self, fname, lname):
    self.firstname = fname
    self.lastname = lname

  def printname(self):
    print(self.firstname, self.lastname)
    
class Student(Person):
  def __init__(self, fname, lname, year):  #添加新内容
    #Person.__init__(self, fname, lname) #保留父类的继承
    super().__init__(fname, lname) #继承父类所有方法和属性
    self.graduationyear = year  #添加属性

  def welcome(self):
    print("Welcome", self.firstname, self.lastname,
          "to the class of", self.graduationyear)
x = Student("Elon", "Musk", 2019)
x.welcome()

####### 迭代
strTitle = " 迭代 "
print(strTitle.center(30,"#"))

mytuple = ("apple", "banana", "cherry")
myit = iter(mytuple) # 所有容器类对象均有获取迭代器iter()方法

print(next(myit))
print(next(myit))
print(next(myit))

## 创建迭代器
class MyNumbers:
  def __iter__(self):
    self.a = 1
    return self

  def __next__(self):
    if self.a <= 4:
      x = self.a
      self.a += 1
      return x
    else:
      raise StopIteration #迭代器中止

myclass = MyNumbers()
myiter = iter(myclass)

for x in myiter:
  print(x)

####### 模块
strTitle = " 模块 "
print(strTitle.center(30,"#"))

import mymodule as mx  #重命名模块
a = mx.person1["age"]
print(a)
print(dir(mx))

from mymodule import person1  #模块中导入，引用时无需使用模块名称
print(person1["age"])

####### 日期
strTitle = " 日期 "
print(strTitle.center(30,"#"))

import datetime
x = datetime.datetime.now()
print(x)  #年月日 时分秒 微秒
print(x.year)  #年
print(x.strftime("%x"))  #weekday
print(x.strftime("%Y-%m-%d %H:%M:%S"))  #年月日 时分秒

####### JSON
strTitle = " JSON "
print(strTitle.center(30,"#"))

import json
#json 转 python -- json.loads()
x =  '{"name":"Bill", "age":63, "city":"Seatle"}'
y = json.loads(x)
print(y["age"])

#python 转json  -- json.dumps()
x = {
  "name": "Bill",
  "age": 63,
  "city": "Seatle"
}
y =  json.dumps(x, indent = 2)#indent定义缩进数，便于阅读
print(y)

#python 类型转 json
print(json.dumps({"name": "Bill", "age": 29}))
print(json.dumps(["apple", "bananas"]))
print(json.dumps(("apple", "bananas")))
print(json.dumps("hello"))
print(json.dumps(42))
print(json.dumps(31.76))
print(json.dumps(True))
print(json.dumps(False))
print(json.dumps(None))

####### RegEx
strTitle = " RegEx "
print(strTitle.center(30,"#"))
import re
txt = "China is a 3 great country"
x = re.search("^China.*country$", txt)
print("Yes.") if (x) else print("No.")

str = "ai,The rain in Spain falls mainly in the plain!"

#Find all lower case characters alphabetically between "a" and "m":
x = re.findall("[a-h]", str)
print(x)

#Check if the string contains "ai" followed by 0 or more "n" characters:
x = re.findall("ain*", str)
print(x)

#Check if the string contains "a" followed by exactly two "l" characters:
x = re.findall("al{2}", str)
print(x)

#Check if the string contains either "falls" or "stays":
x = re.findall("falls|stays", str)
print(x)

x = re.search(r"\bSpain", str)
print(x.group())
del x

####### Try Except
strTitle = " Try Except "
print(strTitle.center(30,"#"))

try:
  print(x)
except NameError:
  print("Variable x is not defined")
except:
  print("Something else went wrong")
else:
  print("Nothing went wrong")
finally:
  print("The 'try except' is finished")

## raise 关键字用于引发异常
x = "hello"
if not type(x) is int:
    #raise TypeError("Only integer are allowed")
    pass

####### 命令行输入
strTitle = " input "
print(strTitle.center(30,"#"))
"""
print("Enter your name:")
x = input()
print("Hello ", x)
"""
####### 字符串格式化
strTitle = " 字符串格式化 "
print(strTitle.center(30,"#"))

quantity = 3
itemno = 567
price = 52
#myorder = "I want {} pieces of item number {} for {:.2f} dollars."
myorder = "I want {0} pieces of item number {1} for {2:.2f} dollars."  #索引号(可重复)
print(myorder.format(quantity, itemno, price))

myorder = "I have a {carname}, it is a {model}."
print(myorder.format(carname = "Porsche", model = "911"))  ##命名索引，须使用名称传值


####### 文件打开
strTitle = " 文件打开 "
print(strTitle.center(30,"#"))

f = open("demofile.txt", "r")
#print(f.read())  #读取所有
#print(f.read(5))  #指定字符数
print(f.readline())   #读行
f.close()
"""
f = open("demofile2.txt", "w")  #覆盖写入，a 追加
f.write("Now the file has more content!")
f.close()
f = open("demofile2.txt", "r")
print(f.read())
f.close()

import os
if os.path.exists("demofile2.txt"):
  os.remove("demofile2.txt")#删除
else:
  print("The file does not exist")
"""
#os.rmdir()  #删除文件夹

####### NumPy
strTitle = " NumPy "
print(strTitle.center(30,"#"))

import numpy as np
print("NumPy Version:"+ np.__version__)

arr = np.array([1, 2, 3, 4, 5]) 
print(arr)
print(type(arr))








