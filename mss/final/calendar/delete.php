<?php
require('../database.php');
require('../user.php');
require('event.php');
session_start();
$user_id = $_SESSION['email'];
database_connect();
$result = retrieve_user($user_id);
database_disconnect();

$calid = $_REQUEST['calid'];
if ($result[0]['admin'] == 1)
{
   database_connect();
   $result = delete_event($calid);
   database_disconnect();
   header('location:manage.php');
}
else
{
   echo '<p>Access Denied!</p>';
}
?>

