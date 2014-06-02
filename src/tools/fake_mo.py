#encoding:utf-8

import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import string
import random
import time

def main():
    
    for i in range(string.atoi(sys.argv[1])):
        phone_number='133%s'%("".join(random.sample('01234567890123456789012345678901234567890123456789',8)))
        sp_number='10669999'
        mo_message=random.choice(['232', '12321','12322','112','rfd'])
        linkid="".join(random.sample('012345678901234567890123456789012345678901234567890123456789',20))
        #gwid=random.randint(1,20)
        gwid=10
        
        sql = "insert into wraith_message(motime,phone_number,mo_message,sp_number,linkid,gwid)values(NOW(),'%s','%s','%s','%s','%s')"%(phone_number,mo_message,sp_number,linkid,gwid)
        print sql
        mysql.cur.execute(sql)
        
        time.sleep(0.1)
if __name__ == "__main__":
    main()