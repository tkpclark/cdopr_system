#encoding:utf-8
'''
class Mobile_dict:
    
    __m_dict__ = {}
    __t__ = ','
    def load_mobile_dict(self):
    
        fd = open('/home/app/wraith/conf/code-utf8.dict', 'rb')
        for fitem in fd.readlines():
            if len(fitem.split(self.__t__)) == 3:
                province, area, num = fitem.split(',')
                province = province.strip()
                area =  area.strip()
                num = num.strip()
    
                if self.__m_dict__.has_key(num):
                    #print "mobiledict.config double key:", num
                    pass
                else:
                    self.__m_dict__[num] = {}
                    self.__m_dict__[num]['province'] = province
                    self.__m_dict__[num]['area'] = area
            else:
                print "mobiledict.config err:", fitem
        fd.close()
    def get_mobile_area(self, phone_code):
        if self.__m_dict__.has_key(phone_code):
            #print self.__m_dict__[phone_code]['area']
            return (self.__m_dict__[phone_code]['province'],self.__m_dict__[phone_code]['area'])
        else:
            return ('未知','未知')
'''

class Mobile_dict:
    
    __m_dict__ = {}
    __t__ = ','
    def load_mobile_dict(self):
      
        fd = open('../../conf/code-utf8.dict', 'rb')
        i = 0
        for fitem in fd.readlines():
            if len(fitem.split(self.__t__)) == 6:
                code_start, code_end, province, area, l1,l2 = fitem.split(',')
                code_start =  code_start.strip()
                code_end =  code_end.strip()
                province = province.strip()
                area =  area.strip()
    
                if self.__m_dict__.has_key(i):
                    #print "mobiledict.config double key:", num
                    pass
                else:
                    self.__m_dict__[i] = {}
                    self.__m_dict__[i]['code_start'] = code_start
                    self.__m_dict__[i]['code_end'] = code_end
                    self.__m_dict__[i]['province'] = province
                    self.__m_dict__[i]['area'] = area
                    i += 1
            else:
                print "mobiledict.config err:", fitem
        fd.close()
        print  "records loaded!"
        
    def get_mobile_area(self, phone_number):
        for i in range(len(self.__m_dict__)):
            if( (phone_number >= self.__m_dict__[i]['code_start']) and (phone_number <= self.__m_dict__[i]['code_end']) ):
                return (self.__m_dict__[i]['province'],self.__m_dict__[i]['area'])
        
        #cant find
        return ('未知','未知')
            
                