import sys
import os
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