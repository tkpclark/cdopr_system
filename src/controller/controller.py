#encoding:utf-8

import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import time
import urllib
import urllib2
import logging
from logging.handlers import RotatingFileHandler
import json
from product_route import *
from codeseg import *
from blklist import *
from visit_limit import *
from frequency import *
import datetime
import copy
from ran import in_po
    
def get_data():
    sql = 'select id,phone_number,mo_message,sp_number,linkid,gwid,province,area,motime from wraith_message where mo_status is null order by id asc limit 1000'
    #logging.info(sql)
    data = mysql.queryAll(sql);
    return data

def write_db(id, cmd_info, zone, mo_status):
    
    ####正式运营请注释掉此部分代码
    if(mo_status=='ok'):
        report=',report=%d'%(2 if in_po(0.05) else 1)
    else:
        report=''
    
    ####
    if(len(cmd_info)>1):
        sql = "update wraith_message set province='%s',area='%s', cmdID='%s',fee='%s',feetype='%s',service_id='%s',mt_message='%s',msgtype='%s', mo_status='%s',is_agent='%s' %s where id='%s'"%(zone[0],zone[1], cmd_info['cmdID'],cmd_info['fee'],cmd_info['feetype'],cmd_info['service_id'],cmd_info['mt_message'],cmd_info['msgtype'],mo_status,cmd_info['is_agent'],report,id)
    else:
        sql = "update wraith_message set province='%s',area='%s', mo_status='%s' where id='%s'" %(zone[0],zone[1],mo_status,id)
    #logging.info('dbsql:%s',sql)
    
    mysql.query(sql)
    
    
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    logfile = '/home/tkp/cdopr/logs/controller/controller.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=10*1024,backupCount=500)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.00]:  %(message)s - %(filename)s:%(lineno)d')
    Rthandler.setFormatter(formatter)
    logger=logging.getLogger()
    logger.addHandler(Rthandler)
    logger.setLevel(logging.NOTSET)
    
    #init product_route
    global product_route
    product_route = Product_route()
    product_route.load_products()
    
    global blklist
    blklist = Blklist()
    blklist.load_blklist()
    
    global codeseg
    codeseg = Codeseg()
    
    global visit_limit
    visit_limit = Visit_limit()
    visit_limit.load_dict()
    
    global frequency
    frequency = Frequency()
    
def main():
    
    init_env()
    logging.info("starting...")
    cmd_info={}
    
    while True:
        data = get_data() 
        #print (len(data))
        if(len(data) == 0):
            time.sleep(1)
            #logging.info("no data")
            continue
     

        for record in data:
            ########logging.debug(json.dumps(record))
            for i in range(1):#just for jumping to the end
                
                ########get province and area
                if(record['province']=='None'):
                    zone = codeseg.get_mobile_area(record['phone_number'])
                else:
                    zone = (record['province'],record['area'])
                    
                #######match a product
                cmd_info.clear()            
                cmd_info = copy.copy(product_route.match(record['gwid'], record['sp_number'], record['mo_message']))
                if(cmd_info == {}):
                    mo_status='无匹配业务'
                    break
                
                #logging.info('match product: %s',cmd_info)
                ###########mt_message
                if(cmd_info['app_module']):#调用相应的应用来生成下行
                    cmd_info['mt_message']=''
                    pass
                    #调用应用的代码写在这里
                else:#立刻生成下行                
                    cmd_info['mt_message']=product_route.get_random_content(cmd_info['cmdID'])
                
                
                ########Frequency
                if(frequency.rec_freq(record['phone_number'],record['motime'])==False):
                    mo_status = '频度过高'
                    break
                
                ########linkisok?
                if(record['linkid'].isdigit() == False):
                    mo_status = 'linkid异常'
                    break
                
                
                ##########blk list check
                #logging.info("matching..."+record['phone_number'])
                if(blklist.match(record['phone_number'])):
                    mo_status='黑名单'
                    break
                
                
                
                
                ########check visit count 
                f,v = visit_limit.is_arrive_limit(record['phone_number'],cmd_info['cmdID'],zone[0])
                if(f != '0'):
                    mo_status = v
                    break
                
                ########check open province  
                if (len(cmd_info['open_province']) > 0) and (cmd_info['open_province'] != 'None') and zone[0] not in cmd_info['open_province']:
                    mo_status='省份未开通'
                    break
    
                if(zone[0]+'@'+zone[1] in cmd_info['forbidden_area']):
                    mo_status='区域禁止'
                    break
                
                ########all check is ok
                mo_status='ok'
                ########
                ########
                break
            logging.info("record:%s;zone:%s,%s;result:%s",record,zone[0],zone[1],mo_status)
            write_db(record['id'],cmd_info,zone,mo_status)
                   
                
            
            
            #time.sleep(10)
if __name__ == "__main__":
    main()
    


