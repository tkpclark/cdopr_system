<?php
//PC网游计费
require_once "./mysql.php";
require_once "../../../../log4php/php/Logger.php";
try{
	$output = 400;
	$callBack=$curl_errno=$curl_error='初始值';

	$app_name   = trim(isset($_REQUEST['app_name'])?$_REQUEST['app_name']:'');
	$app_id     = trim(isset($_REQUEST['app_id'])?$_REQUEST['app_id']:'');
	//$pay_Tel  = iconv('UTF-8', 'UTF-8',trim(isset($_REQUEST['pay_Tel'])?$_REQUEST['pay_Tel']:''));
	$pay_Tel    = trim(isset($_REQUEST['pay_Tel'])?$_REQUEST['pay_Tel']:'');
	$pay_code   = trim(isset($_REQUEST['pay_code'])?$_REQUEST['pay_code']:'');
	$cp_code    = trim(isset($_REQUEST['cp_code'])?$_REQUEST['cp_code']:'');
	$pay_time   = trim(isset($_REQUEST['pay_time'])?$_REQUEST['pay_time']:'');
	$pay_otder  = trim(isset($_REQUEST['pay_otder'])?$_REQUEST['pay_otder']:'');
	$sec_cp     = trim(isset($_REQUEST['sec_cp'])?$_REQUEST['sec_cp']:'');
	$pay_status = trim(isset($_REQUEST['pay_status'])?$_REQUEST['pay_status']:'');
	
	
	$log="app_name=".$app_name."&app_id=".$app_id."&pay_Tel=".$pay_Tel."&pay_code=".$pay_code."&cp_code=".$cp_code."&pay_time=".$pay_time."&pay_otder=".$pay_otder."&sec_cp=".$sec_cp."&pay_status=".$pay_status;


	/*$phone = substr($paymentUser,0,7);
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
	}*/
	

	if(!empty($app_name) && !empty($app_id) && !empty($pay_Tel) && !empty($cp_code) && !empty($pay_time) && !empty($pay_otder) && !empty($sec_cp) && !empty($pay_code) && $pay_status!==null)
	{
		$sql="select price from wraith_pc_games_code where code=$pay_code";
		$result_price = exsql($sql);
		$price=mysqli_fetch_row($result_price);
		$price = $price[0];
		if(!empty($price))
		{
			//if(empty($province)){$province='未知';}
			$sql="insert into wraith_pc_games(id,app_name,app_id,pay_Tel,pay_code,cp_code,pay_time,pay_otder,sec_cp,pay_status,accept_time,price) values('','$app_name','$app_id','$pay_Tel','$pay_code','$cp_code','$pay_time','$pay_otder','$sec_cp','$pay_status',NOW(),'$price')";
			if($result = exsql($sql))
			{
					$insert_id = $mysqli->insert_id;
					//$header = "Content-type: text/xml";
					$curlPost = "port=106901336278&ServiceID=".$pay_code."&MobileNumber=".$pay_Tel."&LinkId=".$pay_otder."&ChargeResult=".$pay_status;

					
					$rand = rand(1,100);

					$forward_status=$forward_mr_time=$forward_mr_result='';
					//if(empty($deduction) || $rand<=$deduction){
						$ch = curl_init();
						curl_setopt($ch,CURLOPT_URL,"http://122.0.66.188:18886/data/postmo151.aspx");
						
						curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
						//curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
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

					//}

					$output ='OK';
					$sql = "update wraith_pc_games set forward_status='$forward_status' ,forward_mr_time='$forward_mr_time',forward_mr_result='$forward_mr_result' where id=$insert_id";
					exsql($sql);
			}
		}
	}
}catch(Exception $e) {
	$output =  $e->getMessage();
}
//根据接口规范返回输出结果
echo $output;
//打印日志
$logging = Logger::getLogger('pcgames');
Logger::configure('./pcgames.xml');
$logging->info($log.'-----output='.$output.'&callback='.$callBack."&curl_errno=".$curl_errno."&curl_error=".$curl_error);

						
?>
