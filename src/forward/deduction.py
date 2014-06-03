#encoding:utf-8
import sys
import os
from Mydb import mysql
import datetime

class Deduction:
    __deduction_dict__ = []
    
    def __init__(self):
        pass
    
    def load_dict(self):
        sql = "select * from mtrs_deduction"
        self.__deduction_dict__ = mysql.queryAll(sql)
        #print self.__deduction_dict__
    #def get_user_visit_count(self,phone_number,zone,cpProdID):

        ###monthly   
    def get_deduction(self,cpProdID,zone):
        
        #exactly
        for record in self.__deduction_dict__:
            #print record['cpProdID'],record['zone']
            #print cpProdID,zone
            if( (record['cpProdID'] == cpProdID) and (record['zone'] == zone)):
                 return float(record['deduction'])
        #default of a product
        for record in self.__deduction_dict__:
            #print record['cpProdID'],record['zone']
            #print cpProdID,zone
            if( (record['cpProdID'] == cpProdID) and (record['zone'] == '默认')):
                return float(record['deduction'])
        
        #eventually default
        return 0;
        
        
        
if __name__ == "__main__":
    deduction = Deduction()
    deduction.load_dict()
