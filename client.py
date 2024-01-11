import requests  
  
# 构造要发送的JSON数据  
data = {  
    "key1": "value1",  
    "key2": "value2"  
}  
  
# 发送POST请求到localhost:8080，并传递JSON数据  
response = requests.post('http://localhost:8080', json=data)  
  
# 打印响应  
print(response.text)