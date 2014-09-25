<?php
require_once "./mysql.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$unicomId    = trim(isset($_POST['unicomId'])?$_POST['unicomId']:'');
	$outTradeNo  = trim(isset($_POST['outTradeNo'])?$_POST['outTradeNo']:'');
	$status      = trim(isset($_POST['status'])?$_POST['status']:'');
	$totalFee    = trim(isset($_POST['totalFee'])?$_POST['totalFee']:'');
	$subject     = trim(isset($_POST['subject'])?$_POST['subject']:'');
	$paymentTime = trim(isset($_POST['paymentTime'])?$_POST['paymentTime']:'');
	$appKey      = trim(isset($_POST['appKey'])?$_POST['appKey']:'');

	$log="unicomId=".$unicomId."outTradeNo=".$outTradeNo."&subject=".$subject."&status=".$status."&totalFee=".$totalFee."&appKey=".$appKey."&paymentTime=".$paymentTime;
	
	if(!empty($unicomId) && !empty($status) && !empty($outTradeNo)){

		$sql = "select down_deduction from wraith_wo_deduction where province='默认' and name='wo_sdk'";
		$result_dd = exsql($sql);
		$deductionsd=mysqli_fetch_row($result_dd);
		$deduction = $deductionsd[0];

		$sql = "select forward_status from wraith_wo_sdk where unicomId='$unicomId'";
		$result = exsql($sql);
		$forward_status=mysqli_fetch_row($result);
		$forward_status = $forward_status[0];
		$rand = rand(1,100);
		if(!empty($deduction) && $rand>=$deduction && $forward_status==0){
			$sql="update wraith_wo_sdk set status='$status',paymentTime='$paymentTime',forward_status='2' where unicomId='$unicomId'";
		}else{
			$sql="update wraith_wo_sdk set status='$status',paymentTime='$paymentTime' where unicomId='$unicomId'";
		}

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
Logger::configure('./w001_mr.xml');
$logging->info($log.'-----'.$output);
?>
