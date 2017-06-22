<?php

include('phue.php');
require_once __DIR__ . '/vendor/autoload.php';
/**
* Debugging output

$req_dump = print_r($_POST, TRUE);
$fp = fopen('request.log', 'a');
fwrite($fp, $req_dump);
fclose($fp);

*/

//Check if auth key has been send
$p_strKey = '';
$strKey = '246809864334680';
$blnAccess = false;
if(isset($_POST['key'])) $p_strKey = $_POST['key'];
//Testing functions, only enable for testing
//if(isset($_GET['key'])) $p_strKey = $_GET['key'];
if($strKey == $p_strKey) $blnAccess = true;

//Start pushover and hue alert
if ($blnAccess == true) {
	
	echo 'blaaaaa', $blnAccess, ' key ', $strKey;
	$push = new Pushover();
	$push->setToken('paste token ID');
	$push->setUser('paste user ID');

	$push->setTitle('Doorbell');
	$push->setMessage('Voordeur aangebeld: ' .date("H:i:s"));
	$push->setUrl('');
	$push->setUrlTitle('Deurbel voordeur');

	$push->setDevice('');
	$push->setPriority(2);
	$push->setRetry(30); //Used with Priority = 2; Pushover will resend the notification every 60 seconds until the user accepts.
	$push->setExpire(60); //Used with Priority = 2; Pushover will resend the notification every 60 seconds for 3600 seconds. After that point, it stops sending notifications.
	$push->setCallback('');
	$push->setTimestamp(date("H:i:s"));
	$push->setDebug(true);
	$push->setSound('bike');

	$strPushStatus = $push->send();
    
	$objPhue = new Phue();
	$strLightStatus = $objPhue->Call_Light();
	echo '<pre>';
	print_r($strPushStatus);
	print_r($strLightStatus);
	echo '</pre>';
}
?>