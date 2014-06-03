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
from deduction import *
from command import *
import datetime
import copy
from ran import in_po
#from forward_func import *
#import threading

def update_forward_info(message_id,forward_status,forward_result,type):
    global mysql
    sql = " update wraith_message set forward_status='%d', forward_%s_result='%d',forward_%s_time=NOW() where id='%s'"%(forward_status,type,forward_result,type,message_id)    
    #logging.info('dbsql:%s',sql)
    mysql.query(sql)
    #time.sleep(100)
    
def f_no(record,mourl):
    #logging.info('forwarding nothing %s',record)
    #time.sleep(1)
    return 1
 
def f_mo(record,mourl):
    #logging.info('forwarding record %s',record)
    #time.sleep(1)
    #http://youraddress/interface_mo?spnumber=106673336&msg=CP&fee=2&mobile=13179386983&linkid=72523970&createtime=20120320095009
    nowtime = '11223344'
    url = '%s?spnumber=%s&msg=%s&fee=%s&mobile=%s&linkid=%s&createtime=%s' \
    %(mourl,record['sp_number'],record['mo_message'],record['fee'],record['phone_number'],record['linkid'],nowtime)
    logging.info('(%s):%s',record['id'], url)
    return 1
    
def f_mr(record,mrurl):
    #logging.info('forwarding record %s',record)
    #time.sleep(1)
    nowtime = '11223344'
    url = '%s?mobile=%s&linkid=%s&status=%s&createtime=%s' \
    %(mrurl,record['phone_number'],record['linkid'],record['report'],nowtime)
    logging.info('(%s):%s',record['id'], url)
    return 1
    
    
def get_data():
    sql = 'select * from wraith_message where is_agent=2 and ((forward_status=0) or (forward_status=1 and report is not NULL)) limit 1000'
    #logging.info(sql)
    data = mysql.queryAll(sql);
    return data


def write_db(id, cmd_info):
    sql = '''
    update wraith_message set
    cmd_spnumber='%s',
    cmd_mocmd='%s',
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
    %(cmd_info['cmd_spnumber'],cmd_info['cmd_mocmd'],cmd_info['sp_id'],cmd_info['cpname'],cmd_info['cpID'],cmd_info['spID'],cmd_info['service_name'],cmd_info['spname'],cmd_info['cp_productID'],cmd_info['cp_product_name'],cmd_info['serviceID'],cmd_info['serv_mocmd'],cmd_info['serv_spnumber'],id)
    #logging.info(sql)
    
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
    

    
    global deduction
    deduction = Deduction()
    deduction.load_dict()
    
    global cmd
    cmd = Command()
    cmd.load_dict()
    

    
def main():
    
    init_env()
    logging.info("starting...")
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
        for record in data:
            ########logging.debug(json.dumps(record))
            #logging.info("record:%s",record)
            cmd_info = cmd.get_cmd_info(record['cmdID'])
            
            #####转发mo#####
            if(record['forward_status']=='0'):
                type = 'mo'
                
                #logging.info("cmd_info:%s",cmd_info)
                write_db(record['id'],cmd_info)
                
                #threading.Thread(target=eval(cmd_info['forward_mo_module']), args=(record['id'], cmd_info['mourl'])).start()
                de = deduction.get_deduction(cmd_info['cp_productID'],record['province'])
                if(in_po(de)): 
                    forward_status = 4 
                    forward_result = 2 
                else:
                    forward_result = eval("%s(record,cmd_info['mourl'])"%(cmd_info['forward_mo_module'])) 
                    forward_status = 1 if(forward_result == 1) else 6 
                
                
            ####转发mr#####
            elif(record['forward_status']=='1'):
                type = 'mt'
                #threading.Thread(target=eval(cmd_info['forward_mr_module']), args=(record['id'], cmd_info['mourl'])).start()
                #f11(record['id'],cmd_info['mourl'])
                forward_result = eval("%s(record,cmd_info['mrurl'])"%(cmd_info['forward_mr_module']))
                forward_status = 2 if(forward_result == 1) else 7 
                
                
                
            ################
            else:
                logging.info("impossible!")
                
            update_forward_info(record['id'],forward_status,forward_result,type)
            #sys.exit()     
                
if __name__ == "__main__":
    main()
    


