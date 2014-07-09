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
    '''
    condition1 = "mo_status is not null and mo_status!='ok'"#mo处理结果为非法的记录
    condition2 = "motime < NOW()-interval 4 hour" #超过n小时仍未处理完毕的记录
    condition3 = "is_agent=1 and report is not null" #非转发业务处理完成的记录
    condition4 = 'forward_status>1'#转发完成的记录
    '''
    
    sql = "select id from wraith_message where motime < CURDATE()"
    logging.info(sql)
    data = mysql.queryAll(sql);
    return data

'''
def migrate():
    sql = "insert into wraith_message_history select * from wraith_message where motime < CURDATE()" 
           
    logging.info('dbsql:%s',sql)
    mysql.query(sql)
    
    sql = "delete from wraith_message where motime < CURDATE()" 
    logging.info('dbsql:%s',sql)
    #mysql.query(sql)
'''
    
def migrate(id):
    

    sql = "replace into wraith_message_history select * from wraith_message where id='%s'"%(id) 
    logging.info('dbsql:%s',sql)
    mysql.query(sql)
    affected_num = mysql.conn.affected_rows()
    logging.info('affected_rows:%d'%(affected_num))
                 
    if(affected_num != 1):
        logging.info("failed to insert ,ignore")
        return
    
    sql = "delete from wraith_message where id='%s'"%(id) 
    logging.info('dbsql:%s',sql)
    mysql.query(sql)
    
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    logfile = '/home/tkp/cdopr/logs/migrate/migrate.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=1000*1024,backupCount=500)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.00]:  %(message)s - %(filename)s:%(lineno)d')
    Rthandler.setFormatter(formatter)
    logger=logging.getLogger()
    logger.addHandler(Rthandler)
    logger.setLevel(logging.NOTSET)
    
    
    
def main():
    
    init_env()
    
    
    data = get_data() 

    for record in data:
        ########logging.debug(json.dumps(record))
            #logging.info("record:%s",record)
            
            migrate(record['id'])
    #migrate()
            
            #time.sleep(10)
if __name__ == "__main__":
    main()
    


