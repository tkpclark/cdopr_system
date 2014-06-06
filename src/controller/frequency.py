#encoding:utf-8
import sys
import os
import redis
import datetime
import logging

class Frequency:
    r = False
    def __init__(self):
        self.r = redis.StrictRedis(host='localhost', port=6379, db=0)
    #def get_user_visit_count(self,phone_number,province,cmdID):
    def rec_freq(self,phone_number,motime):

        key = 'freq_%s' % (phone_number)

        try:
            #key不存在，设置本次mo的值，然后返回True
            if(self.r.exists(key)==False):
                print 'key doesnt exist'
                self.r.setex(key,60,motime)
                return True
            #若key存在，检查是否这N秒内，若是则返回失败，不是则设置为本次motime，并返回成功
            else:
                latter = datetime.datetime.strptime(motime,'%Y-%m-%d %H:%M:%S')
                former = datetime.datetime.strptime(self.r.get(key),'%Y-%m-%d %H:%M:%S')
                print former,latter
                if ((latter-former).seconds < 3):
                    return False
                else:
                    self.r.setex(key,60,motime)
                    return True
        except:
            print 'fail'
            return True

if __name__ == "__main__":
    
    global frequency
    frequency = Frequency()
    
    print frequency.rec_freq('13810002000',sys.argv[1])
    