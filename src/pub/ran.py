#encoding:utf-8

import sys
import os
import string
import random

def in_po(p):
    po=100*p
    a=random.randint(1,100)
    if(a <= po):
        return True
    else:
        return False
    
def main():
    
    for i in range(string.atoi(sys.argv[1])):
       print in_po(0.21)
        #phone_number='133%s'%("".join(random.sample('01234567890123456789012345678901234567890123456789',8)))
        #mo_message=random.choice(['112', '1232'])
        #gwid=random.randint(1,20)
        
if __name__ == "__main__":
    main()