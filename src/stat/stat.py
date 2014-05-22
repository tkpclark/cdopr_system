import sys
import os
import time
from Mydb import mysql
import logging
import datetime
from logging.handlers import RotatingFileHandler
def stat(stat_hour):
    
    print "***stat hour: " + stat_hour
    db_stat_hour = "DATE_FORMAT(in_time,'%Y-%m-%d:%H')"
    #db_stat_hour = sys.argv[1]
    
    #delete old days if
    sql = "delete from wraith_statistic where stat_time = '%s'" % (stat_hour)
    print sql
    mysql.query(sql)
    
    ##group
    sql = "select gwid,sp_number,product_id,product_code,amount,province,count(*) as num from `wraith_mt` where %s='%s' group by gwid,sp_number,product_id,product_code,province" % (db_stat_hour,stat_hour)
    print sql
    result = mysql.queryAll(sql)
    if(mysql.rowcount()>0):
        for row in result:
            where_clause = " %s='%s' and gwid='%s' and sp_number='%s' and product_id='%s' and product_code='%s' and amount='%s' and province='%s' "%(db_stat_hour,stat_hour,row['gwid'],row['sp_number'],row['product_id'],row['product_code'],row['amount'],row['province'])
            print "num: " + row['num']
            #count sucessful record number:
            csql = "select count(*) as success_num, sum(amount) as success_amount from wraith_mt where %s and (report = '4' or report  ='DELIVRD' or report = '0') " % (where_clause)
            print csql
            cresult = mysql.queryAll(csql)
            success_num = cresult[0]['success_num']
            success_amount = cresult[0]['success_amount'] if cresult[0]['success_amount'] else 0
            #count other
            
            
            #insert
            csql = "insert into wraith_statistic(gwid,sp_number,product_id,product_code,amount,stat_time,province,num,success_num,all_amount)values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')" % (row['gwid'],row['sp_number'],row['product_id'],row['product_code'],row['amount'],stat_hour,row['province'],row['num'],success_num,success_amount)
            print csql
            mysql.query(csql)
def init_env():
    
    #chdir
    os.chdir(sys.path[0])
    
    #init logging
    '''
    logfile = '/home/app/wraith/logs/controller/controller.log'
    Rthandler = RotatingFileHandler(logfile, maxBytes=10*1024*1024,backupCount=5)
    formatter = logging.Formatter('[%(asctime)s][%(levelname)s][1.00]:  %(message)s - %(filename)s:%(lineno)d')
    Rthandler.setFormatter(formatter)
    logger=logging.getLogger()
    logger.addHandler(Rthandler)
    logger.setLevel(logging.NOTSET)
    '''

def main():
    init_env()
    
    d = datetime.datetime.now()
    onehour = datetime.timedelta(hours=1)
    for i in range(1):
        d -= onehour
        stat_hour = d.strftime("%Y-%m-%d:%H")
        stat(stat_hour)
    
if __name__ == "__main__":
    main()
