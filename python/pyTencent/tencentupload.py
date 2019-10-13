#!/usr/bin/env python
# -*- coding: utf-8 -*-
__author__ = 'zhihuizhang'
import traceback
import requests
URI = "/device-image-xxx/353211086127285/1629150443522494487/1564992198974.jpg"
HOST = "ishare-10003415.cos.ap-shanghai.myqcloud.com"
AUTH = 'q-sign-algorithm=sha1&q-ak=AKIDRhMNXamF53w6lIGXPJRK7eOSmLtsj0fG&q-sign-time=1564992148;1565078608&q-key-time=1564992148;1565078608&q-header-list=&q-url-param-list=&q-signature=84a473345e8132d4cfa85b0336b0bd62a7ff9a01'
URL = "http://" + HOST + URI
headers = {
    "Host": HOST,
    "Authorization": AUTH
}
with open(r"./test.jpg", "rb") as f:
    data = f.read()
ret = requests.put(URL, data=data, headers=headers)
print (ret.status_code)
print (requests.codes.ok)
print (ret.headers)
