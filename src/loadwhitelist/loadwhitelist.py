#encoding:utf-8
import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import redis

def loadcode():
    
    #添加新增的
    sql = "select * from wraith_whitelist"
    tmp = mysql.queryAll(sql)
    
    #print "blklist loaded!"
    r = redis.StrictRedis(host='localhost', port=6379, db=3)
    r.flushdb()
    for item in tmp:
        r.set(item['phone_number'],'1')
        #print r.get(item['code'])

    print '成功白名单信息！'
if __name__ == "__main__":
    loadcode()