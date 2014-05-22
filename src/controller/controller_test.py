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
from product_route_test import *
from m_dict import *
from blklist import *
from visit_limit import *
import datetime


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

    def read_data(self):
        sql = "select * from wraith_mo order by id desc limit 1"
        data = mysql.queryAll(sql);
        return data
    

    
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    logfile = '/home/app/wraith/logs/controller/controller_test.log'
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
    
    while True:
        data = mo_data.read_data()
        #print (len(data))
        if(len(data) == 0):
            time.sleep(1)
            continue

        for record in data:
            ########logging.debug(json.dumps(record))
            logging.info(record)
            
            ########linkisok?
            if(record['linkid'].isdigit() == False):
                logging.info('!!!linkid abnormal:' + record['linkid'])
                continue
            
            
            ##########blk list check
            #logging.info("matching..."+record['phone_number'])
            if(blklist.match(record['phone_number'])):
                logging.info('!!!blklist:' + record['phone_number'])
                continue
            
            
         
            #######match a product            
            product = product_route.match(record['gwid'], record['sp_number'], record['message'])
            if(product == False):
                logging.fatal('!!! %s + %s + %s not match',record['gwid'], record['sp_number'], record['message'])   
                continue
            
            
            
            
            logging.info('match product:' + product['id'])
            
           
            ########append product info for app
            record['product_seq'] = product['id']
            record['product_id'] = product['product_id']
            record['product_code'] = product['product_code']
            record['amount'] = product['amount']
            record['province'],record['area'] = mobile_dict.get_mobile_area(record['phone_number'])
            record['default_msg']=product_route.get_random_content(product['id'])
            record['allow_province']=product['allow_province']
            
            print record['default_msg']
                 
            ########check allow province  
            if record['province'] not in record['allow_province']:
                logging.info('phone is not in allow provinces! province:%s',record['province'])
                continue
            
            if record['area'] in '深圳 汕头 邯郸 石家庄 扬州 镇江 烟台 怀化':
                logging.info('phone is in forbidden area! area:%s',record['area'])
                continue
            
            
            logging.info(record['area'])
                    
            sys.exit()
            
            
            #time.sleep(10)
if __name__ == "__main__":
    main()
    


