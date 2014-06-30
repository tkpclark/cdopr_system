<?php
require_once "./mysql.php";
require_once "./CityCode.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$Spid     = trim($_GET['Spid']);
	$Src      = trim($_GET['Src']);
	$Status   = trim($_GET['Status']);//report
	$Linkid   = trim($_GET['Linkid']);//linkid
	$Cmd      = trim($_GET['Cmd']);
	
	if(!empty($Status) && !empty($Linkid)){
		if($Status=='DELIVRD')
			$report=1;
		else
			$report=2;

		$sql="update wraith_message set report='$report',report_orig='$Status' where linkid=$Linkid";
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
Logger::configure('./a001_mr.xml');
$logging->info($_SERVER["QUERY_STRING"].'-----'.$output);
?>
