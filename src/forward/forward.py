#encoding:utf-8

import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import time
import logging
from logging.handlers import RotatingFileHandler
import json
from command import *
import datetime
import copy
#from forward_func import *
#import threading

def update_forward_result(message_id,forward_result,type):
    global mysql
    sql = " update wraith_message set forward_%s_result='%d' where id='%s'"%(type,forward_result,message_id)    
    logging.info('dbsql:%s',sql)
    mysql.query(sql)
    #time.sleep(100)
    
    
def f10(record):
    logging.info('forwarding record %s',record)
    time.sleep(1)
    update_forward_result(record['id'],1,"mo")
def f11(record):
    logging.info('forwarding record %s',record)
    time.sleep(1)
    update_forward_result(record['id'],2,"mt")
    
    
def get_data():
    sql = 'select * from wraith_message where is_agent=1 and forward_status in (0,1)'
    #logging.info(sql)
    data = mysql.queryAll(sql);
    return data


def write_db(id, cmd_info):
    sql = '''
    update wraith_message set 
    sp_id='%s',
    cpname='%s',
    cpID='%s',
    spID='%s',
    service_name='%s',
    spname='%s', 
    cp_productID='%s',
    cp_product_name='%s',
    serviceID='%s',
    serv_mocmd='%s',
    serv_spnumber='%s'
    where id='%s' 
    '''\
    %(cmd_info['sp_id'],cmd_info['cpname'],cmd_info['cpID'],cmd_info['spID'],cmd_info['service_name'],cmd_info['spname'],cmd_info['cp_productID'],cmd_info['cp_product_name'],cmd_info['serviceID'],cmd_info['serv_mocmd'],cmd_info['serv_spnumber'],id)
    logging.info(sql)
    
    mysql.query(sql)

def update_forward_status(id,forward_status,type):
    sql = " update wraith_message set forward_status='%d',forward_%s_time=NOW() where id='%s'"%(forward_status,type,id)    
    logging.info('dbsql:%s',sql)
    mysql.query(sql)

    
def init_env():
    
    #init logging
    logfile = '/home/tkp/cdopr/logs/forward/forward.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=10*1024*1024,backupCount=5)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.00]:  %(message)s - %(filename)s:%(lineno)d')
    Rthandler.setFormatter(formatter)
    logger=logging.getLogger()
    logger.addHandler(Rthandler)
    logger.setLevel(logging.NOTSET)
    
    
    
    global cmd
    cmd = Command()
    cmd.load_cmds()

    
def main():
    
    init_env()
    
    while True:
        data = get_data() 
        #print (len(data))
        if(len(data) == 0):
            time.sleep(1)
            continue
        
        
        '''
        if(len(threading.enumerate()) > 100):
            logging.info("threading numbers too much! sleep for a while...")
            time.sleep(1)
            continue
        '''
        deal_num = 0
        for record in data:
            ########logging.debug(json.dumps(record))
            #logging.info("record:%s",record)
                
            #get cmd info
            cmd_info = cmd.get_cmd_info(record['cmdID'])
            #logging.info("cmd_info:%s",cmd_info)
           
            #finish cmd info of this message
            if(record['serv_mocmd']=='None'):
                write_db(record['id'],cmd_info)
                
                
            #1.转发mo+mr一起转的记录
            #forward_method为1，report not null，forward_status=0
            if(cmd_info['forward_method']=='1' and record['forward_status']=='0' and record['report']!='None'):
                update_forward_status(record['id'],3,"mo")
                #threading.Thread(target=eval(cmd_info['forward_mo_module']), args=(record['id'], cmd_info['mourl'])).start()
                eval("%s(record)"%(cmd_info['forward_mo_module']))
                deal_num += 1
            #2.转mo/mr分的记录的上行
            #forward_method为0，forward_status=0 
            elif(cmd_info['forward_method']=='0' and record['forward_status']=='0'):
                update_forward_status(record['id'],1,"mo")
                #threading.Thread(target=eval(cmd_info['forward_mo_module']), args=(record['id'], cmd_info['mourl'])).start()
                eval("%s(record)"%(cmd_info['forward_mo_module']))
                deal_num += 1
                #f10(record['id'],cmd_info['mourl'])
            #3.转mo/mr分开的下行
            #forward_method为0，forward_status=1 report is not null
            elif(cmd_info['forward_method']=='0' and record['forward_status']=='1' and record['report']!='None'):
                update_forward_status(record['id'],2,"mt")
                #threading.Thread(target=eval(cmd_info['forward_mt_module']), args=(record['id'], cmd_info['mourl'])).start()
                #f11(record['id'],cmd_info['mourl'])
                eval("%s(record)"%(cmd_info['forward_mo_module']))
                deal_num += 1
                    
                #sys.exit()
                
        if(deal_num == 0):
            time.sleep(1)       
                
        #logging.info("all:%d,deal:%d",len(data),deal_num)  
            
            #time.sleep(10)
if __name__ == "__main__":
    main()
    


