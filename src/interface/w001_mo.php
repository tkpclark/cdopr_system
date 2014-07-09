<?php
require_once "./mysql.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	//mb_convert_encoding($outTradeNo,"UTF-8","GBK");  
	//iconv('', 'UTF-8', $outTradeNo); 
	$unicomId      = trim(isset($_POST['unicomId'])?$_POST['unicomId']:'');
	$outTradeNo    = trim(isset($_POST['outTradeNo'])?$_POST['outTradeNo']:'');
	$subject       = iconv('UTF-8', 'UTF-8',trim(isset($_POST['subject'])?$_POST['subject']:''));
	$description   = iconv('UTF-8', 'UTF-8',trim(isset($_POST['description'])?$_POST['description']:''));
	$price         = trim(isset($_POST['price'])?$_POST['price']:'');
	$quantity      = trim(isset($_POST['quantity'])?$_POST['quantity']:'');
	$totalFee      = trim(isset($_POST['totalFee'])?$_POST['totalFee']:'');
	$appKey        = trim(isset($_POST['appKey'])?$_POST['appKey']:'');
	$appName       = iconv('UTF-8', 'UTF-8',trim(isset($_POST['appName'])?$_POST['appName']:''));
	$callBackPath  = trim(isset($_POST['callBackPath'])?$_POST['callBackPath']:'');
	$timeStamp     = trim(isset($_POST['timeStamp'])?$_POST['timeStamp']:'');


	$log="unicomId=".$unicomId."outTradeNo=".$outTradeNo."&subject=".$subject."&description=".$description."&price=".$price."&quantity=".$quantity."&totalFee=".$totalFee."&appKey=".$appKey."&appName=".$appName."&callBackPath=".$callBackPath."&timeStamp=".$timeStamp;

	if(!empty($unicomId) && !empty($outTradeNo) && !empty($subject) && !empty($totalFee) && !empty($appKey) && !empty($appName) && !empty($callBackPath) && !empty($timeStamp)){
		$sql="insert into wraith_wo_sdk(unicomId ,outTradeNo,subject,description,price,quantity,totalFee,appKey,appName,callBackPath,timeStamp) values('$unicomId','$outTradeNo','$subject','$description','$price','$quantity','$totalFee','$appKey','$appName','$callBackPath','$timeStamp')";
		if($result = exsql($sql)){
			$output = 1;
		}
	}
	
}catch(Exception $e) {
	$output =  $e->getMessage();
}
//根据接口规范返回输出结果
echo $output;
//打印日志
$logging = Logger::getLogger('rent_recv');
Logger::configure('./w001_mo.xml');
$logging->info($log.'-----'.$output);
?>
