#encoding:utf-8
import sys
import os
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


def write_db(id, cmd_info):
    
    '''
    sql = "update wraith_message set "
    for key in cmd_info:
        if(key=='id'):
            continue
        sql += "%s='%s',"%(key,cmd_info[key])
    sql = sql[0:-1]
    sql += " where id='%s'"%(id)
    '''
    sql = "update wraith_message set province='%s',area='%s',fee='%s',service_id='%s',mt_message='%s',msgtype='%s', msg_status='%s',msg_status_info='%s' where id='%s'"%(cmd_info['province'],cmd_info['area'],cmd_info['fee'],cmd_info['service_id'],cmd_info['mt_message'],cmd_info['msgtype'],cmd_info['msg_status'], cmd_info['msg_status_info'],id)
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
            while True:#just for jumping to the end
                logging.info(record)
                mo_data.set_deal_pos(record['id'])
                cmd_info.clear()
                #cmd_info['msg_status']='110'
                
                
                #######match a product            
                cmd_info = copy.copy(product_route.match(record['gwid'], record['sp_number'], record['mo_message']))
                if(cmd_info == {}):
                    logging.fatal('!!! %s + %s + %s not match',record['gwid'], record['sp_number'], record['mo_message'])
                    cmd_info['msg_status_info']='无匹配业务'
                    break
                
                logging.info('match product: %s',cmd_info)
                
                ########get province and area
                cmd_info['province'],cmd_info['area'] = mobile_dict.get_mobile_area(record['phone_number'])
                
                
                ########linkisok?
                if(record['linkid'].isdigit() == False):
                    logging.info('!!!linkid abnormal:' + record['linkid'])
                    cmd_info['msg_status_info'] = 'linkid异常'
                    break
                
                
                ##########blk list check
                #logging.info("matching..."+record['phone_number'])
                if(blklist.match(record['phone_number'])):
                    logging.info('!!!blklist:' + record['phone_number'])
                    cmd_info['msg_status_info']='黑名单'
                    break
                
                              
                cmd_info['mt_message']=product_route.get_random_content(cmd_info['ID'])
                
                
                
                ########check visit count 
                limit_flag = visit_limit.is_arrive_limit(record['phone_number'],cmd_info['ID'],cmd_info['province'],cmd_info['gwid'])
                if(limit_flag > 0):
                    logging.info('visit limit! phone_number:%s, cmd_id:%s, limit flag:%d',record['phone_number'],cmd_info['ID'],limit_flag)
                    cmd_info['msg_status_info']='访问次数超限,%d'%(limit_flag)
                    break
                
                ########check allow province  
                if (len(cmd_info['allow_province']) > 0) and (cmd_info['allow_province'] != 'None') and cmd_info['province'] not in cmd_info['allow_province']:
                    logging.info('phone is not in allow provinces! province:%s',cmd_info['province'])
                    cmd_info['msg_status_info']='省份未开通'
                    break
    
                if(cmd_info['province']+'@'+cmd_info['area'] in cmd_info['forbidden_area']):
                    logging.info('phone is in forbidden area! area:%s@%s',cmd_info['province'],cmd_info['area'])
                    cmd_info['msg_status_info']='区域禁止'
                    break
                
                ########all check is ok
                cmd_info['msg_status_info']='mo正常'
                cmd_info['msg_status']='10'
                logging.info("breaking...")
                break
                ########
                ########
                
            write_db(record['id'],cmd_info)
                   
                
            
            
            #time.sleep(10)
if __name__ == "__main__":
    main()
    


