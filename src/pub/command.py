#encoding:utf-8
from Mydb import mysql
import logging
class Command:
    
    __cmd_dict__ = []
    def load_dict(self):
        sql = '''
        select t1.id as cmdID,t1.sp_number as cmd_spnumber,t1.mo_cmd as cmd_mocmd,
        coalesce(t2.ID,0) as cpID,
        coalesce(t2.cpname,'')as cpname,
        coalesce(t3.id,0) as cp_productID,
        coalesce(t3.`name`,'') as cp_product_name,
        coalesce(t3.mourl,'') as mourl,
        coalesce(t3.mrurl,'') as mrurl,
        coalesce(t3.forward_mo_module,'') as forward_mo_module,
        coalesce(t3.forward_mr_module,'') as forward_mr_module,
        coalesce(t1.open_province,'') as open_province,
        coalesce(t1.forbidden_area,'') as forbidden_area,
        coalesce(t4.ID , 0) as spID,
        coalesce(t4.sp_id,'') as sp_id,
        coalesce(t4.spname,'') as spname,
        coalesce(t5.ID , 0) as serviceID,
        coalesce(t5.`name`, '') as service_name,
        coalesce(t5.sp_number ,'') as serv_spnumber,
        coalesce(t5.mo_cmd,'') as serv_mocmd
        from mtrs_cmd t1
        left join mtrs_cp_product t3 on t1.cpProdID=t3.id
        left join mtrs_cp t2 on t3.cpID=t2.id
        left join mtrs_service t5 on t1.serviceID=t5.ID 
        left join mtrs_sp t4 on t5.spID=t4.ID
        '''
        self.__cmd_dict__ = mysql.queryAll(sql)
        logging.info(sql)
        #logging.info("loadding cmds:%s",self.__cmd_dict__)
        #print 'cmd_info loaded'
        for i in range(len(self.__cmd_dict__)):
            logging.info(self.__cmd_dict__[i])
        
    def get_cmd_info(self, cmdID):
        #print "cmdID:%s"%cmdID
        for i in range(len(self.__cmd_dict__)):
            if (cmdID == self.__cmd_dict__[i]['cmdID']):
                return (self.__cmd_dict__[i])
        
        #cant find
        return {}
            
                