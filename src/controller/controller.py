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
from m_dict import *
from blklist import *
from visit_limit import *
import datetime
import copy


class MoData:
    
    __seq_file__ = "id.seq"
    def get_deal_pos(self):
        f = open(self.__seq_file__)
        id = f.read()
        id = id.strip()
        f.close()
        if(id.isdigit()):
            return id
        else:
            print("not digtal in %s"%(self.__seq_file__))
            sys.exit(1)
    def set_deal_pos(self, id):
        f = open(self.__seq_file__,'w+')
        f.write(id)
        f.close()
    def read_data(self):
        sql = "select id,phone_number,mo_message,sp_number,linkid,gwid from wraith_message where id > '%s' limit 1"%(self.get_deal_pos())
        #logging.info(sql)
        data = mysql.queryAll(sql);
        return data


def write_db(id, cmd_info, zone, mo_status):
    
    if(len(cmd_info)>1):
        sql = "update wraith_message set cmdID='%s',province='%s',area='%s',fee='%s',service_id='%s',mt_message='%s',msgtype='%s', mo_status='%s',is_agent='%s' where id='%s'"%(cmd_info['cmdID'],zone[0],zone[1],cmd_info['fee'],cmd_info['service_id'],cmd_info['mt_message'],cmd_info['msgtype'],mo_status,cmd_info['is_agent'],id)
    else:
        sql = "update wraith_message set province='%s',area='%s', mo_status='%s' where id='%s'" %(zone[0],zone[1],mo_status,id)
    logging.info('dbsql:%s',sql)
    
    mysql.query(sql)
    
    
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    logfile = '/home/tkp/cdopr/logs/controller/controller.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=10*1024*1024,backupCount=5)
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
    
    global mobile_dict
    mobile_dict = Mobile_dict()
    mobile_dict.load_mobile_dict()
    
    global visit_limit
    visit_limit = Visit_limit()
    visit_limit.load_dict()
    
def main():
    
    init_env()
    mo_data = MoData() 
    cmd_info={}
    
    while True:
        data = mo_data.read_data()
        #print (len(data))
        if(len(data) == 0):
            time.sleep(1)
            continue

        for record in data:
            ########logging.debug(json.dumps(record))
            for i in range(1):#just for jumping to the end
                logging.info("record:%s",record)
                mo_data.set_deal_pos(record['id'])
                

                
                ########get province and area
                zone = mobile_dict.get_mobile_area(record['phone_number'])
                
                #######match a product
                cmd_info.clear()            
                cmd_info = copy.copy(product_route.match(record['gwid'], record['sp_number'], record['mo_message']))
                if(cmd_info == {}):
                    logging.fatal('!!! %s + %s + %s not match',record['gwid'], record['sp_number'], record['mo_message'])
                    mo_status='无匹配业务'
                    break
                
                logging.info('match product: %s',cmd_info)
                
                
                
                
                ########linkisok?
                if(record['linkid'].isdigit() == False):
                    logging.info('!!!linkid abnormal:' + record['linkid'])
                    mo_status = 'linkid异常'
                    break
                
                
                ##########blk list check
                #logging.info("matching..."+record['phone_number'])
                if(blklist.match(record['phone_number'])):
                    logging.info('!!!blklist:' + record['phone_number'])
                    mo_status='黑名单'
                    break
                
                              
                ###########mt_message
                cmd_info['mt_message']=product_route.get_random_content(cmd_info['cmdID'])
                
                
                
                ########check visit count 
                limit_flag = visit_limit.is_arrive_limit(record['phone_number'],cmd_info['cmdID'],zone[0],cmd_info['gwid'])
                if(limit_flag > 0):
                    logging.info('visit limit! phone_number:%s, cmd_id:%s, limit flag:%d',record['phone_number'],cmd_info['ID'],limit_flag)
                    mo_status='访问次数超限,%d'%(limit_flag)
                    break
                
                ########check open province  
                if (len(cmd_info['open_province']) > 0) and (cmd_info['open_province'] != 'None') and zone[0] not in cmd_info['open_province']:
                    logging.info('phone is not in open provinces! province:%s',zone[0])
                    mo_status='省份未开通'
                    break
    
                if(zone[0]+'@'+zone[1] in cmd_info['forbidden_area']):
                    logging.info('phone is in forbidden area! area:%s@%s',zone[0],zone[1])
                    mo_status='区域禁止'
                    break
                
                ########all check is ok
                mo_status='ok'
                #logging.info("breaking...")
                break
                ########
                ########
                
            write_db(record['id'],cmd_info,zone,mo_status)
                   
                
            
            
            #time.sleep(10)
if __name__ == "__main__":
    main()
    


