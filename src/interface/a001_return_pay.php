<?php
//支付服务器返回接口
require_once "./mysql.php";
require_once "../../../../log4php/php/Logger.php";
try{
	//$output = 1;
	$result        = trim(@$_REQUEST['result']);
	$paymentid     = trim(@$_REQUEST['paymentid']);
	$errorstr      = trim(@$_REQUEST['errorstr']);
	$company       = trim(@$_REQUEST['company']);
	$channelid     = trim(@$_REQUEST['channelid']);
	$softgood      = trim(@$_REQUEST['softgood']);
	$customer      = trim(@$_REQUEST['customer']);
	$money         = trim(@$_REQUEST['money']);
	$softserver    = trim(@$_REQUEST['softserver']);
	$playername    = trim(@$_REQUEST['playername']);
	$date          = trim(@$_REQUEST['date']);
	$pkey          = trim(@$_REQUEST['pkey']);
	$paytype       = trim(@$_REQUEST['paytype']);
	$customorderno = trim(@$_REQUEST['customorderno']);
	$gamecode      = trim(@$_REQUEST['gamecode']);

	

	
}catch(Exception $e) {
	$output =  $e->getMessage();
}
//根据接口规范返回输出结果
//echo $output;
//打印日志
$logging = Logger::getLogger('rent_recv');
Logger::configure('./a001_return_pay.xml');
$logging->info($_SERVER["QUERY_STRING"]);
?>
