from Mydb import mysql
class Blklist:
    
    __blklist__ = []
    __t__ = '*'
    
    def load_blklist(self):
        sql = "select phone_number from wraith_blklist"
        self.__blklist__ = mysql.queryAll(sql);
        #print self.__blklist__
    
    def match(self, phone_number):
        
        for item in self.__blklist__:
          if(item['phone_number'] == phone_number):
              return True
        return False
   
    
    
