<?php
require_once "./mysql.php";
require_once "./CityCode.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$phone_number  = trim($_GET['Mobile']);//phone_number
	$sp_number     = trim($_GET['SpNumber']);//sp_number
	$linkid        = trim($_GET['LinkID']);//linkid
	$mo_message    = urldecode(trim($_GET['MoContent']));//mo_message
	$fee           = trim($_GET['FeeCode']);//fee
	$motime        = trim($_GET['MoTime']);//motime
	$City          = trim($_GET['City']);//province,area
	$City          = iconv("UTF-8", "UTF-8", urldecode($City));
//echo $City;exit;
	if(!empty($phone_number) && !empty($sp_number) && !empty($linkid) && !empty($mo_message) && !empty($fee) && !empty($motime) && !empty($City)){
		//获取省市
		$arr = explode(',',$City);
		if(!empty($arr)){
			$province=$arr[0];
			$area=$arr[1];
		}
		if(!isset($province) && !isset($province)){
			throw new Exception ( '401');
		}
		$sql="insert into wraith_message(phone_number,mo_message,sp_number,linkid,fee,motime,province,area,gwid) values('$phone_number','$mo_message','$sp_number','$linkid','$fee','$motime','$province','$area','41')";
		if($result = exsql($sql)){
			$output = 'ok';
		}
	}
	
}catch(Exception $e) {
	$output =  $e->getMessage();
}
//根据接口规范返回输出结果
echo $output;
//打印日志
$logging = Logger::getLogger('sjdy_mo');
Logger::configure('./sjdy_mo.xml');
$logging->info($_SERVER["QUERY_STRING"].'-----'.$output);
?>
