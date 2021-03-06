<?php

$ptr = "/";

$author = "Cameron Palmer";
$siteurl = "cameronpalmer.com";
$root_dir = dirname(__FILE__);
if (file_exists("{$root_dir}/blog_root.php")) include("{$root_dir}/blog_root.php");

function doheader($t="",$id="main") {
    global $author;
header("Content-Type: text/html; charset=utf-8");
echo '<'.'?xml version="1.0" encoding="utf-8"?>'."\n";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title><?=$t?> (<?echo $author?>)</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link rel="shortcut icon" type="image/x-icon" href="<?echo $ptr?>favicon.ico" />
    <link rel="alternate" title="RSS 2.0" href="<?echo $ptr?>rss/2.0" type="application/rss+xml" />
    <link rel="stylesheet" type="text/css" href="<?echo $ptr?>main.css" />
    <script type="text/javascript" src="<?echo $ptr?>dom/annimg.js"></script>
    <link rel="stylesheet" type="text/css" href="<?echo $ptr?>dom/annimg.css" />
    <?php if($id=="post") {?><script type="text/javascript" src="<?echo $ptr?>dom/validator.js"></script><?php } ?>
</head>
<body id="www-cameronpalmer-com">
<h1 class="hide"><?echo $author?></h1>
<div id="container_container"><div id="container">
<?php

}

function dofooter() {
    global $ptr, $siteurl;
?><!-- end entries -->

</div>
<div id="sidebar">
<h1><a href="<?echo $ptr . "../"?>" title="Return Home"><?echo $siteurl?></a></h1>
<h2>Navigation</h2>
<ul id="menu">
    <li><a href="<?echo $ptr?>">Blog</a></li>
    <li><a href="<?echo $ptr?>ark">Archives</a></li>
    <li><a href="<?echo $ptr?>about">About Me</a></li>
    <li><a href="../">Main Page</a></li>
</ul>

</div>

</div></body></html>
<?php
}


function dolastmod($files=false) {
	$m=0;

	if($files===false) {
		$files=array();
	} else if(is_string($files)) {
		$files=array($files);
	}
	$files[]=$_SERVER['SCRIPT_FILENAME'];
	$files[]=realpath("template.php");
	$files[]=realpath("_config.php");
	
	foreach($files as $f) {
		if(filemtime($f) > $m) $m=filemtime($f);
	}
		
	$tsstring = gmdate("D, d M Y H:i:s ", $m) . "GMT";
	
	$not_matching=false;
	if(isset($_SERVER["HTTP_IF_MODIFIED_SINCE"]) && ($_SERVER["HTTP_IF_MODIFIED_SINCE"] != $tsstring)) $not_matching = true;
	if(isset($_SERVER["HTTP_IF_NONE_MATCH"]) && ($_SERVER["HTTP_IF_NONE_MATCH"] != '"'.md5($timestamp).'"')) $not_matching = true;
	
	if((isset($_SERVER["HTTP_IF_MODIFIED_SINCE"]) || isset($_SERVER["HTTP_IF_NONE_MATCH"])) && !$not_matching) {
	  header("HTTP/1.1 304 Not Modified");
	  exit();
	} else {
	  header("Last-Modified: " . $tsstring);
	  header("ETag: \"".md5($timestamp)."\"");
	}

	header("Content-Type: text/html; charset=UTF-8");
	ob_start("contentlength");
}

function contentlength($s) {
  $size = ob_get_length();
  header("Content-Length: {$size}");
  return $s;
}


function mprint_r($a) {
echo "<pre>";
print_r($a);
echo "</pre>";
}

function mdie($s) {
	header("HTTP/1.0 404 Not Found");
	doheader("Error", "err");
	echo "<div class=\"warn\">\n	<h3>Error</h3>\n	<p>";
	echo htmlentities($s);
	echo "</p>\n</div>\n\n";
	dofooter();
	exit();
}

function unserialize_file($fn) {
	return unserialize(file_get_contents($fn));
}

function serialize_file($fn,$a) {
	if(!$f=@fopen($fn,"wb")) return false;
	fputs($f,serialize($a));
	fclose($f);
	return true;
}

?>
