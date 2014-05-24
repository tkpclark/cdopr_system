#encoding:utf-8
import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import time
import logging
from logging.handlers import RotatingFileHandler


def get_data():
    sql = "select id from wraith_message where(mo_status is not null and mo_status!='ok') or (forward_status > 1) or (motime < NOW()-interval 4 hour) or (is_agent=0 and report is not null)"
    logging.info(sql)
    data = mysql.queryAll(sql);
    return data


def migrate(id):
    sql = "insert into wraith_message_history select * from wraith_message where id=%s"%(id)    
    logging.info('dbsql:%s',sql)
    mysql.query(sql)
    
    sql = "delete from wraith_message where id=%s"%(id)    
    logging.info('dbsql:%s',sql)
    mysql.query(sql)

    
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    logfile = '/home/tkp/cdopr/logs/migrate/migrate.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=10*1024*1024,backupCount=5)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.00]:  %(message)s - %(filename)s:%(lineno)d')
    Rthandler.setFormatter(formatter)
    logger=logging.getLogger()
    logger.addHandler(Rthandler)
    logger.setLevel(logging.NOTSET)
    
    
    
def main():
    
    init_env()
    
    while True:
        data = get_data() 
        #print (len(data))
        if(len(data) == 0):
            time.sleep(20)
            continue


        for record in data:
            ########logging.debug(json.dumps(record))
                #logging.info("record:%s",record)
                migrate(record['id'])
              
                   
            
            #time.sleep(10)
if __name__ == "__main__":
    main()
    


