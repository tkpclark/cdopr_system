import logging
from logging.handlers import RotatingFileHandler
import urllib
import urllib2
import time
from forward import mysql

def update_forward_result(message_id,forward_result,type):
    global mysql
    sql = " update wraith_message set forward_%s_result='%d' where id='%s'"%(type,forward_result,message_id)    
    logging.info('dbsql:%s',sql)
    mysql.query(sql)
    time.sleep(20)
    
def f10(message_id,url):
    logging.info('f10 ok')
    time.sleep(1)
    update_forward_result(message_id,1,"mo")
def f11(message_id,url):
    logging.info('f11 ok')
    time.sleep(1)
    update_forward_result(message_id,2,"mt")