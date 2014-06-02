import sys
import os
os.chdir(sys.path[0])
sys.path.append('../pub')
from Mydb import mysql
from product_route import *

product_route = Product_route()
product_route.load_products()
#print product_route.match('1','10668888123','bdcd')
result = product_route.match(sys.argv[1],sys.argv[2],sys.argv[3])
print result
