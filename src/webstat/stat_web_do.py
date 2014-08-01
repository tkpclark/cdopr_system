#encoding:utf-8

import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
import time
import logging
import datetime
from logging.handlers import RotatingFileHandler

#stat_table = 'wraith_message'

def stat(stat_hour):
    logging.info("***stat hour:%s***",stat_hour)
    print '***stat hour:%s***'%(stat_hour)
    db_stat_hour = "DATE_FORMAT(timeStamp,'%Y-%m-%d:%H')"
    #db_stat_hour = sys.argv[1]
    
    
    sql = "select ditchId,price,province from wraith_wo_web where %s='%s' group by ditchId,price,province " % (db_stat_hour,stat_hour)
    logging.info(sql)
    result = mysql.queryAll(sql)
    if(mysql.rowcount()==0):
        return
    
    
    for row in result:
       
        where_clause = " %s='%s' and ditchId='%s'  and price='%s' and province='%s' " \
        %(db_stat_hour,stat_hour,row['ditchId'],row['price'],row['province'])
        logging.info(where_clause)
        
        #count msg_count_all 总条数
        csql = "select count(*) as msg_count_all from wraith_wo_web where %s" % (where_clause)
        logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_all = cresult[0]['msg_count_all']
        
        
        #msg_count_suc and count amount_suc record number:成功条数 金额
        csql = "select count(*) as msg_count_suc, sum(totalFee) as amount_suc from wraith_wo_web where %s and resultCode = '0' " % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_suc = cresult[0]['msg_count_suc']
        amount_suc = cresult[0]['amount_suc'] if cresult[0]['amount_suc']!='None' else '0'
        
        
        
        
        #成功转发成功的mr数量和金额            
        #count msg_count_forward and amount_forward record number
        csql = "select count(*) as msg_count_forward_mr,sum(totalFee) as amount_forward from wraith_wo_web where %s and resultCode = '0' and forward_mr_result='1' " % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_forward_mr = cresult[0]['msg_count_forward_mr']
        amount_forward = cresult[0]['amount_forward'] if cresult[0]['amount_forward']!='None' else '0'
        
        #insert or update
        csql = "select * from wraith_wo_web_statistic where stat_time='%s' and ditchId='%s' and price='%s' and province='%s'" \
        %(stat_hour,row['ditchId'],row['price'],row['province'])
        cresult = mysql.queryAll(csql)
        #logging.info(csql)
        if(len(cresult)>0):
            csql = "update wraith_wo_web_statistic set msg_count_all='%s',msg_count_suc='%s',amount_suc='%s',msg_count_forward_mr='%s',amount_forward='%s' where stat_time='%s' and ditchId='%s' and price='%s' and province='%s'" \
            %(msg_count_all,msg_count_suc,amount_suc,msg_count_forward_mr,amount_forward,stat_hour,row['ditchId'],row['price'],row['province'])
        else:
            csql = "insert into wraith_wo_web_statistic(stat_time,price,ditchId,province,msg_count_all,msg_count_suc,amount_suc,msg_count_forward_mr,amount_forward)values('%s','%s','%s','%s','%s','%s','%s','%s','%s')" \
            % (stat_hour,row['price'],row['ditchId'],row['province'],msg_count_all,msg_count_suc,amount_suc,msg_count_forward_mr,amount_forward)
        logging.info(csql)
        mysql.query(csql)
        
        
            
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    logfile = '/home/tkp/cdopr/logs/stat/stat_wo_web.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=10*1024*1024,backupCount=5)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.00]:  %(message)s - %(filename)s:%(lineno)d')
    Rthandler.setFormatter(formatter)
    logger=logging.getLogger()
    logger.addHandler(Rthandler)
    logger.setLevel(logging.NOTSET)

