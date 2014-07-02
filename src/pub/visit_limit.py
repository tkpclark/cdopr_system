#encoding:utf-8
import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
import redis
from Mydb import mysql
import datetime

#日月限一共分三组
#每个省内用户的日月限
#每个省的总量日月限
#所有用户总量的日月限
#这些限制都是针对某指令来设置的（cmd）
class Visit_limit:
    __v_dict__ = []
    r = False
    def __init__(self):
        self.r = redis.StrictRedis(host='localhost', port=6379, db=0)
        
    def load_dict(self):
        sql = "select * from wraith_visit_limit"
        self.__v_dict__ = mysql.queryAll(sql)
        #print self.__v_dict__
    #def get_user_visit_count(self,phone_number,province,cmdID):
    
    
    #设置所有省所有用户的总日访问次数计数，并返回计数结果
    def set_cmd_all_visit_count_daily(self,cmdID):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        
        
        key = 'a1_%s_%s' % (cmdID,day)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            self.r.setex(key,86400,1)
            return 1
        else:
            return self.r.incr(key)
    
    #获取所有省所有用户的总日访问次数计数，并返回计数结果
    def get_cmd_all_visit_count_daily(self,cmdID):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        
        
        key = 'a1_%s_%s' % (cmdID,day)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            return 0
        else:
            return self.r.get(key)      
            
      
    #设置所有省所有用户的总月访问次数计数，并返回计数结果
    def set_cmd_all_visit_count_monthly(self,cmdID):
        now = datetime.datetime.now()
        month = now.strftime('%Y%m')
        
        
        ###daily
        key = 'a2_%s_%s' % (cmdID,month)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            self.r.setex(key,2764800,1)
            return 1
        else:
            return self.r.incr(key)
    #获取所有省所有用户的总月访问次数计数，并返回计数结果
    def get_cmd_all_visit_count_monthly(self,cmdID):
        now = datetime.datetime.now()
        month = now.strftime('%Y%m')
        
        
        ###daily
        key = 'a2_%s_%s' % (cmdID,month)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            return 0
        else:
            return self.r.get(key)
        
    
    
    #设置省内所有用户的总日访问次数计数，并返回计数结果
    def set_cmd_prov_all_visit_count_daily(self,cmdID,province):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        
        
        key = 'p1_%s_%s_%s' % (cmdID,province,day)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            self.r.setex(key,86400,1)
            return 1
        else:
            return self.r.incr(key)
            
    #获取省内所有用户的总日访问次数计数，并返回计数结果
    def get_cmd_prov_all_visit_count_daily(self,cmdID,province):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        
        
        key = 'p1_%s_%s_%s' % (cmdID,province,day)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            return 0
        else:
            return self.r.get(key)
              
      
    #设置省内所有用户的总月访问次数计数，并返回计数结果
    def set_cmd_prov_all_visit_count_monthly(self,cmdID,province):
        now = datetime.datetime.now()
        month = now.strftime('%Y%m')
        
        
        ###daily
        key = 'p2_%s_%s_%s' % (cmdID,province,month)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            self.r.setex(key,2764800,1)
            return 1
        else:
            return self.r.incr(key)  
        
    #获取省内所有用户的总月访问次数计数，并返回计数结果
    def get_cmd_prov_all_visit_count_monthly(self,cmdID,province):
        now = datetime.datetime.now()
        month = now.strftime('%Y%m')
        
        
        ###daily
        key = 'p2_%s_%s_%s' % (cmdID,province,month)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            return 0
        else:
            return self.r.get(key)      
        
    #设置省内用户的日访问次数计数，并返回计数结果
    def set_cmd_prov_user_visit_count_daily(self,cmdID,province,phone_number):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        
        
        key = 'u1_%s_%s_%s' % (cmdID,phone_number,day)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            self.r.setex(key,86400,1)
            return 1
        else:
            return self.r.incr(key)
            
    #获取省内用户的日访问次数计数，并返回计数结果
    def get_cmd_prov_user_visit_count_daily(self,cmdID,province,phone_number):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        
        
        key = 'u1_%s_%s_%s' % (cmdID,phone_number,day)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            return 0
        else:
            return self.r.get(key)        
      
    #设置省内用户的月访问次数计数，并返回计数结果
    def set_cmd_prov_user_visit_count_monthly(self,cmdID,province,phone_number):
        now = datetime.datetime.now()
        month = now.strftime('%Y%m')
        
        
        ###daily
        key = 'u2_%s_%s_%s' % (cmdID,phone_number,month)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            self.r.setex(key,2764800,1)
            return 1
        else:
            return self.r.incr(key)
        
    #获取省内用户的月访问次数计数，并返回计数结果
    def get_cmd_prov_user_visit_count_monthly(self,cmdID,province,phone_number):
        now = datetime.datetime.now()
        month = now.strftime('%Y%m')
        
        
        ###daily
        key = 'u2_%s_%s_%s' % (cmdID,phone_number,month)
        #self.r.INCR(key)
        if(self.r.exists(key)==False):
            return 0
        else:
            return self.r.get(key)
          
    ##############################################################################
    #获取数据库中设置的全业务门限值
    def get_cmd_all_visit_limit(self,cmdID):        
        #exactly
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == '全部') and (record['limit_type'] == '2')):
                 return (record['daily_limit'],record['monthly_limit'])
        
        #没设置则不限制
        return (0,0);
    
    
    #获取数据库中设置的省内用户的总门限值
    def get_cmd_prov_all_visit_limit(self,cmdID,province):        
        #exactly
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == province) and (record['limit_type'] == '2')):
                 return (record['daily_limit'],record['monthly_limit'])
            
        #default of a cmd
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == '默认') and (record['limit_type'] == '2')):
                return (record['daily_limit'],record['monthly_limit'])
        
        #没设置则不限制
        return (0,0);
    
    
    
    
     #获取数据库中设置的全业务转发门限值（全国转发总量）
    def get_cmd_all_forward_limit(self,cmdID):        
        #exactly
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == '全部') and (record['limit_type'] == '3')):
                 return (record['daily_limit'],record['monthly_limit'])
        
        #没设置则不限制
        return (0,0);
    
    
    #获取数据库中设置的省内用户的转发总门限值
    def get_cmd_prov_all_forward_limit(self,cmdID,province):        
        #exactly
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == province) and (record['limit_type'] == '3')):
                 return (record['daily_limit'],record['monthly_limit'])
            
        #default of a cmd
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == '默认') and (record['limit_type'] == '3')):
                return (record['daily_limit'],record['monthly_limit'])
        
        #没设置则不限制
        return (0,0);
    
    
    
    
    
    
    
    
    #获取数据库中设置的单用户门限值
    def get_cmd_prov_user_visit_limit(self,cmdID,province):        
        #exactly
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == province) and (record['limit_type'] == '1')):
                 return (record['daily_limit'],record['monthly_limit'])
            
        #default of a cmd
        for record in self.__v_dict__:
            #print record['cmdID'],record['province']
            #print cmdID,province
            if( (record['cmdID'] == cmdID) and (record['province'] == '默认') and (record['limit_type'] == '1')):
                return (record['daily_limit'],record['monthly_limit'])
        
        #没设置则不限制
        return (0,0);
    
    
    
    
    
    
    #################################################################################
    
    #判断用户访问是否达到各种上线 全业务/省内总来那个/省内单用户
    def is_arrive_limit(self,phone_number, cmdID, province):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        month = now.strftime('%Y%m')
        
        cmd_prov_user_visit_count_daily_limit,cmd_prov_user_visit_count_monthly_limit = self.get_cmd_prov_user_visit_limit(cmdID, province)
        if((int)(cmd_prov_user_visit_count_daily_limit) and \
           (int)(self.get_cmd_prov_user_visit_count_daily(cmdID, province,phone_number)) >= (int)(cmd_prov_user_visit_count_daily_limit)):
            return ('1','用户超日限')
            
        if((int)(cmd_prov_user_visit_count_monthly_limit) and \
           (int)(self.get_cmd_prov_user_visit_count_monthly(cmdID, province, phone_number)) >= (int)(cmd_prov_user_visit_count_monthly_limit)):
            return  ('2','用户超月限')
             
             
        cmd_prov_all_visit_count_daily_limit,cmd_prov_all_visit_count_monthly_limit = self.get_cmd_prov_all_visit_limit(cmdID, province)
        if((int)(cmd_prov_all_visit_count_daily_limit) and \
           (int)(self.get_cmd_prov_all_visit_count_daily(cmdID, province)) >= (int)(cmd_prov_all_visit_count_daily_limit)):
            return ('3','省总量超日限')
            
        if((int)(cmd_prov_all_visit_count_monthly_limit) and \
           (int)(self.get_cmd_prov_all_visit_count_monthly(cmdID, province)) >= (int)(cmd_prov_all_visit_count_monthly_limit)):
            return ('4','省总量超月限')
            
            
        cmd_all_visit_count_daily_limit,cmd_all_visit_count_monthly_limit = self.get_cmd_all_visit_limit(cmdID)
        if((int)(cmd_all_visit_count_daily_limit) and \
           (int)(self.get_cmd_all_visit_count_daily(cmdID)) >= (int)(cmd_all_visit_count_daily_limit)):
            return ('5','总量超日限')
            
        if((int)(cmd_all_visit_count_monthly_limit) and \
           (int)(self.get_cmd_all_visit_count_monthly(cmdID)) >= (int)(cmd_all_visit_count_monthly_limit)):
            return ('6','总量超月限')
           
         
         
        
        #limit of the first 9 bits of the phone_number
        '''
        group_visit_count_dayily=self.set_user_visit_count_daily(phone_number[0:-2], cmdID, province)
        if((int)(group_visit_count_dayily)) > 80:
            return 3
        '''
        
        #once you got here, means that no any limit break ,then return false
        #set visit count
        self.set_cmd_prov_user_visit_count_daily(cmdID, province, phone_number)
        self.set_cmd_prov_user_visit_count_monthly(cmdID, province, phone_number)
        self.set_cmd_prov_all_visit_count_daily(cmdID, province)
        self.set_cmd_prov_all_visit_count_monthly(cmdID, province)
        self.set_cmd_all_visit_count_daily(cmdID)
        self.set_cmd_all_visit_count_monthly(cmdID)
        return ('0','ok')
        
        
 
 
 
 #判断转发是否达到各种上线 全业务/省内总来那个/省内单用户
    def is_arrive_forward_limit(self,phone_number, cmdID, province):
        now = datetime.datetime.now()
        day = now.strftime('%Y%m%d')
        month = now.strftime('%Y%m')
   
             
        cmd_prov_all_forward_count_daily_limit,cmd_prov_all_forward_count_monthly_limit = self.get_cmd_prov_all_forward_limit(cmdID, province)
        print cmd_prov_all_forward_count_daily_limit,cmd_prov_all_forward_count_monthly_limit
        if((int)(cmd_prov_all_forward_count_daily_limit) and \
           (int)(self.get_cmd_prov_all_visit_count_daily(cmdID, province)) >= (int)(cmd_prov_all_forward_count_daily_limit)):
            return (13,'转发省总量超日限')
            
        if((int)(cmd_prov_all_forward_count_monthly_limit) and \
           (int)(self.get_cmd_prov_all_visit_count_monthly(cmdID, province)) >= (int)(cmd_prov_all_forward_count_monthly_limit)):
            return (14,'转发省总量超月限')
            
            
        cmd_all_forward_count_daily_limit,cmd_all_forward_count_monthly_limit = self.get_cmd_all_forward_limit(cmdID)
        if((int)(cmd_all_forward_count_daily_limit) and \
           (int)(self.get_cmd_all_visit_count_daily(cmdID)) >= (int)(cmd_all_forward_count_daily_limit)):
            return (15,'转发总量超日限')
            
        if((int)(cmd_all_forward_count_monthly_limit) and \
           (int)(self.get_cmd_all_visit_count_monthly(cmdID)) >= (int)(cmd_all_forward_count_monthly_limit)):
            return (16,'转发总量超月限')
        
        return (0,'0')
           
         
         
                
if __name__ == "__main__":
    visit_limit = Visit_limit()
    visit_limit.load_dict()
    #print visit_limit.get_user_visit_limit('3','山东'),
    #print visit_limit.set_user_visit_count_daily('13810002000','1','山东'),
    #print visit_limit.set_user_visit_count_monthly('13810002000','1','山东')
    
    print visit_limit.is_arrive_limit('13810002001','54','山东')