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
	$paymentUser   = trim(isset($_POST['paymentUser'])?$_POST['paymentUser']:'');
	$ditchId       = trim(isset($_POST['ditchId'])?$_POST['ditchId']:'');


	$log="unicomId=".$unicomId."outTradeNo=".$outTradeNo."&subject=".$subject."&description=".$description."&price=".$price."&quantity=".$quantity."&totalFee=".$totalFee."&appKey=".$appKey."&appName=".$appName."&callBackPath=".$callBackPath."&timeStamp=".$timeStamp."&paymentUser=".$paymentUser."&ditchId=".$ditchId;

	

	if(!empty($unicomId) && !empty($outTradeNo) && !empty($subject) && !empty($totalFee) && !empty($appKey) && !empty($appName) && !empty($callBackPath) && !empty($timeStamp)){

		$forward_status=0;
		$phone = substr($paymentUser,0,7);
		$sql = "select province from wraith_code_segment where code='$phone'";
		$result_p = exsql($sql);
		$provinces=mysqli_fetch_row($result_p);
		$province = $provinces[0];
		$deduction='';
		if(!empty($province)){
			$sql = "select up_deduction from wraith_wo_deduction where province='$province' and name='wo_sdk'";
			$result_d = exsql($sql);
			$deductions=mysqli_fetch_row($result_d);
			$deduction = $deductions[0];
			if($deduction == ''){
				$sql = "select up_deduction from wraith_wo_deduction where province='默认' and name='wo_sdk'";
				$result_dd = exsql($sql);
				$deductionsd=mysqli_fetch_row($result_dd);
				$deduction = $deductionsd[0];
			}
		}else{
			$province='未知';
		}


		$rand = rand(1,100);
		if(!empty($deduction) && $rand>=$deduction){
			$forward_status=1;
		}
		$sql="insert into wraith_wo_sdk(unicomId ,outTradeNo,subject,description,price,quantity,totalFee,appKey,appName,callBackPath,timeStamp,forward_status,paymentUser,ditchId,province) values('$unicomId','$outTradeNo','$subject','$description','$price','$quantity','$totalFee','$appKey','$appName','$callBackPath','$timeStamp','$forward_status','$paymentUser','$ditchId','$province')";
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
