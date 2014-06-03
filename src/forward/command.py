#encoding:utf-8
from Mydb import mysql
import logging
class Command:
    
    __cmd_dict__ = []
    def load_dict(self):
        sql = '''
        select t1.id as cmdID,t1.sp_number as cmd_spnumber,t1.mo_cmd as cmd_mocmd,t2.ID as cpID,t2.cpname,
        t3.id as cp_productID,t3.`name` as cp_product_name,t3.mourl,
        t3.mrurl,t3.forward_mo_module,t3.forward_mr_module,t1.open_province,t1.forbidden_area,
        t4.ID as spID, t4.sp_id,t4.spname,
        t5.ID as serviceID, t5.`name` as service_name,t5.sp_number as serv_spnumber,t5.mo_cmd as serv_mocmd
        from mtrs_cmd t1, mtrs_cp t2, mtrs_cp_product t3, mtrs_sp t4, mtrs_service t5
        where t1.cpProdID=t3.id and t3.cpID=t2.id and t1.serviceID=t5.ID and t5.spID=t4.ID
        '''
        self.__cmd_dict__ = mysql.queryAll(sql)
        logging.info(sql)
        logging.info("loadding cmds:%s",self.__cmd_dict__)
        #print 'cmd_info loaded'
        
    def get_cmd_info(self, cmdID):
        #print "cmdID:%s"%cmdID
        for i in range(len(self.__cmd_dict__)):
            if (cmdID == self.__cmd_dict__[i]['cmdID']):
                return (self.__cmd_dict__[i])
        
        #cant find
        return {}
            
                