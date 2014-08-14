#encoding:utf-8

import sys
import string
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
from visit_limit import *
import datetime
import copy
from ran import in_po
import urllib2
import urllib
#from forward_func import *
#import threading

def visit_url(url):
    #logging.info(url)
    try:
        res = urllib2.urlopen(url, timeout=2)
        r = res.read().strip()
        logging.info("res:%s",r)
        return (1,r)
    except Exception, e:
        logging.info(e)
        #logging.info('failed')
        return (0,'')
def update_forward_info(message_id,forward_status,forward_result,forward_resp,forward_url,type):
    global mysql
    
    if(   forward_status==4     \
       or forward_status==3     \
       or forward_status==5     \
       or forward_status==13    \
       or forward_status==14    \
       or forward_status==15    \
       or forward_status==16 
       ):
        sql = "update wraith_message set forward_status='%d' where id='%s'"%(forward_status, message_id)
    else:
        sql = "update wraith_message set forward_status='%d', forward_%s_result='%d',forward_%s_time=NOW(),forward_%s_resp='%s',forward_%s_url='%s' where id='%s'" \
        %(forward_status,type,forward_result,type,type,forward_resp,type,forward_url,message_id)    
    #logging.info('dbsql:%s',sql)
    mysql.query(sql)
    #time.sleep(100)
    
#用于不转发的情况
def f_no(record,mourl):
    #logging.info('forwarding nothing %s',record)
    #time.sleep(1)
    return (3,0,0,0)
 
#用于上下行一起转的情况中，上行填写
def f_fake(record,mourl):
    return (1,1,0,0)

def f_mo(record,mourl):
    #logging.info('forwarding record %s',record)
    #time.sleep(1)
    #http://youraddress/interface_mo?spnumber=106673336&msg=CP&fee=2&mobile=13179386983&linkid=72523970&createtime=20120320095009
    nowtime = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    url = '%s?spnumber=%s&msg=%s&fee=%s&mobile=%s&linkid=%s&createtime=%s' \
    %(mourl,record['sp_number'],record['mo_message'],record['fee'],record['phone_number'],record['linkid'],nowtime)
    logging.info('(%s):%s',record['id'], url)
    forward_result,forward_resp = visit_url(url)
    forward_status = 1 if(forward_result == 1) else 6 #1：转发成功 6：mo转发失败
    return(forward_status,forward_result,forward_resp,url) 
    
def f_mr(record,mrurl):
    #logging.info('forwarding record %s',record)
    #time.sleep(1)
    nowtime = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    report='DELIVRD' if record['report']=='1' else record['report']
    url = '%s?spnumber=%s&msg=%s&mobile=%s&linkid=%s&status=%s&createtime=%s' \
    %(mrurl,record['sp_number'],record['mo_message'],record['phone_number'],record['linkid'],report,nowtime)
    logging.info('(%s):%s',record['id'], url)
    forward_result,forward_resp = visit_url(url)
    forward_status = 2 if(forward_result == 1) else 7 
    return(forward_status,forward_result,forward_resp,url) 
    
def f_mo_1(record,mourl):
    #logging.info('forwarding record %s',record)
    #time.sleep(1)
    #http://youraddress/interface_mo?spnumber=106673336&msg=CP&fee=2&mobile=13179386983&linkid=72523970&createtime=20120320095009
    nowtime = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    url = '%s?spnumber=%s&msg=%s&fee=%s&mobile=%s&linkid=%s&createtime=%s&prov=%s&area=%s' \
    %(mourl,record['sp_number'],record['mo_message'],record['fee'],record['phone_number'],record['linkid'],nowtime,record['province'],record['area'])
    logging.info('(%s):%s',record['id'], url)
    forward_result,forward_resp = visit_url(url)
    forward_status = 1 if(forward_result == 1) else 6 #1：转发成功 6：mo转发失败
    return(forward_status,forward_result,forward_resp,url) 
    
def f_mr_1(record,mrurl):
    #logging.info('forwarding record %s',record)
    #time.sleep(1)
    nowtime = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    report='DELIVRD' if record['report']=='1' else record['report']
    url = '%s?spnumber=%s&msg=%s&mobile=%s&linkid=%s&status=%s&createtime=%s&prov=%s&area=%s' \
    %(mrurl,record['sp_number'],record['mo_message'],record['phone_number'],record['linkid'],report,nowtime,record['province'],record['area'])
    logging.info('(%s):%s',record['id'], url)
    forward_result,forward_resp = visit_url(url)
    forward_status = 2 if(forward_result == 1) else 7 
    return(forward_status,forward_result,forward_resp,url) 
    
def get_data():
    sql = "select * from wraith_message where mo_status='ok' and is_agent=2 and ((forward_status=0) or (forward_status=1 and report is not NULL)) order by id asc limit 100"
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
    Rthandler = RotatingFileHandler(logfile, maxBytes=1000*1024,backupCount=500)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.01]:  %(message)s - %(filename)s:%(lineno)d')
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
    
    global visit_limit
    visit_limit = Visit_limit()
    visit_limit.load_dict()

    
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
            
            
            forward_status=''
            forward_result = 0
            forward_resp = ''
            forward_url = ''
            type = ''
                    
            
            for i in range(1):#just for jumping to the end
                cmd_info = cmd.get_cmd_info(record['cmdID'])
                
                
                ######是否转发超限#######
                f,v = visit_limit.is_arrive_forward_limit(record['phone_number'],cmd_info['cmdID'],record['province'])
                if(f != 0):
                    forward_status=f
                    forward_result = 0
                    forward_resp = ''
                    forward_url = ''
                    type = '' #useless just make a defination
                    break
                
                
                
                
                
                #＝＝＝＝＝＝转发＝＝＝＝＝＝#
                
                #####转发mo#####
                if(record['forward_status']=='0'):
                    type = 'mo'
                    
                    #logging.info("cmd_info:%s",cmd_info)
                    #write_db(record['id'],cmd_info)
                    
                    #threading.Thread(target=eval(cmd_info['forward_mo_module']), args=(record['id'], cmd_info['mourl'])).start()
                    de = deduction.get_deduction(cmd_info['cp_productID'],record['province'])
                    if(in_po(de)): 
                        forward_status = 4 #上行被扣量
                        forward_result = 0 #随便赋值
                    else:
                        forward_status,forward_result,forward_resp,forward_url = eval("%s(record,cmd_info['mourl'])"%(cmd_info['forward_mo_module'])) 
                        
                    
                    
                ####转发mr#####
                elif(record['forward_status']=='1'):
                    type = 'mr'
                    #threading.Thread(target=eval(cmd_info['forward_mr_module']), args=(record['id'], cmd_info['mourl'])).start()
                    #f11(record['id'],cmd_info['mourl'])
                    forward_status,forward_result,forward_resp,forward_url = eval("%s(record,cmd_info['mrurl'])"%(cmd_info['forward_mr_module']))
                    
                    
                    
                    
                ################
                else:
                    logging.info("impossible!")
                
                
                
                
            update_forward_info(record['id'],forward_status,forward_result,forward_resp,forward_url,type)
            #sys.exit()     
                
if __name__ == "__main__":
    main()
    


