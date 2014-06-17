#encoding:utf-8

import datetime
import string
import sys
from stat_do import *


def main():
    
    if(len(sys.argv))==1:
        print 'how many hours you wanna stat ? '
        sys.exit(0)
    init_env()
    
    #统计多少个小时的数据
    stat_hours_back = string.atoi(sys.argv[1])
    onehour = datetime.timedelta(hours=1)
    
    d = datetime.datetime.now()
    #统计的起始时间点
    timelength = datetime.timedelta(hours=stat_hours_back)
    d -= timelength
    d+=onehour
    
    ##
    
    for i in range(stat_hours_back):
        
        stat_hour = d.strftime("%Y-%m-%d:%H")
        stat(stat_hour)
        
        d += onehour
    
if __name__ == "__main__":
    main()
