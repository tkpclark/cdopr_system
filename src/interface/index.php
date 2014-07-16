<?php  
$paymentUser    = trim(isset($_REQUEST['paymentUser'])?$_REQUEST['paymentUser']:'');
if(!empty($paymentUser)){
echo 'ok';
}else{
echo 'no';
}
?> 
