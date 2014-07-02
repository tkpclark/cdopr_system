<?php
//WO+WEB计费
require_once "./mysql.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$callBack='';

	$paymentUser    = trim(isset($_POST['paymentUser'])?$_POST['paymentUser']:'');
	$outTradeNo    = trim(isset($_POST['outTradeNo'])?$_POST['outTradeNo']:'');
	$subject       = iconv('UTF-8', 'UTF-8',trim(isset($_POST['subject'])?$_POST['subject']:''));

	$description   = iconv('UTF-8', 'UTF-8',trim(isset($_POST['description'])?$_POST['description']:''));
	$price         = trim(isset($_POST['price'])?$_POST['price']:'');
	$quantity      = trim(isset($_POST['quantity'])?$_POST['quantity']:'');

	$totalFee      = trim(isset($_POST['totalFee'])?$_POST['totalFee']:'');
	$callBackPath  = trim(isset($_POST['callBackPath'])?$_POST['callBackPath']:'');
	$timeStamp     = trim(isset($_POST['timeStamp'])?$_POST['timeStamp']:'');
	$resultCode         = trim(isset($_POST['resultCode'])?$_POST['resultCode']:'');
	$resultDescription  = iconv('UTF-8', 'UTF-8',trim(isset($_POST['resultDescription'])?$_POST['resultDescription']:''));

	$log="paymentUser=".$paymentUser."outTradeNo=".$outTradeNo."&subject=".$subject."&description=".$description."&price=".$price."&quantity=".$quantity."&totalFee=".$totalFee."&resultCode=".$resultCode."&resultDescription=".$resultDescription."&callBackPath=".$callBackPath."&timeStamp=".$timeStamp;

	if(!empty($paymentUser) && !empty($outTradeNo) && !empty($subject) && !empty($totalFee) && !empty($callBackPath) && !empty($timeStamp) && !empty($resultCode)){
		$sql="insert into wraith_wo_web(paymentUser,outTradeNo,subject,description,price,quantity,totalFee,callBackPath,timeStamp,resultCode,resultDescription) values('$paymentUser','$outTradeNo','$subject','$description','$price','$quantity','$totalFee','$callBackPath','$timeStamp','$resultCode','$resultDescription')";
		if($result = exsql($sql)){

				$curlPost = "paymentUser=".$paymentUser."&outTradeNo=".$outTradeNo."&subject=".$subject."&description=".$description."&price=".$price."&quantity=".$quantity."&totalFee=".$totalFee."&resultCode=".$resultCode."&resultDescription=".$resultDescription."&timeStamp=".$timeStamp;
				$ch = curl_init();
				curl_setopt($ch,CURLOPT_URL,$callBackPath);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
				curl_setopt($ch, CURLOPT_HEADER, 0);
				curl_setopt($ch, CURLOPT_DNS_USE_GLOBAL_CACHE, false);
				curl_setopt($ch, CURLOPT_POST, 1);
				curl_setopt($ch, CURLOPT_POSTFIELDS, $curlPost);
				curl_setopt($ch, CURLOPT_TIMEOUT, 3);
				$data = curl_exec($ch);
				curl_close($ch);

				$output =1;
				$callBack=$data;
		}
	}
}catch(Exception $e) {
	$output =  $e->getMessage();
}
//根据接口规范返回输出结果
echo $output;
//打印日志
$logging = Logger::getLogger('w002_mo');
Logger::configure('./w002_mo.xml');
$logging->info($log.'-----output='.$output.'callback='.$callBack);
?>
