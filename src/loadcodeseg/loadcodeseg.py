#encoding:utf-8
import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import redis

def loadcode():
    sql = "select * from wraith_code_segment"
    tmp = mysql.queryAll(sql)
    
    #print "blklist loaded!"
    r = redis.StrictRedis(host='localhost', port=6379, db=1)
    #r.flushdb()
    for item in tmp:
        r.set(item['code'],item['province']+"_"+item['area'])
        #print r.get(item['code'])

    print '成功加载号段信息！'
if __name__ == "__main__":
    loadcode()