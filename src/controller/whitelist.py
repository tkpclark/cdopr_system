#encoding:utf-8

import sys 
import os
import redis
'''
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
class Blklist:
    
    __blklist__ = {}
    __t__ = '*'
    
    def load_blklist(self):
        sql = "select phone_number from wraith_blklist"
        tmp = mysql.queryAll(sql)
        
        #print "blklist loaded!"
        for item in tmp:
            self.__blklist__[item['phone_number']]=''
            
        #print self.__blklist__
    
    def match(self, phone_number):
        
        if(self.__blklist__.has_key(phone_number)):
            return True
        else:
            return False
   
    
if __name__ == "__main__":
    blklist = Blklist()
    blklist.load_blklist()
    print blklist.match(sys.argv[1])
    
'''




class Whitelist:
    
    r = False
    def __init__(self):
        self.r = redis.StrictRedis(host='localhost', port=6379, db=3)
        
    def match(self, phone_number):
        return  self.r.exists(phone_number)
   
    
if __name__ == "__main__":
    whitelist = Whitelist()
    print whitelist.match(sys.argv[1])    