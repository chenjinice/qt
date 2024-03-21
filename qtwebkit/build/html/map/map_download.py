from urllib import request
from urllib.request	import urlopen
from urllib import parse
from enum import Enum, auto
import re , os , sys ,math, time , random



agents = []
agents.append('Mozilla/5.0 (Windows; U; MSIE 9.0; Windows NT 9.0; en-US)')
agents.append('Mozilla/5.0 (compatible; MSIE 10.0; Macintosh; Intel Mac OS X 10_7_3; Trident/6.0)')
agents.append('Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0; GTB7.4; InfoPath.2; SV1; .NET CLR 3.3.69573; WOW64; en-US)')
agents.append('Opera/9.80 (X11; Linux i686; U; ru) Presto/2.8.131 Version/11.11')
agents.append('Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.2 (KHTML, like Gecko) Chrome/22.0.1216.0 Safari/537.2')
agents.append('Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/537.13 (KHTML, like Gecko) Chrome/24.0.1290.1 Safari/537.13')
agents.append('Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11')
agents.append('Mozilla/5.0 (Windows NT 6.2; Win64; x64; rv:16.0.1) Gecko/20121011 Firefox/16.0.1')
agents.append('Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:15.0) Gecko/20100101 Firefox/15.0.1')
agents.append('Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5355d Safari/8536.25')

#枚举
class Ret(Enum):
	EXIST = auto()
	ERROR = auto()
	OK = auto()


def getAgent():
	count = len(agents)
	index = random.randint(0,count-1)
	return agents[index]


def getImage(x,y,z):
	x = str(x)
	y = str(y)
	z = str(z)
	url = "http://mt1.google.com/vt/lyrs=s&x="+x+"&y="+y+"&z="+z
	path = "./" + z + "/" + x;
	name = path + "/" + y + ".jpg"

	if os.path.exists(name) :
		print(name + ": exist ◯")
		return Ret.EXIST


	#伪造html头信息，防被禁ip，貌似没啥卵用
	headers = {
		'User-Agent': getAgent()
	}
	req = request.Request(url,headers = headers)
	try:
		data = urlopen(url).read()
	except Exception as e:
		print(name + ": read error  ✘ ")
		return Ret.ERROR

	mkDir(path)
	saveData(name,data)
	return Ret.OK

def mkDir(path):
	if not os.path.exists(path) :
		os.makedirs(path,0o777)


def saveData(name,data):
	with open(name,'wb') as f:
		f.write(data)
		print(name + ": saved ✔ ")


def getX(point1,point2,z):
	result = []
	x1 = pow(2,z-1)*(point1[1]/180.0+1)
	x2 = pow(2,z-1)*(point2[1]/180.0+1)
	x1 = int(x1)
	x2 = int(x2)
	if x1 > x2:
		result.append(x2)
		result.append(x1)
	else:
		result.append(x1)
		result.append(x2)
	return result


def getY(point1,point2,z):
	result = []
	temp1 = math.tan(math.pi*point1[0]/180.0)+1.0/math.cos(math.pi*point1[0]/180.0);
	temp2 = math.tan(math.pi*point2[0]/180.0)+1.0/math.cos(math.pi*point2[0]/180.0);
	y1 = pow(2,z-1)*(1-math.log(temp1)/math.pi); 
	y2 = pow(2,z-1)*(1-math.log(temp2)/math.pi); 
	y1 = int(y1)
	y2 = int(y2)
	if y1 > y2:
		result.append(y2)
		result.append(y1)
	else:
		result.append(y1)
		result.append(y2)
	return result


def myDelay():
	value = random.randint(5,30)
	time.sleep(value)
	print(" ------ sleep  %d  seconds ------" %(value) )



point1 = [28.104800, 112.857940]
point2 = [28.116378, 112.870891]
z_range = [22,22] 

i = 1
for z in range(z_range[0],z_range[1]+1):
	x_range = getX(point1,point2,z)
	y_range = getY(point1,point2,z)
	for x in range(x_range[0],x_range[1]+1):
		# print("[z:%d/%d,x:%d/%d] : " %(z,z_range[1],x,x_range[1]));
		for y in range(y_range[0],y_range[1]+1):
			print("[y:%d/%d,x:%d/%d,z:%d/%d] : " %(y,y_range[1],x,x_range[1],z,z_range[1]),end='');
			ret = getImage(x,y,z)

			if ret == Ret.OK: 
				i+=1
				if (i%50) == 0 : myDelay()
			# if (i%60) == 0 : time.sleep(20)


		
print("================== end end end end end end  ====================== ")




while True:
	continue


