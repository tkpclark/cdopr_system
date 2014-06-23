#encoding:utf-8
import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import redis

def loadcode():
    
    #添加新增的
    sql = "select phone_number from wraith_blklist where status=0"
    tmp = mysql.queryAll(sql)
    
    #print "blklist loaded!"
    r = redis.StrictRedis(host='localhost', port=6379, db=2)
    #r.flushdb()
    for item in tmp:
        r.set(item['phone_number'],'1')
        #print r.get(item['code'])
        
        
    #去掉删除的
    sql = "select phone_number from wraith_blklist where status=2"
    tmp = mysql.queryAll(sql)
    
    #print "blklist loaded!"
    r = redis.StrictRedis(host='localhost', port=6379, db=2)
    #r.flushdb()
    for item in tmp:
        r.delete(item['phone_number'])
        

    print '成功黑名单信息！'
if __name__ == "__main__":
    loadcode()