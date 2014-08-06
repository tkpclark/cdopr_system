<?php
require_once "./mysql.php";
require_once "./CityCode.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$Mobile     = trim($_GET['Mobile']);
	$SpNumber   = trim($_GET['SpNumber']);
	$MoContent  = trim($_GET['MoContent']);
	$linkid     = trim($_GET['LinkID']);//linkid
	$Status     = trim($_GET['RptStat']);//report
	
	if(!empty($Status) && !empty($linkid)){
		if($Status=='DELIVRD'){
			$report=1;
		}else{
			$report=2;
		}
		$sql="update wraith_message set report='".$report."',report_orig='".$Status."' where linkid='".$linkid."'";
		//echo $sql;exit;
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
$logging = Logger::getLogger('sjdy_mr');
Logger::configure('./sjdy_mr.xml');
$logging->info($_SERVER["QUERY_STRING"].'-----'.$output.'--sql='.$sql);
?>
