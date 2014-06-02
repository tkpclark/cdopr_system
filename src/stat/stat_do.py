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


def stat(stat_hour):
    logging.info("***stat hour:%s***",stat_hour)
    db_stat_hour = "DATE_FORMAT(motime,'%Y-%m-%d:%H')"
    #db_stat_hour = sys.argv[1]
    
    #delete old days if
    sql = "delete from wraith_statistic where stat_time = '%s'" % (stat_hour)
    #logging.info(sql)
    mysql.query(sql)
    
    
    sql = "select gwid,feetype,is_agent,cmdID,spID,serviceID,cpID,cp_productID,province from wraith_message_history where %s='%s' group by gwid,feetype,is_agent,cmdID,spID,serviceID,cpID,cp_productID,province " % (db_stat_hour,stat_hour)
    #logging.info(sql)
    result = mysql.queryAll(sql)
    if(mysql.rowcount()>0):
        for row in result:
           
            where_clause = " %s='%s' and gwid='%s' and feetype='%s' and is_agent='%s' and cmdID='%s' and spID='%s' and serviceID='%s' and cpID='%s' and cp_productID='%s' and province='%s' " \
            %(db_stat_hour,stat_hour,row['gwid'],row['feetype'],row['is_agent'],row['cmdID'],row['spID'],row['serviceID'],row['cpID'],row['cp_productID'],row['province'])
            #logging.info(where_clause)
            
            #count msg_count_all
            csql = "select count(*) as msg_count_all from wraith_message_history where %s" % (where_clause)
            #logging.info(csql)
            cresult = mysql.queryAll(csql)
            msg_count_all = cresult[0]['msg_count_all']
            
            #count msg_count_legal
            csql = "select count(*) as msg_count_legal from wraith_message_history where %s and mo_status='ok'" % (where_clause)
            #logging.info(csql)
            cresult = mysql.queryAll(csql)
            msg_count_legal = cresult[0]['msg_count_legal']
            
            
            #msg_count_suc and count amount_suc record number:
            csql = "select count(*) as msg_count_suc, sum(fee) as amount_suc from wraith_message_history where %s and report = '1' " % (where_clause)
            #logging.info(csql)
            cresult = mysql.queryAll(csql)
            msg_count_suc = cresult[0]['msg_count_suc']
            amount_suc = cresult[0]['amount_suc'] if cresult[0]['amount_suc']!='None' else '0'
            
            
            #count msg_count_deduction and amount_deduction record number
            csql = "select count(*) as msg_count_deduction, sum(fee) as amount_deduction from wraith_message_history where %s and forward_status=4 " % (where_clause)
            #logging.info(csql)
            cresult = mysql.queryAll(csql)
            msg_count_deduction = cresult[0]['msg_count_deduction']
            amount_deduction = cresult[0]['amount_deduction'] if cresult[0]['amount_deduction']!='None' else '0'
            
            #insert
            csql = "insert into wraith_statistic(stat_time,gwid,feetype,is_agent,cmdID,spID,serviceID,cpID,cpProdID,province,msg_count_all,msg_count_legal,msg_count_suc,msg_count_deduction,amount_suc,amount_deduction)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')" \
            % (stat_hour,row['gwid'],row['feetype'],row['is_agent'],row['cmdID'],row['spID'],row['serviceID'],row['cpID'],row['cp_productID'],row['province'],msg_count_all,msg_count_legal,msg_count_suc,msg_count_deduction,amount_suc,amount_deduction)
            #logging.info(csql)
            mysql.query(csql)
            
            
            
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    logfile = '/home/tkp/cdopr/logs/stat/stat.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=10*1024*1024,backupCount=5)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.00]:  %(message)s - %(filename)s:%(lineno)d')
    Rthandler.setFormatter(formatter)
    logger=logging.getLogger()
    logger.addHandler(Rthandler)
    logger.setLevel(logging.NOTSET)

