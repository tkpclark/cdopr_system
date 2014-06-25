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
    db_stat_hour = "DATE_FORMAT(motime,'%Y-%m-%d:%H')"
    #db_stat_hour = sys.argv[1]
    
    
    sql = "select gwid,feetype,is_agent,cmdID,spID,serviceID,cpID,cp_productID,province from wraith_message where %s='%s' and cmdId!=0 group by gwid,feetype,is_agent,cmdID,spID,serviceID,cpID,cp_productID,province " % (db_stat_hour,stat_hour)
    logging.info(sql)
    result = mysql.queryAll(sql)
    if(mysql.rowcount()==0):
        return
    
    
    for row in result:
       
        where_clause = " %s='%s' and gwid='%s' and feetype='%s' and is_agent='%s' and cmdID='%s' and spID='%s' and serviceID='%s' and cpID='%s' and cp_productID='%s' and province='%s' " \
        %(db_stat_hour,stat_hour,row['gwid'],row['feetype'],row['is_agent'],row['cmdID'],row['spID'],row['serviceID'],row['cpID'],row['cp_productID'],row['province'])
        #logging.info(where_clause)
        
        #count msg_count_all
        csql = "select count(*) as msg_count_all from wraith_message where %s" % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_all = cresult[0]['msg_count_all']
        
        #count msg_count_legal
        csql = "select count(*) as msg_count_legal from wraith_message where %s and mo_status='ok'" % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_legal = cresult[0]['msg_count_legal']
        
        
        #msg_count_suc and count amount_suc record number:
        csql = "select count(*) as msg_count_suc, sum(fee) as amount_suc from wraith_message where %s and report = '1' " % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_suc = cresult[0]['msg_count_suc']
        amount_suc = cresult[0]['amount_suc'] if cresult[0]['amount_suc']!='None' else '0'
        
        
        #count msg_count_deduction record number
        csql = "select count(*) as msg_count_deduction from wraith_message where %s and mo_status='ok' and forward_status in(1,2,3,6,7) " % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_deduction = cresult[0]['msg_count_deduction']
        
        #count amount_deduction record number
        csql = "select count(*) as msg_count_deduction_suc,sum(fee) as amount_deduction from wraith_message where %s and report=1 and mo_status='ok' and forward_status in(1,2,3,6,7) " % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_deduction_suc = cresult[0]['msg_count_deduction_suc']
        amount_deduction = cresult[0]['amount_deduction'] if cresult[0]['amount_deduction']!='None' else '0'
        
        #成功转发的mo数量            
        #count msg_count_forward and amount_forward record number
        csql = "select count(*) as msg_count_forward_mo from wraith_message where %s and forward_mo_result='1' " % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_forward_mo = cresult[0]['msg_count_forward_mo']
        
        
        #成功转发的mr数量和金额            
        #count msg_count_forward and amount_forward record number
        csql = "select count(*) as msg_count_forward_mr,sum(fee) as amount_forward from wraith_message where %s and forward_mr_result='1' " % (where_clause)
        #logging.info(csql)
        cresult = mysql.queryAll(csql)
        msg_count_forward_mr = cresult[0]['msg_count_forward_mr']
        amount_forward = cresult[0]['amount_forward'] if cresult[0]['amount_forward']!='None' else '0'
        
        #insert or update
        csql = "select * from wraith_statistic where stat_time='%s' and gwid='%s' and feetype='%s' and is_agent='%s' and cmdID='%s' and spID='%s' and serviceID='%s' and cpID='%s' and cpProdID='%s' and province='%s'" \
        %(stat_hour,row['gwid'],row['feetype'],row['is_agent'],row['cmdID'],row['spID'],row['serviceID'],row['cpID'],row['cp_productID'],row['province'])
        cresult = mysql.queryAll(csql)
        #logging.info(csql)
        if(len(cresult)>0):
            csql = "update wraith_statistic set msg_count_all='%s',msg_count_legal='%s',msg_count_suc='%s',msg_count_deduction='%s',msg_count_deduction_suc='%s',amount_suc='%s',amount_deduction='%s',msg_count_forward_mo='%s',msg_count_forward_mr='%s',amount_forward='%s' where stat_time='%s' and gwid='%s' and feetype='%s' and is_agent='%s' and cmdID='%s' and spID='%s' and serviceID='%s' and cpID='%s' and cpProdID='%s' and province='%s'" \
            %(msg_count_all,msg_count_legal,msg_count_suc,msg_count_deduction,msg_count_deduction_suc,amount_suc,amount_deduction,msg_count_forward_mo,msg_count_forward_mr,amount_forward,stat_hour,row['gwid'],row['feetype'],row['is_agent'],row['cmdID'],row['spID'],row['serviceID'],row['cpID'],row['cp_productID'],row['province'])
        else:
            csql = "insert into wraith_statistic(stat_time,gwid,feetype,is_agent,cmdID,spID,serviceID,cpID,cpProdID,province,msg_count_all,msg_count_legal,msg_count_suc,msg_count_deduction,msg_count_deduction_suc,amount_suc,amount_deduction,msg_count_forward_mo,msg_count_forward_mr,amount_forward)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')" \
            % (stat_hour,row['gwid'],row['feetype'],row['is_agent'],row['cmdID'],row['spID'],row['serviceID'],row['cpID'],row['cp_productID'],row['province'],msg_count_all,msg_count_legal,msg_count_suc,msg_count_deduction,msg_count_deduction_suc,amount_suc,amount_deduction,msg_count_forward_mo,msg_count_forward_mr,amount_forward)
        logging.info(csql)
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

