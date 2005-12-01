<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
    <head>
      <title>Calendar</title>
      <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
      <link rel="stylesheet" type="text/css" href="calendar.css" />
    </head>
    <body>
    <?php
        require(strtolower(dirname(__FILE__)).'/Calendar.php');
        $cal = new Calendar();
        $today = $cal->getToday();
        $cal->htmlCalendar();
    ?>
    </body>
</html>