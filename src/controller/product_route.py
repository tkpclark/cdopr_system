#product is just cmd s


from Mydb import mysql
import random
import logging
class Product_route:
    
    __products__ = []
    contents = {}
    __t__ = '*'
    
    def load_products(self):
        sql =  '''select t1.ID as cmdID,t1.app_module,t1.mo_cmd,t1.sp_number,t1.is_agent,
        t2.fee,t2.feetype,t2.msgtype,t2.gwid,t1.open_province,t1.forbidden_area,t2.service_id
        from mtrs_cmd t1, mtrs_service t2
        where t1.serviceID=t2.ID and t1.status=1'''
        self.__products__ = mysql.queryAll(sql);
        logging.info(sql)
        #logging.info('all cmdinfo: %s',self.__products__)
        for i in range(len(self.__products__)):
            logging.info(self.__products__[i])
        
        #load conents
        default_content={}
        default_content['content']=""
        for product in self.__products__:
            sql = "select content from mtrs_cmd_mt where cmdID="+product['cmdID']
            one_prod_contents = mysql.queryAll(sql);
            if(len(one_prod_contents)==0):
                one_prod_contents.append(default_content)
            self.contents[product['cmdID']]=one_prod_contents
        
        #print self.contents
        #print 'products loaded'
    def get_random_content(self,product_id):      
        ran_number = random.randint(0,len(self.contents[product_id])-1)
        return self.contents[product_id][ran_number]['content']
        
        
        
    def __probably_match__(self,gwid, sp_number, message,province):
            
        ##
        i = 0
        for m in range(100):
            i += 1
            if(len(sp_number) <= 9 and sp_number.find(self.__t__)>0):
                break
            if(i != 1):
                sp_number = self.__extend_prob__(sp_number)
            _message = message
            
            j=0
            for n in range(100):
                j += 1
                if(_message == self.__t__):
                    break
                if(j != 1):
                    _message = self.__extend_prob__(_message)       
                print gwid,sp_number,_message,province
                product = self.__search__(gwid, sp_number, _message, province)
                if(product != False):
                    return product
            
        return False
    
    def match(self, gwid, sp_number, message,province):
        #there is  "tel:" before ctcc spnumber, need to remove it
        if(sp_number[0:4] == 'tel:'):
            sp_number = sp_number[4:]
        #accurately first    
        product = self.__search__(gwid, sp_number, message, province)
        if(product != False):
            return product
        #probably
        product = self.__probably_match__(gwid, sp_number, message, province)
        if(product != False):
            return product
        
        return {}
    
    def __search__(self,gwid, sp_number, message,province):
        for product in self.__products__:
            #print product
            #print "%s,%s,%s | %s,%s,%s"%(gwid, sp_number, message.lower(), product['gwid'], product['sp_number'], product['mo_cmd'].lower())
            if(product['gwid'] == gwid) and (product['sp_number'] == sp_number) and (product['mo_cmd'].lower() == message.lower()) and (province in product['open_province']):
                return product
        return False
   
    def __extend_prob__(self,str):
        if(str.find(self.__t__) < 0):
            str += self.__t__
        else:
            str = str[0:-2] + self.__t__
        return str
    
    
