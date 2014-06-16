<?php
require_once "./mysql.php";
require_once "./CityCode.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$Spid     = trim($_GET['Spid']);
	$Src      = trim($_GET['Src']);//phone_number
	$CityCode = trim($_GET['CityCode']);//province+area
	$Dest     = trim($_GET['Dest']);//sp_number
	$Linkid   = trim($_GET['Linkid']);//linkid
	$Cmd      = trim($_GET['Cmd']);//mo_message
	$Fee      = trim($_GET['Fee']);//fee
	$Svcid    = trim($_GET['Svcid']);
	$Time     = trim($_GET['Time']);//motime
	
	if(!empty($Src) && !empty($CityCode) && !empty($Dest) && !empty($Linkid) && !empty($Cmd) && !empty($Fee) && !empty($Time)){
		//获取省市
		foreach($CityCode_arr as $v){
			if($v[2]==$CityCode){
				$province=$v[0];
				$area=$v[1];
			}
		}
		if(!isset($province) && !isset($province)){
			throw new Exception ( '1');
		}
		$sql="insert into wraith_message(phone_number,mo_message,sp_number,linkid,fee,motime,province,area,gwid) values('$Src','$Cmd','$Dest','$Linkid','$Fee','$Time','$province','$area','37')";
		if($result = exsql($sql)){
			$output = 0;
		}
	}
	
}catch(Exception $e) {
	$output =  $e->getMessage();
}
//根据接口规范返回输出结果
echo $output;
//打印日志
$logging = Logger::getLogger('rent_recv');
Logger::configure('./a001_mo.xml');
$logging->info($_SERVER["QUERY_STRING"].'-----'.$output);
?>
