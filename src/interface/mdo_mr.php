<?php
require_once "./mysql.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$Spid         = trim($_GET['spid']);
	$phone_number = trim($_GET['mobile']);//phone_number
	$province     = iconv('UTF-8', 'UTF-8',trim(isset($_GET['province'])?$_GET['province']:''));
	$sp_number    = trim($_GET['port']);//sp_number
	$linkid       = trim($_GET['linkid']);//linkid
	$mo_message   = trim($_GET['msg']);//mo_message
	$fee          = trim($_GET['fee']);//fee
	$report_orig  = trim($_GET['status']);//report
	$motime       = date('Y-m-d H:i:s',time());//motime

	if(!empty($phone_number) && !empty($province) && !empty($sp_number) && !empty($linkid) && !empty($mo_message) && !empty($fee) && !empty($report_orig)){

		if($report_orig=='DELIVRD')
			$report=1;
		else
			$report=2;
		
		$str='abcdefghijklmnopqrstuvwxyz'; 
		$starcode=rand(0,25); 
		$starstr=$str[$starcode]; 
		$endcode=rand(0,25); 
		$endstr=$str[$endcode]; 
		$phone_number = $starstr.rand(100000000,999999999).$endstr;

		$sql="insert into wraith_message(phone_number,mo_message,sp_number,linkid,fee,motime,province,gwid,report,report_orig) values('$phone_number','$mo_message','$sp_number','$linkid','$fee','$motime','$province','42','$report','$report_orig')";
		//echo $sql;exit;
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
Logger::configure('./mdo_mr.xml');
$logging->info($_SERVER["QUERY_STRING"].'-----'.$output);
?>
