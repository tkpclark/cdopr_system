#encoding:utf-8
import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import redis

def loadcode():
    
    #添加新增的
    
    sql1 = "update wraith_blklist set status=3 where status=0 limit 10000" #status=3 正在导入
    sql2 = "select phone_number from wraith_blklist where status=3"
    sql3 = "update wraith_blklist set status=1 where status=3"
   
    #print "blklist loaded!"
    r = redis.StrictRedis(host='localhost', port=6379, db=2)
    #r.flushdb()
    
    try:
        while True:
            #print sql1
            mysql.query(sql1)
            #print sql2
            tmp = mysql.queryAll(sql2)
            if(mysql.rowcount()==0):
                break
            for item in tmp:
                r.set(item['phone_number'],'1')
                #print r.get(item['code'])
            #print sql3
            mysql.query(sql3)
            #print '\n'
    except Exception, e:
        print e
            
    #去掉删除的
    sql4 = "update wraith_blklist set status=4 where status=2 limit 10000" #status=3 正在导入
    sql5 = "select phone_number from wraith_blklist where status=4"
    sql6 = "delete from wraith_blklist where status=4"
    
    try:
        while True:
            #print sql3
            mysql.query(sql4)
            tmp = mysql.queryAll(sql5)
            if(mysql.rowcount()==0):
                break
 
    #r.flushdb()
            for item in tmp:
                r.delete(item['phone_number'])
            mysql.query(sql6)
    except Exception, e:
        print e   

    print '成功黑名单信息！'
if __name__ == "__main__":
    loadcode()