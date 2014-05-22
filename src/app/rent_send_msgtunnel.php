<?php

function send_to_destination_msgtunnel($request)
{
	//msgtunnel

	global $logging;
	global $mysqli;
	/**
	 * 
	 * example
	 * http://ip/recv_mo_sms?spnumber=111&message=222&phone=3333&linkid=000000000000
	 * http://ip/recv_mr_sms?msgstatus=DELIVED&linkid=000000000000
	 */
	
	if($request['deal_flag']==0)//send mo
	{
		$url = sprintf("http://localhost/recv_mo_sms?spnumber=%s&message=%s&phone=%s&linkid=%s",$request['sp_number'],$request['message'],$request['phone_number'],$request['linkid']);
		$deal_flag = 1;
	}
	else if($request['deal_flag']==1 and strlen($request['report']) >= 1)//send mr
	{
		$url = sprintf("http://localhost/recv_mr_sms?msgstatus=%s&linkid=%s",$request['report'],$request['linkid']);
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


