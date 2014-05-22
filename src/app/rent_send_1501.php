<?php
require_once('des.php');


function send_to_destination_1501($request)
{
	//齐顺
	//mo 加密前参数依次为 手机号#指令#长号码#linkid
	//mr 加密前参数依次为 手机号#指令#长号码#linkid#状态报告
	
	global $logging;
	global $mysqli;

	///des
	$key='wraith55';
	$Des_Crypt = new Des_Crypt($key);
	
	if($request['deal_flag']==0)//send mo
	{
		$data = sprintf("%s#%s#%s#%s",$request['phone_number'],$request['message'],$request['sp_number'],$request['linkid']);
		//plaintext
		$logging->info("data plain text:".$data);
		$data = $Des_Crypt->encrypt($data,$key,true);
		$data = urlencode($data);
		$url = "http://124.160.238.117/mobile/servlet/ReceiverSpzyData?data=".$data;
		$deal_flag = 1;
	}
	else if($request['deal_flag']==1 and strlen($request['report']) >= 1)//send mr
	{
		$data = sprintf("%s#%s#%s#%s#%s",$request['phone_number'],$request['message'],$request['sp_number'],$request['linkid'],$request['report']);
		//plaintext
		$logging->info("data plain text:".$data);
		$data = $Des_Crypt->encrypt($data,$key,true);
		$data = urlencode($data);
		$url = "http://124.160.238.117/mobile/servlet/ReceiverSpzyMR?data=".$data;
		$deal_flag = 2;
	}
	else//wait for report
	{
		return;
	}
	

	$logging->info("url:".$url);
	
	
	$resp = file_get_contents($url);
	if(1)//success
	{
		update_deal_flag($request, $deal_flag);
	}
	else
	{
		$logging->info("ERROR:resp:".$resp);
	}


}

