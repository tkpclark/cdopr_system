<?php
//WO+WEB计费
require_once "./mysql.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$callBack=$curl_errno=$curl_error='初始值';

	$paymentUser    = trim(isset($_REQUEST['paymentUser'])?$_REQUEST['paymentUser']:'');
	$outTradeNo    = trim(isset($_REQUEST['outTradeNo'])?$_REQUEST['outTradeNo']:'');
	$subject       = iconv('UTF-8', 'UTF-8',trim(isset($_REQUEST['subject'])?$_REQUEST['subject']:''));

	$description   = iconv('UTF-8', 'UTF-8',trim(isset($_REQUEST['description'])?$_REQUEST['description']:''));
	$price         = trim(isset($_REQUEST['price'])?$_REQUEST['price']:'');
	$quantity      = trim(isset($_REQUEST['quantity'])?$_REQUEST['quantity']:'');

	$totalFee      = trim(isset($_REQUEST['totalFee'])?$_REQUEST['totalFee']:'');
	$callBackPath  = trim(isset($_REQUEST['callBackPath'])?$_REQUEST['callBackPath']:'');
	$timeStamp     = trim(isset($_REQUEST['timeStamp'])?$_REQUEST['timeStamp']:'');
	$resultCode         = trim(isset($_REQUEST['resultCode'])?$_REQUEST['resultCode']:'');
	$resultDescription  = iconv('UTF-8', 'UTF-8',trim(isset($_REQUEST['resultDescription'])?$_REQUEST['resultDescription']:''));
	$ditchId       = trim(isset($_REQUEST['ditchId'])?$_REQUEST['ditchId']:'');
	
	
	$log="ditchId=".$ditchId."&paymentUser=".$paymentUser."&outTradeNo=".$outTradeNo."&subject=".$subject."&description=".$description."&price=".$price."&quantity=".$quantity."&totalFee=".$totalFee."&resultCode=".$resultCode."&resultDescription=".$resultDescription."&callBackPath=".$callBackPath."&timeStamp=".$timeStamp;

	if(empty($ditchId)){
		$ditchId='w001';
	}

	$phone = substr($paymentUser,0,7);
	$sql = "select province from wraith_code_segment where code='$phone'";
	$result_p = exsql($sql);
	$provinces=mysqli_fetch_row($result_p);
	$province = $provinces[0];
	$deduction='';
	if(!empty($province)){
		$sql = "select down_deduction from wraith_wo_deduction where province='$province' and name='w001'";
		$result_d = exsql($sql);
		$deductions=mysqli_fetch_row($result_d);
		$deduction = $deductions[0];
		if($deduction == ''){
			$sql = "select down_deduction from wraith_wo_deduction where province='默认' and name='w001'";
			$result_dd = exsql($sql);
			$deductionsd=mysqli_fetch_row($result_dd);
			$deduction = $deductionsd[0];
		}
	}
	

	if(!empty($paymentUser) && !empty($outTradeNo) && !empty($subject) && !empty($totalFee) && !empty($callBackPath) && !empty($timeStamp) && $resultCode!==null){
		if(empty($province)){$province='未知';}
		$sql="insert into wraith_wo_web(paymentUser,outTradeNo,subject,description,price,quantity,totalFee,callBackPath,timeStamp,resultCode,resultDescription,province,ditchId) values('$paymentUser','$outTradeNo','$subject','$description','$price','$quantity','$totalFee','$callBackPath','$timeStamp','$resultCode','$resultDescription','$province','$ditchId')";
		if($result = exsql($sql)){
				$insert_id = $mysqli->insert_id;
				$curlPost = "paymentUser=".$paymentUser."&outTradeNo=".$outTradeNo."&subject=".$subject."&description=".$description."&price=".$price."&quantity=".$quantity."&totalFee=".$totalFee."&resultCode=".$resultCode."&resultDescription=".$resultDescription."&timeStamp=".$timeStamp;
				
				$rand = rand(1,100);

				$forward_status=$forward_mr_time=$forward_mr_result='';
				if(empty($deduction) || $rand<=$deduction){
					$ch = curl_init();
					curl_setopt($ch,CURLOPT_URL,$callBackPath);
					curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
					curl_setopt($ch, CURLOPT_HEADER, 0);
					curl_setopt($ch, CURLOPT_DNS_USE_GLOBAL_CACHE, false);
					curl_setopt($ch, CURLOPT_POST, 1);
					curl_setopt($ch, CURLOPT_POSTFIELDS, $curlPost);
					curl_setopt($ch, CURLOPT_TIMEOUT, 3);
					$data = curl_exec($ch);
					//出错则显示错误信息
					$curl_errno = curl_errno($ch); 
					$curl_error = curl_error($ch); 
					curl_close($ch);
					$callBack=$data;
					$forward_status=1;
					$forward_mr_time=date('Y-m-d H:i:s',time());
					if(trim($callBack)=='ok'){
						$forward_mr_result=1;
					}else{
						$forward_mr_result=2;
					}

				}

				$output =1;
				$sql = "update wraith_wo_web set forward_status='$forward_status' ,forward_mr_time='$forward_mr_time',forward_mr_result='$forward_mr_result' where id=$insert_id";
				exsql($sql);
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
$logging->info($log.'-----output='.$output.'&callback='.$callBack."&curl_errno=".$curl_errno."&curl_error=".$curl_error);
?>
