<?php
require('../database.php');
require('../user.php');
require('vote.php');
session_start();
if (!$_SESSION['email']) 
   header("location:../login.php");
$user_id = $_SESSION['email'];
$first = $_SESSION['first_name'];

database_connect();
$result = retrieve_user($user_id);
$admin = $result[0]['admin'];

if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
   $vote_date = date("Y-m-d");   
   $vote_value = $_REQUEST['vote'];
   vote($user_id, $vote_date, $vote_value);
   database_disconnect();
   header("location:../main.php");
}
database_disconnect();
$title = "Lone Star Community - Vote";
$leftbar = "../leftbar.php";
require('../stc-template.php');
if ($admin)
   echo "<p><a href=\"results.php\">Results</a></p>";
?>

<h1>Cast Your Vote</h1>
<p align="left">Honestly, do you all agree that this is the last time we have kids do our website?</p>

<form action="<? echo $_SERVER['PHP_SELF']?>" method="post">
<table>
   <tr>
       <td><input type="radio" name="vote" value="0" /></td><td>Yes</td>
   </tr>
   <tr>
       <td><input type="radio" name="vote" value="1" /></td><td>No</td>
   </tr>
</table>
<p><input type="submit" name="submit" value="Vote" /></p>  			 
</form>



